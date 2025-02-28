#ifndef GAUSS_H_INCLUDE
#define GAUSS_H_INCLUDE

struct Observe;

double gauss_method(const Observe* obs1, const Observe* obs2,
    const Observe* obs3, double* orbit, const double mu,
    const int desired_soln);              

double convenient_gauss(const Observe* obs, int n_obs, double* orbit,
    const double mu, const int desired_soln); 

#endif // !GAUSS_H_INCLUDE

