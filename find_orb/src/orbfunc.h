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


double initial_orbit(Observe* obs, int n_obs, double* orbit);
int set_locs(const double* orbit, double t0, Observe* obs, int n_obs);
void attempt_extensions(Observe* obs, const int n_obs, double* orbit,
    const double epoch);      
void get_relative_vector(const double jd, const double* ivect,
    double* relative_vect, const int planet_orbiting);  
double get_planet_mass(const int planet_idx); 
double observation_rms(const Observe* obs);      


#endif // !ORBFUNC_H_INCLUDE

