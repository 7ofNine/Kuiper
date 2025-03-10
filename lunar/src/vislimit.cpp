/* vislimit.cpp: functions for limiting magnitude/sky brightness computations

Copyright (C) 2010, Project Pluto

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.    */

/*
The computations for sky brightness and limiting magnitude can be
logically broken up into several pieces.  Some computations depend
on things that are constant for a given observing site and time:
the lunar and solar zenith distances,  the air masses to those objects,
the temperature and relative humidity,  and so forth.  For use in Guide,
I expect to compute brightness at many points in the sky,  while all
these other values hold constant.  So my first step (after putting
lat/lon and these other data into the BRIGHTNESS_DATA struct) is to
call the set_brightness_params() function.  This function does a lot
of "setup work",  figuring out the absorption per unit air mass at
various wavelengths from various causes (gas,  aerosol,  ozone),
the number of air masses to the sun and moon,  and so forth.

Once you've done all this,  you can call compute_sky_brightness() for
any point in the sky.  You do need to provide the zenith angle,  and the
angular distance of that point from the moon and sun.  The brightnesses
are returned in the brightness[] array.  The 'mask' value can be used to
specify which of the five bands is to be computed.  (For example,  if I
use this to make a realistic sky background,  I may just concern myself
with the V band... maybe with B and R if I want to attempt a colored sky.
In either case,  computing all five bands would be excessive.)

Next,  you can call compute_extinction( ) to set any or all of the five
extinction values.  Normally,  I wouldn't see much use for this data.
But you do need to have that data if you intend to call the
compute_limiting_mag( ) function.

All of what follows is adapted from Brad Schaefer's article and code
on pages 57-60,  May 1998 _Sky & Telescope_,  "To the Visual Limits",

https://archive.org/stream/Sky_and_Telescope_1998-05-pdf/#page/n109/mode/2up

which is in turn based on his articles in _Publications of the Astronomical
Society of the Pacific_ :

http://articles.adsabs.harvard.edu/full/1990PASP..102..212S
http://articles.adsabs.harvard.edu/full/1991PASP..103.1033K

NOTICE that I modified his test conditions.  He had the moon and sun
well below the horizon;  I found that this didn't make testing
contributions from those objects any easier,  so I put them where they
could contribute more brightness.

At some point when I have the time,  I'll break out the main( ) portion,
tack in the code for CCD mag limits on page 121 of the same magazine,
and make proper header files.

'Brightness',  by the way,  is in mystery units.  But line 260 of
'vislimit.cpp' divides the brightnesses by 1.11e-15 to get nanoLamberts.
One could (and in the example test code,  I do) divide by 1.11e-6 to
get lamberts.

Then,  the relation between sky brightness in nanoLamberts (B) and in
magnitudes/arcsec^2 is given in the second PASP paper above as
equation (1)

B = 34.08 exp( 20.7233 - 0.92104V)

   Some rearrangement gets you the inverse formula

V = 3.8312 - 2.5 * log10( B)

   which provides the required conversion from brightness in
lamberts (which I've never seen an astronomer use) to brightness in
magnitudes/arcsec^2 (which is used frequently).  If instead we
have a brightness Bm = B * 1.11e-6 "mystery units",

V = 3.8312 - 2.5 * log10( Bm / 1.11e-6)
  = -2.5 * log10( Bm) - 11.055

See the 'test program' below for example usage. */

#ifdef TEST_STATEMENTS
#include <cstdio>
#endif

#include <cmath>
#include "vislimit.h"

#define MAG_TO_BRIGHTNESS( X) (exp( -.4 * (X) * LOG_10))
#define BRIGHTNESS_TO_MAG( X) (-2.5 * log( X) / LOG_10)
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923
#define LOG_10 2.3025850929940456840179914546843642076011014886287729760333279009675726

#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

static double compute_air_mass( const double zenith_angle)
{
   double rval = 40.;
   const double cos_ang = cos( zenith_angle);

   if( cos_ang > 0.)
      rval = 1. / (cos_ang + .025 * exp( -11. * cos_ang));
   return( rval);
}

