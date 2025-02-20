/* f_strtod.cpp: "fast" version of strtod( ).

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

#include <assert.h>
#include <stdint.h>
#include <stddef.h>     /* for NULL definition */
#include <ctype.h>
#include <float.h>

#ifdef __WATCOMC__
   #include <stdbool.h>
#endif

double fast_strtod( const char *iptr, char **endptr);
long double fast_strtold( const char *iptr, char **endptr);

         /* MSVC has problems converting unsigned longs past 2^63
         to doubles or long doubles.  So with MSVC,  we store the
         accumulated digits in an int64_t,  and sometimes can't
         store a digit.  On other compilers,  we can use a uint64_t.
         And older MSVCs don't define INT64_MAX.  Digital Mars
         appears to have similar issues.   */

#if defined( _MSC_VER) || defined( __DMC__)
   typedef int64_t stored_digits;
   #if defined( _MSC_VER)
      # define MAX_STORED_DIGITS     (9223372036854775807i64)
   #else       /* for DMC */
      # define MAX_STORED_DIGITS     (9223372036854775807LL)
   #endif
#else
   typedef uint64_t stored_digits;
   # define MAX_STORED_DIGITS     UINT64_MAX
#endif

#ifndef UINT64_MAX
#define UINT64_MAX ((uint64_t)-1)
#endif

#if defined( __clang__) && defined( __APPLE__)
#pragma GCC diagnostic ignored "-Wc++11-long-long"
#endif     /* suppress nuisance warning on OS/X */

inline long double ten_power( int exponent)
{
   static const long double multipliers[16] = { 1.L, 10.L, 100.L, 1000.L,
               10000.L, 100000.L, 1e+6L, 1e+7L, 1e+8L, 1e+9L, 1e+10L, 1e+11L,
               1e+12L, 1e+13L, 1e+14L, 1e+15L };
   long double rval = multipliers[exponent & 0xf];

   assert( exponent >= 0);
   exponent >>= 4;
   if( exponent)
      {
      int i;
      static const long double big_multipliers[] = { 1e+16L, 1e+32L, 1e+64L,
            1e+128L
#ifndef LDBL_MAX_10_EXP
   #error LDBL_MAX_10_EXP undefined!
#endif
#if( LDBL_MAX_10_EXP >= 256)
          , 1e+256L
#endif
#if( LDBL_MAX_10_EXP >= 512)
          , 1e+512L
#endif
#if( LDBL_MAX_10_EXP >= 1024)
          , 1e+1024L
#endif
#if( LDBL_MAX_10_EXP >= 2048)
          , 1e+2048L
#endif
#if( LDBL_MAX_10_EXP >= 4096)
          , 1e+4096L
#endif
         };

      exponent &= 0x1ff;
      for( i = 0; exponent; i++, exponent >>= 1)
         if( exponent & 1)
            rval *= big_multipliers[i];
      }
   return( rval);
}

#ifdef SLOWER_VERSION_OF_THE_ABOVE
/* This version is simpler and has smaller tables,  but takes just enough
longer to persuade me to use the above version instead.   */
inline long double ten_power( int exponent)
{
   long double rval = 1.L;
   int i;
   static const long double big_multipliers[] = { 10.L, 100.L, 1e+4L,
            1e+8L, 1e+16L, 1e+32L, 1e+64L,
            1e+128L, 1e+256L, 1e+512L, 1e+1024L, 1e+2048L, 1e+4096L };

   exponent &= 0x1fff;
   for( i = 0; exponent; i++, exponent >>= 1)
      if( exponent & 1)
         rval *= big_multipliers[i];
   return( rval);
}
#endif


    /* I've found that most of the time in asc2eph,  the program   */
    /* to convert ASCII JPL ephemerides to binary,  was consumed   */
    /* in parsing floating-point values from the ASCII data.  That */
    /* part was done using sscanf().  strtod() and atof() proved   */
    /* faster,  but still quite slow.  The following was written   */
    /* with speed in mind;  as a result,  it's doubtless bigger    */
    /* and more complicated than your average library strtod().    */
    /* But it speeds asc2eph by two or three times,  depending on  */
    /* platform and compiler.                                      */
    /*    Also,  note that while it's been well-tested with the    */
    /* asc2eph program,  and somewhat tested with the test code in */
    /* 'ftest.cpp' (q.v.),  it's still possible that odd cases may */
    /* remain.  'ftest.cpp' simplifies/semi-automates comparison   */
    /* results from the library strtod() and atof() functions.     */
    /*    2014 May 15:  streamlined fast_strtold()'s logic.        */

