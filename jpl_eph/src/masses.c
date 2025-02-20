#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "jpleph.h"

/* Dumps planetary/lunar masses from JPL ephems.   */

#define N_MASSES 16

int main( const int argc, const char **argv)
{
   int i, n_constants;
   double emrat = 0., gmb = 0., au_in_km = 0.;
   void *p;
   double masses[N_MASSES];
   const char *names[N_MASSES] = { "Sun ", "Merc", "Venu", "EMB ", "Mars",
           "Jupi", "Satu", "Uran", "Nept", "Plut", "Eart", "Moon",
           "Cere", "Pall", "Juno", "Vest"   };
   const double seconds_per_day = 86400.;

   if( argc != 2)
      {
      fprintf( stderr, "'masses' takes the name of a JPL DE file as a"
                       " as a command-line argument.\n");
      fprintf( stderr, "It will output a list of planetary masses in a"
                       " table of the sort found\n");
      fprintf( stderr, "at the end of 'masses.cpp' (q.v.).\n");
      return( -1);
      }
   p = jpl_init_ephemeris( argv[1], NULL, NULL);
   if( !p)
      {
      printf( "JPL data not loaded from '%s'\n", argv[1]);
      printf( "Error code: %d\n", jpl_init_error_code( ));
      return( -1);
      }
   n_constants = (int)jpl_get_long( p, JPL_EPHEM_N_CONSTANTS);
   for( i = 0; i < n_constants; i++)
      {
      char constant_name[7];
      const double ephem_constant = jpl_get_constant( i, p, constant_name);

      if( constant_name[0] == 'G' && constant_name[1] == 'M'
                  && constant_name[3] ==  ' ')
         switch( constant_name[2])
            {
            case 'B':
               gmb = ephem_constant;
               break;
            case 'S':
               masses[0] = ephem_constant;
               break;
            case '1': case '2': case '4': case '5': case '6':
            case '7': case '8': case '9':
               masses[constant_name[2] - '0'] = ephem_constant;
               break;
            }
      if( !memcmp( constant_name, "EMRAT ", 6))
         emrat = ephem_constant;
      if( !memcmp( constant_name, "AU ", 3))
         au_in_km = ephem_constant;
      if( !memcmp( constant_name, "MA000", 5))
         {
         int idx = constant_name[5] - '0';

         if( idx >= 1 && idx <= 4)     /* Ceres, Pallas, Juno, Vesta */
            masses[idx + 11] = ephem_constant;
         }
      }
   masses[3] = gmb;
   masses[11] = gmb / (1 + emrat);     /* moon */
   masses[10] = gmb - masses[11];       /* earth */

   printf( "Data from %s\n", argv[1]);
   printf(
      "       mass(obj)/mass(sun)   mass(sun)/mass(obj)    GM (km^3/s^2)  GM (AU^3/day^2)\n");
   for( i = 0; i < N_MASSES; i++)
      printf( "%s %.15e %.15e %.15e %.15e\n", names[i],
            masses[i] / masses[0], masses[0] / masses[i],
            masses[i] * au_in_km * au_in_km * au_in_km /
                        (seconds_per_day * seconds_per_day), masses[i]);
   return( 0);
}

/* Results are :

Data from DE-432
       mass(obj)/mass(sun)   mass(sun)/mass(obj)    GM (km^3/s^2)
Merc 1.660114153054348e-07 6.023682155592479e+06 2.203177969072598e+04
Venu 2.447838287784771e-06 4.085237186582997e+05 3.248585874397545e+05
EMB  3.040432648022641e-06 3.289005598102476e+05 4.035032298380295e+05
Mars 3.227156037554996e-07 3.098703590290707e+06 4.282837461279101e+04
Jupi 9.547919101886966e-04 1.047348630972762e+03 1.267127623546989e+08
Satu 2.858856727222416e-04 3.497901767786634e+03 3.794058466740400e+07
Uran 4.366249662744965e-05 2.290295052370693e+04 5.794556384409937e+06
Nept 5.151383772628673e-05 1.941225977597307e+04 6.836527004611366e+06
Plut 7.350487833457740e-09 1.360453921776768e+08 9.755011621830380e+02
Sun  1.000000000000000e+00 1.000000000000000e+00 1.327124381789709e+11
Eart 3.003489614792921e-06 3.329460488475656e+05 3.986004298243866e+05
Moon 3.694303322972000e-08 2.706870315119437e+07 4.902800013642883e+03
Cere 4.725582914451237e-10 2.116141052021147e+09 6.271436303937109e+01
Pall 1.018229468217943e-10 9.820968958501590e+09 1.351317153528802e+01
Juno 1.371898570800420e-11 7.289168611179182e+10 1.820680042651692e+00
Vest 1.302666122601934e-10 7.676564106868835e+09 1.728799972636488e+01

*/