static double compute_f_factor( const double obj_dist)
{
   const double obj_dist_degrees = obj_dist * 180. / PI;
   const double cos_dist = cos( obj_dist);
   double rval;

   rval = 6.2e+7 / (obj_dist_degrees * obj_dist_degrees)
                        + exp( LOG_10 * (6.15 - obj_dist_degrees / 40.));
   rval += 229086. * (1.06 + cos_dist * cos_dist);  /* polarization term? */
   return( rval);
            /* Seen on lines 2210 & 2200 for the moon,  and on lines */
            /* 2320 & 2330 for the moon.  I've only foggy ideas what  */
            /* it means;  I think it attempts to compute the falloff in */
            /* scattered light from an object as a function of distance.  */
}

int /*DLL_FUNC*/ set_brightness_params( BRIGHTNESS_DATA *b)
{
   const double month_angle = (b->month - 3.) * PI / 6.;
   const double k_rayleigh_coeff =
                   .1066 * exp( -b->ht_above_sea_in_meters / 8200.);
   const double k_ozone_coeff = (3. + .4 * (b->latitude * cos( month_angle) -
                     cos( 3. * b->latitude))) / 3.;
   const double k_water_coeff = .94 * (b->relative_humidity / 100.) *
                       exp( b->temperature_in_c / 15.) *
                       exp( -b->ht_above_sea_in_meters / 8200.);
              /* 'moon_angle' = distance of moon from opposition,  in     */
              /* degrees.  It's near 0 degrees at full moon,  180 at new. */
   double moon_angle = 180. - b->moon_elongation * 180. / PI;
   double k_aerosol_coeff = .1 * exp( -b->ht_above_sea_in_meters / 1500.);
   int i;

   if( b->relative_humidity > 0.)
      {
      double humidity_param;

      if( b->relative_humidity >= 100.)
         humidity_param = 1000000.;
      else
         humidity_param = 1. - .32 / log( b->relative_humidity / 100.);
      k_aerosol_coeff *= exp( 1.33 * log( humidity_param));
      }
         /* Following line modified by BJG to avoid a discontinuity when */
         /* crossing the equator.  Aerosols tend to increase in summer. */
   k_aerosol_coeff *= 1. + sin( month_angle) * sin( b->latitude);

   b->year_term = 1. + .3 * cos( 2. * PI * (b->year - 1992) / 11.);
         /* accounts for a 30% variation due to sunspots over 11-yr cycle? */
         /* in which case it should be something more exact than '11.' I   */
         /* assume accuracy deteriorates for years far from 1992.          */
   b->air_mass_moon = compute_air_mass( b->zenith_ang_moon);
   b->air_mass_sun  = compute_air_mass( b->zenith_ang_sun);
   moon_angle = fabs( fmod( moon_angle, 360.));
   if( moon_angle > 180.)
      moon_angle = 360. - moon_angle;
   b->lunar_mag = -12.73 + moon_angle * (.026 +
                           4.e-9 * (moon_angle * moon_angle * moon_angle));
               /* line 2180 in B Schaefer code */
   for( i = 0; i < 5; i++)
      {
      static const double fourth_power_terms[5] =
                       { 5.155601, 2.441406, 1., 0.381117, 0.139470 };
      static const double one_point_three_power_terms[5] =
                       { 1.704083, 1.336543, 1., 0.730877, 0.527177 };
      static const double oz[5] = {0., 0., .031, .008, 0.};
      static const double wt[5] = {.074, .045, .031, .02, .015};

      b->kr[i] = k_rayleigh_coeff * fourth_power_terms[i];
      b->ka[i] = k_aerosol_coeff * one_point_three_power_terms[i];
      b->ko[i] = k_ozone_coeff * oz[i];
      b->kw[i] = k_water_coeff * wt[i];

      b->k[i] = b->kr[i] + b->ka[i] + b->ko[i] + b->kw[i];
      b->c3[i] = MAG_TO_BRIGHTNESS( b->k[i] * b->air_mass_moon);
             /* compute dropoff in lunar brightness from extinction: 2200 */
      b->c4[i] = MAG_TO_BRIGHTNESS( b->k[i] * b->air_mass_sun);
      }
   return( 0);
}


   /* If all you want is the sky brightness,  all the data concerning */
   /* separate air masses for gas, aerosols,  and ozone and such is   */
   /* an unnecessary drain on computation.  So that's broken out as a */
   /* separate process in compute_extinction( ). */

