// ephem0.h: header files for ??
//Copyright(C) 2025, Project Kuiper
//
//This program is free software; you can redistribute it and /or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//02110 - 1301, USA.* /

#ifndef EPHEM0_H_INCLUDED
#define EPHEM0_H_INCLUDED

char* get_file_name(char* filename, const char* template_file_name);
double utc_from_td(const double jdt, double* delta_t);
double original_observed_ra(const OBSERVE* obs);
double original_observed_dec(const OBSERVE* obs);

#endif