/* A few 'gotchas' that are now handled (or not) in this code :

   -- A string such as 3.141592e-4945 is well within the range of an
80-bit long double.  However,  it'll get read in as 3141592 multiplied
by 10^-4951,  and that last is outside the range of long doubles.
Which is why there's a bit of code starting with 'if( exponent >=
OVER_THE_TOP_EXPONENT)'. That will make the values to be multiplied
3.141592e-4090 and 10^-855, both within the range of long doubles.

   -- Microsoft doesn't "get" 80-bit doubles;  to them,  long doubles
and doubles are the same thing.  (Insert anti-MS rant here;  I doubt
I can use any profanity for them that hasn't already been done elsewhere.)
When compiled on an MS compiler,  LDBL_MAX_10_EXP will be 308;  on
everything else I've seen,  it'll be 4932.  Suitable #ifs and #ifdefs
have been added to handle this... at least for those two cases,  which
are the only two I can test.  It does lay the groundwork for machines
with 128-bit floats or those limited to 32-bit floats.

   -- In assembling the digits into an integer value,  we can handle
integers up to INT64_MAX / 10 - 1 without overflowing.  Beyond that,
we start putting the digits into 'part_two',  and keep track of how
many digits are so stored.  (We have to do that because an 80-bit long
double has 64 bits of mantissa,  and 'max_rval' only gives us about 61
bits.  But between 'rval' and 'part_two',  we actually have enough bits
to handle a 128-bit quadruple-precision float,  if that ever proves
desirable.)

   Anyway.  If 'rval' and 'part_two' both run out of digits (become
greater than 'max_rval'),  we'll have some digits that are just plain
skipped over (won't contribute to the value of a mere 80-bit float),
but we still have to keep track of them.

   NOTE that this implements the C90 (C++98) concept of strtod.  For
C99/C11/C++11,  we'd need to also handle hexadecimal floats,  INF,
INFINITY,  and the various flavors of NANs.  'errno' isn't set to
ERANGE,  and underflow may not be handled correctly.  I've not felt
the need to fix these yet.

   -- Comparison to the glib strtold() function (see 'ftest.cpp')
shows that roundoff can leave us one off in the last binary digit. (So
far,  I've not seen errors greater than one digit.)  Fixing this would
be _very_ challenging.  In "borderline" cases,  one may have to read
over 700 decimal digits before being able to decide which way roundoff
should go in the binary conversion.  I think fast_strtod is correct
(to the last bit) for almost all "real-world" input.  (The failures
occur when there are more than about 17 digits,  and in some cases
involving high exponents.)  I'd like to define that a little better;
ideally,  we might be able to have the function realize when it might
fail,  then turn those very unusual cases over to "standard" strtold.

   I think this code gets a truly correct result in all cases :

http://www.netlib.org/fp/dtoa.c

*/

#if( LDBL_MAX_10_EXP >= 4096)
   #define OVER_THE_TOP_EXPONENT 4096
   #define OVER_THE_TOP_VALUE 1e+4096L
#else
   #define OVER_THE_TOP_EXPONENT 256
   #define OVER_THE_TOP_VALUE 1e+256L
#endif

