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

#include <iosfwd>
#include <cstdint>

//this is from mpc_obs.h but it doesn't make sense there
using ephem_option_t = uint64_t;  // bad use of using just a longer name

/* Bitfield options for ephemeris_in_a_file( ): */
/* Bottom three bits define an ephemeris type.  "Observables" are the */
/* usual RA/dec,  radial velocity,  etc. type output.  "State vector  */
/* output" results in the time (as a JD) and position (in AU,  relative */
/* to the observer,  in Cartesian coordinates) being output.  "Position */
/* output" is the same thing,  minus the velocity components.  "MPCORB  */
/* output" means that the orbital elements will be written out for each */
/* ephemeris step,  on a single line.  "8-line output" is almost the    */
/* same,  except that the elements are written out in the MPC's usual   */
/* eight-line form.  "Close approaches" will result in the range minima */
/* (times and distances) being output.                                  */

#define OPTION_OBSERVABLES             0
#define OPTION_STATE_VECTOR_OUTPUT     1
#define OPTION_POSITION_OUTPUT         2
#define OPTION_MPCORB_OUTPUT           3
#define OPTION_8_LINE_OUTPUT           4
#define OPTION_CLOSE_APPROACHES        5
#define OPTION_FAKE_ASTROMETRY         6

#define EPHEM_OPTION_BIT( N)  (((ephem_option_t)1) << (N))

#define OPTION_ALT_AZ_OUTPUT            EPHEM_OPTION_BIT( 3)
#define OPTION_RADIAL_VEL_OUTPUT        EPHEM_OPTION_BIT( 4)
#define OPTION_MOTION_OUTPUT            EPHEM_OPTION_BIT( 5)
#define OPTION_PHASE_ANGLE_OUTPUT       EPHEM_OPTION_BIT( 6)
#define OPTION_GROUND_TRACK             EPHEM_OPTION_BIT( 8)
#define OPTION_SEPARATE_MOTIONS         EPHEM_OPTION_BIT( 9)

#define OPTION_ROUND_TO_NEAREST_STEP    EPHEM_OPTION_BIT( 10)
#define OPTION_PHASE_ANGLE_BISECTOR     EPHEM_OPTION_BIT( 11)
#define OPTION_HELIO_ECLIPTIC           EPHEM_OPTION_BIT( 12)
#define OPTION_TOPO_ECLIPTIC            EPHEM_OPTION_BIT( 13)

#define OPTION_VISIBILITY               EPHEM_OPTION_BIT( 14)
#define OPTION_SUPPRESS_UNOBSERVABLE    EPHEM_OPTION_BIT( 15)
#define OPTION_SHOW_SIGMAS              EPHEM_OPTION_BIT( 16)
#define OPTION_COMPUTER_FRIENDLY        EPHEM_OPTION_BIT( 17)
      /* Above option means 'ephems are written in format easy for  */
      /* software to read,  instead of in a human-readable format'. */

      /* Added 2015 May 4 at suggestion of Denis Denisenko          */
#define OPTION_MOIDS                    EPHEM_OPTION_BIT( 18)
#define OPTION_SPACE_VEL_OUTPUT         EPHEM_OPTION_BIT( 19)
#define OPTION_LUNAR_ELONGATION         EPHEM_OPTION_BIT( 20)

#define OPTION_SUPPRESS_RA_DEC          EPHEM_OPTION_BIT( 21)
#define OPTION_SUPPRESS_DELTA           EPHEM_OPTION_BIT( 22)
#define OPTION_SUPPRESS_SOLAR_R         EPHEM_OPTION_BIT( 23)
#define OPTION_SUPPRESS_ELONG           EPHEM_OPTION_BIT( 24)

#define OPTION_SUN_ALT                  EPHEM_OPTION_BIT( 25)
#define OPTION_SUN_AZ                   EPHEM_OPTION_BIT( 26)
#define OPTION_MOON_ALT                 EPHEM_OPTION_BIT( 27)
#define OPTION_MOON_AZ                  EPHEM_OPTION_BIT( 28)
#define OPTION_SKY_BRIGHTNESS           EPHEM_OPTION_BIT( 29)


#define OPTION_SUN_TARGET_PA            EPHEM_OPTION_BIT( 30)
#define OPTION_SUN_HELIO_VEL_PA         EPHEM_OPTION_BIT( 31)
#define OPTION_ORBIT_PLANE_ANGLE        EPHEM_OPTION_BIT( 32)
#define OPTION_GALACTIC_COORDS          EPHEM_OPTION_BIT( 33)
#define OPTION_GALACTIC_CONFUSION       EPHEM_OPTION_BIT( 34)
#define OPTION_SNR                      EPHEM_OPTION_BIT( 35)
#define OPTION_EXPOSURE_TIME            EPHEM_OPTION_BIT( 36)
#define OPTION_EXPLANATIONS             EPHEM_OPTION_BIT( 37)
#define OPTION_CONSTELLATION            EPHEM_OPTION_BIT( 38)
#define OPTION_RV_AND_DELTA_SIGMAS      EPHEM_OPTION_BIT( 39)
// end mpc_obs.h

struct Observe;

char* get_file_name(char* filename, const char* template_file_name);
double utc_from_td(const double jdt, double *delta_t);
double original_observed_ra(const Observe *obs);
double original_observed_dec(const Observe *obs);
int parallax_to_lat_alt(const double rho_cos_phi, const double rho_sin_phi,
       double* lat, double* ht_in_meters, const int planet_idx);
int ephemeris_in_a_file_from_mpc_code(const char* filename, const double* orbit, Observe *obs, const int n_obs,
    const double epoch_jd, const double jd_start, const char* stepsize, const int n_steps, const char* mpc_code,
    ephem_option_t options, const unsigned n_objects);
FILE* open_json_file(char* filename, const char* env_ptr, const char* default_name,
    const char* packed_desig, const char* permits);
void remove_trailing_cr_lf(char* buff); 
double diameter_from_abs_mag(const double abs_mag, const double optical_albedo);
char* real_packed_desig(char* obuff, const char* packed_id); 
double vector_to_polar(double* lon, double* lat, const double* vector);
int lat_alt_to_parallax(const double lat, const double ht_in_meters,   // this name also exists in lunar with a slightly different parameters
    double* rho_cos_phi, double* rho_sin_phi, const int planet_idx);
int write_residuals_to_file(const char* filename, const char* ast_filename,
    const int n_obs, const Observe* obs_data, const int format);
int make_pseudo_mpec(const char* mpec_filename, const char* obj_name);
void format_dist_in_buff(char* buff, const double dist_in_au);
double shadow_check(const double* planet_loc, const double* obs_posn, const double planet_radius_in_au);
int add_ephemeris_details(FILE* ofile, const double start_jd, const double end_jd);
bool is_topocentric_mpc_code(const char* mpc_code);
void size_from_h_text(const double abs_mag, char* obuff, const int obuff_size);






//static void put_residual_into_text(char* text, const double resid, const int resid_format);

#endif
