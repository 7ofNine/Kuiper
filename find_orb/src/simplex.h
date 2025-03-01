#ifndef SIMPLEX_H_INCLUDE
#define SIMPLEX_H_INCLUDE

void init_simplex(double** vects, double* fvals,
    double (*f)(void* context, const double* vect),
    void* context, const int n);        /* simplex.c */

int simplex_step(double** vects, double* fvals,
    double (*f)(void* context, const double* vect),
    void* context, const int n);        /* simplex.c */

#endif
