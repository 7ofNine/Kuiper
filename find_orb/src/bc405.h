#ifndef BC405_H_INCLUDE
#define BC405_H_INCLUDE

int detect_perturbers(const double jd, const double* /*__restrict*/ xyz, double* accel);
double* get_asteroid_mass(const int astnum);
int asteroid_position_raw(const int astnum, const double jd, double* posn, double* vel);

#endif