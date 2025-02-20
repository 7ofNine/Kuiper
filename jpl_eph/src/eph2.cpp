/* Copyright (C) 2018, Project Pluto

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "watdefs.h"
// #include "lunar.h"
// #include "afuncs.h"
#include "jpleph.h"

#define PI 3.141592653589793238462643383279
#define J2000_OBLIQUITY  (23.4392911 * PI / 180)
#define AU_IN_KM 1.495978707e+8
#define SPEED_OF_LIGHT 299792.458

static void show_state_vector( const double *state_vect)
{
   double r2 = state_vect[0] * state_vect[0] +
               state_vect[1] * state_vect[1] + state_vect[2] * state_vect[2];

   printf( "%.11f %.11f %.11f  %.11f\n", state_vect[0], state_vect[1],
            state_vect[2], sqrt( r2));
   printf( "%.11f %.11f %.11f\n", state_vect[3], state_vect[4],
            state_vect[5]);
}

static void format_base_sixty( char *buff, double ang)
{
   int i;

   *buff++ = (ang < 0. ? '-' : '+');
   ang = fabs( ang);
   for( i = 0; i < 4; i++)
      {
      long ival = (long)floor( ang);
      static const long maxval[5] = { 100000L, 60L, 60L, 1000000000L, 0L };

      if( ival < 0L)
         ival = 0L;
      if( ival >= maxval[i])
         ival = maxval[i] - 1L;
      assert( ival >= 0L && ival < 1000000000L);
      snprintf( buff, 11, (i == 3 ? ".%09ld" : "%02ld "), ival);
      buff += (i == 2 ? 2 : 3);
      ang = (ang - (double)ival) * (double)maxval[i + 1];
      }
}

static double vector_length( const double *a)
{
   double rval2 = 0.;
   int i;

   for( i = 3; i; i--, a++)
      rval2 += *a * *a;
   return( sqrt( rval2));
}

#define SOLAR_SYSTEM_BARYCENTER 12

int main( const int argc, const char **argv)
{
   double state_vect2[6], jd = atof( argv[3]);
   const int home_planet = atoi( argv[1]), planet_no = atoi( argv[2]);
   double dist = 0., pvsun[6];
   int i, pass;
   void *p;
#ifdef _WIN32
   const char *filename = "d:\\guide_b\\jpl_eph\\sub_de.406";
#else
   const char *filename = "../de431/jpleph.431";
#endif

   assert( argc >= 4);
   printf( "Year approx %.3f\n", 2000. + (jd - 2451545.) / 365.25);
   p = jpl_init_ephemeris( filename, NULL, NULL);
   if( !p)
      {
      printf( "JPL data '%s' not loaded\n", filename);
      return( -1);
      }
   jpl_pleph( p, jd, SOLAR_SYSTEM_BARYCENTER, home_planet, state_vect2, 1);
   printf( "Observer state vector:\n");
   show_state_vector( state_vect2);
// for( i = 0; i < 3; i++)
//    pvsun[i] = jpl_get_pvsun( p)[i];
   for( pass = 0; pass < 4; pass++)
      {
      double state_vect[6], ang;
      char buff[30];

      jpl_pleph( p, jd - dist * AU_IN_KM / (SPEED_OF_LIGHT * 86400.),
                     SOLAR_SYSTEM_BARYCENTER, planet_no, state_vect, 1);
      if( pass == 3)
         {
         for( i = 0; i < 3; i++)
            pvsun[i] = state_vect[i] + jpl_get_pvsun( p)[i];
//          pvsun[i] += state_vect[i];
         printf( "%.11f %.11f %.11f\n", pvsun[0], pvsun[1], pvsun[2]);
         printf( "Dist to sun: %.11f\n", vector_length( pvsun));
         }
      for( i = 0; i < 6; i++)
         state_vect[i] -= state_vect2[i];
      dist = vector_length( state_vect);
      show_state_vector( state_vect);

      ang = atan2( state_vect[1], state_vect[0]) * 12. / PI;
      format_base_sixty( buff, ang + 12.);
      buff[15] = '\0';
      printf( "%s   ", buff + 1);      /* skip leading '+' */

      ang = -asin( state_vect[2] / dist) * 180. / PI;
      format_base_sixty( buff, ang);
      buff[14] = '\0';
      printf( "%s   %.11f (%.3f km)\n", buff, dist, dist * AU_IN_KM);
      }
   jpl_close_ephemeris( p);
   return( 0);
}
