#ifndef ORBFUNC_H_INCLUDE
#define ORBFUNC_H_INCLUDE

struct Observe
{
    double jd;
    double obs_posn[3];
    double obs_vel[3];
    double vect[3];
    double ra;
    double dec;
    double obs_mag;
    double r;
    double obj_posn[3];
    double obj_vel[3];
    double solar_r;
    double computed_ra;
    double computed_dec;
    double posn_sigma_1;         /* in arcseconds */ 
    double posn_sigma_2;         /* in arcseconds */
    /* Usually, posn_sigma_1 = RA sigma, posn_sigma_2 = dec sigma. */
    double posn_sigma_theta;    /* tilt angle of uncertainty ellipse */
    double mag_sigma;
    double time_sigma;         /* in days */
    double unc_time;           /* uncTime (semi-systematic timing err) in days */
    double computed_mag;
    double ra_bias;            /* in arcseconds */
    double dec_bias;           /* in arcseconds */
    char* second_line;
    int flags;
    int is_included;
    int ref_center;       /* 399 = geocenter, 10 = heliocenter, 0 = SSB... */
    int time_precision;
    int ra_precision;
    int dec_precision;
    int mag_precision;
    char mpc_code[4];
    char packed_id[13];
    char reference[6];
    char columns_57_to_65[10];
    char mag_band;
    char astrometric_net_code;
    char discovery_asterisk;
    char note1;
    char note2;
    char satellite_obs;
    const char** obs_details;
    char* ades_ids;
};

struct sr_orbit_t;
struct Motion_details;

double initial_orbit(Observe* obs, int n_obs, double* orbit);
int set_locs(const double* orbit, double t0, Observe* obs, int n_obs);
void attempt_extensions(Observe* obs, const int n_obs, double* orbit,
    const double epoch);      
void get_relative_vector(const double jd, const double* ivect,
    double* relative_vect, const int planet_orbiting);  
double get_planet_mass(const int planet_idx); 
double observation_rms(const Observe* obs);     
double evaluate_initial_orbit(const Observe* obs,      
    const int n_obs, const double* orbit, const double epoch);
void compute_variant_orbit(double* variant, const double* ref_orbit, const double n_sigmas); 
double vect_diff2(const double* a, const double* b);
int get_residual_data(const Observe *obs, double *xresid, double *yresid);
void light_time_lag(const double jde, const double* orbit,     
    const double* observer, double* result, const int is_heliocentric);
int find_trial_orbit(double* orbit, Observe* obs, int n_obs,
    const double r1, const double angle_param); 
int search_for_trial_orbit(double* orbit, Observe* obs, int n_obs,
    const double r1, double* angle_param); 
void set_distance(Observe* obs, double r);  
double euler_function(const Observe* obs1, const Observe* obs2);
int find_parabolic_orbit(Observe* obs, const int n_obs,
    double* orbit, const int direction);
double improve_along_lov(double* orbit, const double epoch, const double* lov,
    const unsigned n_params, unsigned n_obs, Observe* obs);
int metropolis_search(Observe* obs, const int n_obs, double* orbit,
    const double epoch, int n_iterations, double scale);
int find_vaisala_orbit(double* orbit, const Observe* obs1,   /* orb_func.c */
    const Observe* obs2, const double solar_r);
int extended_orbit_fit(double* orbit, Observe* obs, int n_obs,
    const unsigned fit_type, double epoch);     /* orb_func.c */
int orbital_monte_carlo(const double* orbit, Observe* obs, const int n_obs,
    const double curr_epoch, const double epoch_shown);   /* orb_func.cpp */
int select_tracklet(Observe* obs, const int n_obs, const int idx);

int improve_sr_orbits(sr_orbit_t* orbits, Observe* obs,
    const unsigned n_obs, const unsigned n_orbits,
    const double noise_in_sigmas, const int writing_sr_elems);
bool is_sungrazing_comet(const Observe* obs, const int n_obs);  /* orb_func.c */

void compute_error_ellipse_adjusted_for_motion(double* sigma1, double* sigma2,
    double* posn_angle, const Observe* obs,
    const Motion_details* m);                  /* orb_func.cpp */
double n_nearby_obs(const Observe* obs, const unsigned n_obs,
    const unsigned idx, const double time_span);       /* orb_func.cpp */
int adjust_herget_results(Observe* obs, int n_obs, double* orbit);
double evaluate_for_simplex_method(const Observe* obs,
    const int n_obs, const double* orbit,
    const int planet_orbiting,
    const char* limited_orbit);     /* orb_func.cpp */
double find_r_given_solar_r(const Observe* obs, const double solar_r);
int compute_available_sigmas_hash(const Observe* obs, const int n_obs,
    const double epoch, const unsigned perturbers, const int central_obj);
double vector3_dist(const double* a, const double* b);     /* orb_func.c */
double compute_weighted_rms(const Observe* obs, const int n_obs,
    int* n_resids);                  /* orb_func.cpp */
double find_parabolic_minimum_point(const double x[3], const double y[3]);









#endif // !ORBFUNC_H_INCLUDE

