#ifndef RUNGE_H_INCLUDE
#define RUNGE_H_INCLUDE

int find_best_fit_planet(const double jd, const double* ivect, double* rel_vect);
void find_relative_state_vect(const double jd, const double* ivect,
       double* ovect, const int ref_planet);   
void compute_effective_solar_multiplier(const char* constraints); 



#endif // !RUNGE_H_INCLUDE
