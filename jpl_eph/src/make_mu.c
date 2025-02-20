/* make_mu.c: makes 'mu.txt' (list of asteroid masses) file for Find_Orb

Copyright (C) 2011, Project Pluto

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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "jpleph.h"

#define AU_IN_KM 1.49597870691e+8
#define GAUSS_K .01720209895
#define SOLAR_GM (GAUSS_K * GAUSS_K)
      /* GAUSS_K is a fixed constant. SOLAR_GM = 0.0002959122082855911025, */
      /* in AU^3/day^2; = 1.3271243994E+11 km3/s2 */

/* Run as

./make_mu ephem_filename

   The program will extract the header constants,  including asteroid masses,
from the ephemeris file.  It will then generate a 'mu.txt' file with asteroid
masses matching the specified order.  Compile with

gcc -Wall -Wextra -pedantic -Werror -o make_mu make_mu.c libjpl.a    */

#define MAX_ASTEROIDS 4000

int main( const int argc, const char **argv)
{
   void *p;
   int i, j, n_constants, n_asteroids;
   FILE *ifile = fopen( "/home/phred/.find_orb/NumberedMPs.txt", "rb");
   char buff[200], **idata = (char **)calloc( sizeof( char *), MAX_ASTEROIDS);

   if( argc < 2)
      {
      fprintf( stderr,
               "make_mu requires the name of a JPL ephemeris file as a\n"
               "command-line argument.\n");
      return( -1);
      }
   if( !ifile)
      {
      fprintf( stderr, "Didn't open asteroid name data\n");
      return( -1);
      }

   p = jpl_init_ephemeris( argv[1], NULL, NULL);

   if( !p)
      {
      fprintf( stderr, "JPL data not loaded from '%s'\n", argv[1]);
      fprintf( stderr, "Error code: %d\n", jpl_init_error_code( ));
      return( -1);
      }
   n_constants = (int)jpl_get_long( p, JPL_EPHEM_N_CONSTANTS);
   for( i = 0; i < n_constants; i++)
      {
      char constant_name[7];
      const double ephem_constant = jpl_get_constant( i, p, constant_name);

      if( constant_name[0] == 'M' && constant_name[1] == 'A'
                  && (constant_name[2] >= '0' && constant_name[2] <= '7'))
          {
          const int asteroid_number = atoi( constant_name + 2);

          fseek( ifile, 0L, SEEK_SET);
          for( j = 0; j < asteroid_number; j++)
              if( !fgets( buff, sizeof( buff), ifile))
                  return( -3);
          j = 28;
          while( j && buff[j - 1] == ' ')
             j--;
          buff[j] = '\0';
          idata[n_asteroids] = (char *)malloc( 100);
          snprintf( idata[n_asteroids], 100, "%6d%12.5e %s\n", asteroid_number,
                  ephem_constant / SOLAR_GM, buff + 9);
          n_asteroids++;
          }
      }
   jpl_close_ephemeris( p);
   fclose( ifile);

   ifile = fopen( "mu1.txt", "rb");
   assert( ifile);
   while( fgets( buff, sizeof( buff), ifile))
      if( *buff == ';')       /* it's a comment */
         printf( "%s", buff);
      else
         {
         bool got_it = false;

         for( i = 0; !got_it && i < n_asteroids; i++)
            if( idata[i] && atoi( buff) == atoi( idata[i]))
               {
               printf( "%s", idata[i]);
               free( idata[i]);
               idata[i] = NULL;
               got_it = true;
               }
         if( !got_it)
            printf( "%s", buff);
         }
   fclose( ifile);
   for( i = 0; i < n_asteroids; i++)
      if( idata[i])
         {
         printf( "%s", idata[i]);
         free( idata[i]);
         }
   return( 0);
}