int /*DLL_FUNC*/ compute_extinction( BRIGHTNESS_DATA *b)
{
   const double cos_zenith_ang = cos( b->zenith_angle);
   const double tval = sin( b->zenith_angle) / (1. + 20. / 6378.);
   int i;

   b->air_mass_gas =
               1. / (cos_zenith_ang + .0286 * exp( -10.5 * cos_zenith_ang));
   b->air_mass_aerosol =
               1. / (cos_zenith_ang + .0123 * exp( -24.5 * cos_zenith_ang));
   b->air_mass_ozone = 1. / sqrt( 1. - tval * tval);
   for( i = 0; i < 5; i++)
      if( (b->mask >> i) & 1)
         b->extinction[i] = (b->kr[i] + b->kw[i]) * b->air_mass_gas +
                             b->ka[i] * b->air_mass_aerosol +
                             b->ko[i] * b->air_mass_ozone;
   return( 0);
}

double /*DLL_FUNC*/ compute_limiting_mag( BRIGHTNESS_DATA *b)
{
   const double bl = b->brightness[2] / 1.11e-15;
   double c1, c2;
   double th, tval, rval;

   if( bl > 1500.)
      {
      c1 = 4.4668e-9;
      c2 = 1.2589e-6;
      }
   else
      {
      c1 = 1.5849e-10;
      c2 = 1.2589e-2;
      }
   tval = 1. + sqrt( c2 * bl);
   th = c1 * tval * tval;        /* brightness in foot-candles? */
   rval = -16.57 + BRIGHTNESS_TO_MAG( th) - b->extinction[2];
   return( rval);
}

int /*DLL_FUNC*/ compute_sky_brightness( BRIGHTNESS_DATA *b)
{
   const double sin_zenith = sin( b->zenith_angle);
   const double brightness_drop_2150 =
                 .4 + .6 / sqrt( 1.0 - .96 * sin_zenith * sin_zenith);
               /* Not sure what this is.. line 2150 in B Schaefer code */
               /* Probably means there's a drop in brightness as one   */
               /* moves away from the zenith toward the horizon?       */
   const double fm = compute_f_factor( b->dist_moon);
   const double fs = compute_f_factor( b->dist_sun);
   int i;

   b->air_mass = compute_air_mass( b->zenith_angle);
   for( i = 0; i < 5; i++)
      if( (b->mask >> i) & 1)
         {
         static const double bo[5] = {8.0e-14, 7.e-14, 1.e-13, 1.e-13, 3.e-13};
               /* Base sky brightness in each band */
         static const double cm[5] = {1.36, 0.91, 0.00, -0.76, -1.17 };
               /* Correction to moon's magnitude */
         static const double ms[5] = {-25.96, -26.09, -26.74, -27.26, -27.55 };
               /* Solar magnitude? */
         static const double mo[5] = {-10.93, -10.45, -11.05, -11.90, -12.70 };
               /* Lunar magnitude? */
         const double lunar_alt = PI / 2. - b->zenith_ang_moon;
         const double lunar_fadeout_fudge = 100.;
                    /* above is arbitrarily chosen to make the lunar */
                    /* contribution fade as the moon goes below the horizon */
         const double direct_loss = MAG_TO_BRIGHTNESS( b->k[i] * b->air_mass);
         const double bn =
                 bo[i] * b->year_term * brightness_drop_2150 * direct_loss;
         double twilight_brightness;
         double brightness_daylight;
         double brightness_moon =
                   MAG_TO_BRIGHTNESS( b->lunar_mag + cm[i] - mo[i] + 43.27);

         brightness_moon *= (1. - direct_loss);
                  /* Maybe computing how much of the lunar light gets */
                  /* scattered?   2240 */
         brightness_moon *= (fm * b->c3[i] + 440000. * (1. - b->c3[i]));
         if( lunar_alt < 0.)                   /* moon is below horizon */
            brightness_moon *= exp( lunar_fadeout_fudge * lunar_alt);
         twilight_brightness = ms[i] - mo[i] + 32.5 -
                           (90. - b->zenith_ang_sun * 180. / PI) -
                           b->zenith_angle / (2 * PI * b->k[i]);
                  /* above is in magnitudes,  so gotta do this: */
         twilight_brightness = MAG_TO_BRIGHTNESS( twilight_brightness);
                  /* above is line 2280,  B Schaefer code */
         twilight_brightness *= 100. / (b->dist_sun * 180. / PI);
         twilight_brightness *= 1. - MAG_TO_BRIGHTNESS( b->k[i]);
                  /* preceding line looks suspicious to me... line 2290 */
         brightness_daylight = MAG_TO_BRIGHTNESS( ms[i] - mo[i] + 43.27);
                     /* line 2340 */
         brightness_daylight *= (1. - direct_loss);
                     /* line 2350 */
         brightness_daylight *= fs * b->c4[i] + 440000. * (1. - b->c4[i]);
         b->brightness[i] = bn + brightness_moon +
                     min( brightness_daylight, twilight_brightness);
#ifdef TEST_STATEMENTS
         printf( "Brightnesses (%d): base %lg  moon %lg   twil %lg   sun %lg\n", i, bn,
                  brightness_moon, twilight_brightness, brightness_daylight);
#endif
         }
   return( 0);
}

