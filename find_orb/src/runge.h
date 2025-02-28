#ifndef RUNGE_H_INCLUDE
#define RUNGE_H_INCLUDE

int find_best_fit_planet(const double jd, const double* ivect, double* rel_vect);
void find_relative_state_vect(const double jd, const double* ivect,
       double* ovect, const int ref_planet);   
void compute_effective_solar_multiplier(const char* constraints); 
int get_planet_posn_vel(const double jd, const int planet_no, double* posn, double* vel);
int calc_derivatives(const double jd, const double* ival, double* oval, const int reference_planet);
void calc_approx_planet_orientation(const int planet, const int system_number, const double jde, double* matrix);






#endif // !RUNGE_H_INCLUDE
