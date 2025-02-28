#ifndef ORBFUNC2_H_INCLUDE
#define ORBFUNC2_H_INCLUDE

void push_orbit(const double epoch, const double* orbit);
int pop_orbit(double* epoch, double* orbit);      
double generate_mc_variant_from_covariance(double* var_orbit, const double* orbit);



#endif // !ORBFUNC2_H_INCLUDE