#ifdef TEST_PROGRAM
#include <cstdio>
#include <cstdlib>

int main( const int argc, const char **argv)
{
   BRIGHTNESS_DATA b;
   int i;
   const char *band_name = "UBVRI";

   b.zenith_ang_moon = 40. * PI / 180.;
   b.zenith_ang_sun = 105. * PI / 180.;
   b.moon_elongation = 180. * PI / 180.;        /* full moon */
   b.ht_above_sea_in_meters = 1000.;
   b.latitude = 30. * PI / 180.;
   b.temperature_in_c = 15.;
   b.relative_humidity = 40.;
   b.year = 1998.;
   b.month = 2.;
               /* values varying across the sky: */
   b.zenith_angle = atof( argv[1]) * PI / 180.;
   b.dist_moon = 50. * PI / 180.;
   b.dist_sun = 40. * PI / 180.;
   b.mask = 31;

   for( i = 2; i < argc; i++)
      if( argv[i][0] == '-')
         switch( argv[i][1])
            {
            case 'a':
               b.ht_above_sea_in_meters = atof( argv[i] + 2);
               break;
            case 'd':
               b.dist_moon = atof( argv[i] + 2) * PI / 180.;
               break;
            case 'e':
               b.moon_elongation = atof( argv[i] + 2) * PI / 180.;
               break;
            case 's':
               b.zenith_ang_sun = (90. - atof( argv[i] + 2)) * PI / 180.;
               break;
            case 'l':
               b.zenith_ang_moon = (90. - atof( argv[i] + 2)) * PI / 180.;
               break;
            case 'h':
               b.relative_humidity = atof( argv[i] + 2);
               break;
            case 't':
               b.temperature_in_c = atof( argv[i] + 2);
               break;
            default:
               printf( "Option '%s' not recognized\n", argv[i]);
               break;
            }

   set_brightness_params( &b);
   compute_sky_brightness( &b);
   compute_extinction( &b);
   printf( "Band  k  mag/arcsec^2 Ext\n");
   for( i = 0; i < 5; i++)
      {
      const double brightness_in_mags_per_sq_arcsec =
                   -2.5 * log10( b.brightness[i]) - 11.055;

      printf( "%c : %6.3f %6.2f %6.3lf\n", band_name[i], b.k[i],
                  brightness_in_mags_per_sq_arcsec, b.extinction[i]);
      }
   printf( "Limiting magnitude: %.5lf\n", compute_limiting_mag( &b));
   return( 0);
}
#endif
