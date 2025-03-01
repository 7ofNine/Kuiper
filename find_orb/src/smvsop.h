#ifndef SMVSOP_H_INCLUDE
#define SMVSOP_H_INCLUDE

int check_for_perturbers(const double t_cen, const double* vect); /* sm_vsop*/
int compute_rough_planet_loc(const double t_cen, const int planet_idx,
    double* vect);    /* sm_vsop.c */


#endif // !SMVSOP_H_INCLUDE

