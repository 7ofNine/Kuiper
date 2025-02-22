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

/* 'nanoseconds_since_1970( )' returns something close to the result of
ctime( ),  except a billion times larger and with added precision.
Naturally,  DOS/Windows,  OS/X,  and POSIX systems each use different
methods.

   As currently coded,  the actual precision provided is 10^-7 second
in Windows;  a millisecond with the WATCOM compiler;  and 10^-6 second
in everything else.  But note that "true" nanosecond precision is
possible,  if actually desired (see the NOT_CURRENTLY_IN_USE code).

   The range of a 64-bit signed integer is large enough to enable
this function to work until Friday, 2262 Apr 11 23:47:16.854775808.
We can get an addition 292 years by using unsigned integers,  but
it may be wiser to switch to 128-bit integers.

   Note that the usual limitations apply:  no leap seconds,  and if
the computer's time is adjusted by NTP or the user,  the result may
actually go backward.  If you want to know what time it is,  don't
ask a computer.  */

#include <windows.h>
#include <cstdint>

#include "afuncs.h"

int64_t /*DLL_FUNC*/ nanoseconds_since_1970( void);                      /* afuncs.c */


int64_t /*DLL_FUNC*/ nanoseconds_since_1970( void)
{
   FILETIME ft;
   const uint64_t jd_1601 = 2305813;  /* actually 2305813.5 */
   const uint64_t jd_1970 = 2440587;  /* actually 2440587.5 */
   const uint64_t ten_million = 10000000;
   const uint64_t diff = (jd_1970 - jd_1601) * ten_million * (uint64_t)seconds_per_day;
   uint64_t decimicroseconds_since_1970;   /* i.e.,  time in units of 1e-7 seconds */

   GetSystemTimeAsFileTime( &ft);
   decimicroseconds_since_1970 = ((uint64_t)ft.dwLowDateTime |
                                ((uint64_t)ft.dwHighDateTime << 32)) - diff;
   return( decimicroseconds_since_1970 * (int64_t)100);
}

double /*DLL_FUNC*/ current_jd( void)
{
   static const double jan_1970 = 2440587.5;
   const double jd = jan_1970 +
                  (double)nanoseconds_since_1970( ) * 1e-9 / seconds_per_day;

   return( jd);
}
