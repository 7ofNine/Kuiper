#ifndef ELEM_OUT_H_INCLUDE
#define ELEM_OUT_H_INCLUDE


#include "ephem0.h"  //for ephem_option_t which is just an uint64


#include <iosfwd>

struct Elements;
struct Observe;

char* fgets_trimmed(char* buff, size_t max_bytes, FILE* ifile);
char* iso_time(char* buff, const double jd, const int precision);
const char* get_find_orb_text(const int index);
int store_defaults(const ephem_option_t ephemeris_output_options, const int element_format,
    const int element_precision, const double max_residual_for_filtering, const double noise_in_sigmas); 
int get_defaults(ephem_option_t* ephemeris_output_options, int* element_format,
    int* element_precision, double* max_residual_for_filtering, double* noise_in_sigmas);
int find_worst_observation(const Observe* obs, const int n_obs);
double calc_obs_magnitude(const double obj_sun,
    const double obj_earth, const double earth_sun, double* phase_ang);
int get_orbit_from_mpcorb_sof(const char* object_name, double* orbit,
    Elements* elems, const double full_arc_len, double* max_resid);
int get_idx1_and_idx2(const int n_obs, const Observe* obs, int* idx1, int* idx2);
double mag_band_shift(const char mag_band, int* err_code);   
double centralize_ang(double ang);         
double find_epoch_shown(const Observe* obs, const int n_obs);
int set_language(const int language);    
void get_find_orb_text_filename(char* filename); 
int compute_canned_object_state_vect(double* loc, const char* mpc_code, const double jd);
int save_ephemeris_settings(const ephem_option_t ephemeris_output_options,
    const int n_steps, const char* obscode, const char* step_size,
    const char* ephem_start, const char* config); 
int load_ephemeris_settings(ephem_option_t* ephemeris_output_options,
    int* n_steps, char* obscode, char* step_size, char* ephem_start,
    const char* config);  
void rotate_state_vector_to_current_frame(double* state_vect,
    const double epoch_shown, const int planet_orbiting,
    char* body_frame_note);
void set_statistical_ranging(const int new_using_sr); 
void make_observatory_info_text(char* text, const size_t textlen,
    const Observe* obs, int n_obs, const char* mpc_code);









//static int elements_in_mpcorb_format(char* buff, const char* packed_desig,const char* full_desig, const Elements *elem,
//           const Observe* obs, const int n_obs);   /* orb_func.c */
//static int elements_in_guide_format(char* buff, const Elements* elem,
//    const char* obj_name, const Observe* obs, const unsigned n_obs);
//static int names_compare(const char* name1, const char* name2);
//static int get_uncertainty(const char* key, char* obuff, const bool in_km);




#endif