long double fast_strtold( const char *iptr, char **endptr)
{
   int n_skipped_digits = 0;
   const stored_digits max_rval = MAX_STORED_DIGITS / (stored_digits)10 - 1;
   stored_digits rval = 0, part_two = 0;
   const char *decimal_point = NULL;
   int part_two_digits = 0, downshift = 0;
   int exponent = 0, exponent2, pass;
   bool exponent_is_negative = false;
   bool is_negative = false;
   bool digits_found = false;
   long double d_rval, ten_pow;

   if( endptr)
      *endptr = (char *)iptr;
   while( isspace( *iptr))
      iptr++;
   if( *iptr == '+')
      iptr++;
   else if( *iptr == '-')
      {
      iptr++;
      is_negative = true;
      }
#ifdef C99_C11_COMPLIANCE        /* not actually implemented yet! */
   if( *iptr == '0' && (iptr[1] == 'x' || iptr[1] == 'X'))
      {
      d_rval = get_hex_strtold( iptr, endptr);
      return( is_negative ? -d_rval : d_rval);
      }
    /* ...and check for INF, INFINITY,  NAN,  NANsequence */
#endif
               /* pass = 0 -> digits before decimal point         */
               /* pass = 1 -> digits after decimal point (if any) */
   for( pass = 0; pass < 2; pass++)
      {
      while( *iptr >= '0' && *iptr <= '9')
         {
         digits_found = true;
         if( rval < max_rval)
            rval = rval * 10 + (stored_digits)( *iptr - '0');
         else
            {
            if( part_two < max_rval)
               {
               part_two = part_two * 10 + (stored_digits)( *iptr - '0');
               part_two_digits++;
               }
            else
               n_skipped_digits++;
            }
         iptr++;
         }
      if( !pass && *iptr == '.')
         decimal_point = ++iptr;
      else
         break;
      }
   if( !digits_found)      /* must be at least one digit */
      return( 0.);
   if( decimal_point)
      downshift = (int)( iptr - decimal_point);
   if( *iptr == 'e' || *iptr == 'E')
      {
      const char *iptr2 = iptr;

      iptr2++;
      if( *iptr2 == '+')
         iptr2++;
      else if( *iptr2 == '-')
         {
         exponent_is_negative = true;
         iptr2++;
         }
      while( *iptr2 >= '0' && *iptr2 <= '9')
         {
         exponent = exponent * 10 + (int)( *iptr2++ - '0');
         iptr = iptr2;
         }
      if( exponent_is_negative)
         exponent = -exponent;
      }
   exponent += part_two_digits + n_skipped_digits - downshift;
   exponent2 = exponent - part_two_digits;
   d_rval = (long double)rval;
   if( exponent < 0)
      {
      exponent = -exponent;
      exponent_is_negative = true;
      }
   else
      exponent_is_negative = false;
   if( exponent >= OVER_THE_TOP_EXPONENT)
      {
      if( exponent_is_negative)
         d_rval /= OVER_THE_TOP_VALUE;
      else
         d_rval *= OVER_THE_TOP_VALUE;
      exponent -= OVER_THE_TOP_EXPONENT;
      }
   ten_pow = ten_power( exponent);
   if( exponent_is_negative)
      d_rval /= ten_pow;
   else
      d_rval *= ten_pow;
   if( part_two_digits)
      {
      long double tval = (long double)part_two;

      if( exponent2 < 0)
          tval /= ten_power( -exponent2);
      else
          tval *= ten_power( exponent2);
      d_rval += tval;
      }
   if( endptr)
      *endptr = (char *)iptr;
   return( is_negative ? -d_rval : d_rval);
}

/* I'm assuming that one can handle fast_strtod() simply by using
fast_strtold().  This should certainly handle things nicely,  _except_
if we're on a platform where long doubles are slower than "ordinary"
doubles.  In that case,  it may be worth revising the above code.

   Note that if you do so,  you can probably omit the pieces about
'part_two'.  The 64-bit integer 'rval' will hold more than enough
precision for the mere 52 mantissa bits of a 64-bit float.  */

double fast_strtod( const char *iptr, char **endptr)
{
   return( (double)fast_strtold( iptr, endptr));
}
