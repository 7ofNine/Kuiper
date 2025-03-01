#ifndef ORBFUNC2_H_INCLUDE
#define ORBFUNC2_H_INCLUDE

struct Observe;

void push_orbit(const double epoch, const double* orbit);
int pop_orbit(double* epoch, double* orbit);      
double generate_mc_variant_from_covariance(double* var_orbit, const double* orbit);
int simplex_method(Observe* obs, int n_obs, double* orbit,
    const double r1, const double r2, const char* constraints);
int superplex_method(Observe* obs, int n_obs, double* orbit, const char* constraints);
int write_excluded_observations_file(const Observe* obs, int n_obs);
int link_arcs(Observe* obs, int n_obs, const double r1, const double r2);
int find_circular_orbits(Observe* obs1, Observe* obs2, double* orbit, const int desired_soln);   /* orb_fun2.cpp */
double improve_along_lov(double* orbit, const double epoch, const double* lov,
    const unsigned n_params, unsigned n_obs, Observe* obs);
int apply_excluded_observations_file(Observe* obs, const int n_obs);




#endif // !ORBFUNC2_H_INCLUDE

