#ifndef GEPOT_H_INCLUDE
#define GEPOT_H_INCLUDE

double geo_potential_in_au(const double x, const double y, const double z,
    double* derivs, const int n_terms);    /* geo_pot.c */

#endif
