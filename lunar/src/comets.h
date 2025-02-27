/* comets.h: header file for comet/asteroid functions
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
#ifndef COMETS_H_NCLUDE
#define COMETS_H_NCLUDE



#pragma pack(4)
struct Elements
{
   double perih_time, q, ecc, incl, arg_per, asc_node;
   double epoch,  mean_anomaly;
            /* derived quantities: */
   double lon_per, minor_to_major;
   double perih_vec[3], sideways[3];
   double angular_momentum, major_axis, t0, w0;
   double abs_mag, slope_param, gm;
   int is_asteroid, central_obj;
   };
#pragma pack( )

/* Note that in the above structure,  t0 = 1/n = time to move */
/* one radian in mean anomaly = orbital period / (2*pi).      */


extern "C" {

#include <cstdint>

// void calc_vectors( ELEMENTS *elem, const double sqrt_gm);
int /*DLL_FUNC*/ calc_classical_elements(Elements *elem, const double *r,
                             const double t, const int ref);
int /*DLL_FUNC*/ comet_posn_and_vel(Elements *elem, double t,
                  double   *loc, double   *vel);
int /*DLL_FUNC*/ comet_posn(Elements *elem, double t, double   *loc);       /* astfuncs.c */
void /*DLL_FUNC*/ derive_quantities(Elements *e, const double gm);
int /*DLL_FUNC*/ setup_elems_from_ast_file(Elements *class_elem,
              const uint32_t   *elem, const double t_epoch);
double /*DLL_FUNC*/ phase_angle_correction_to_magnitude( const double phase_angle,
                                 const double slope_param);

int extract_sof_data(Elements *elem, const char *buff, const char *header);
int extract_sof_data_ex(Elements *elem, const char *buff, const char *header,
                        double *extra_info);                /* sof.cpp */
double extract_yyyymmdd_to_jd( const char *buff);           /* sof.cpp */

typedef struct
{
   double obj1_true_anom, jd1;       /* these are set in find_moid_full */
   double obj2_true_anom, jd2;       /* NOT SET YET */
   double barbee_speed;              /* in AU/day */
} moid_data_t;

double /*DLL_FUNC*/ find_moid_full( const Elements *elem1, const Elements *elem2, moid_data_t *mdata);

}

#endif // !COMETS_H_NCLUDE

