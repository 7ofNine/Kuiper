#ifndef BIAS_H_INCLUDED
#define BIAS_H_INCLUDED

int find_fcct_biases(const double ra, const double dec, const char catalog,
    const double jd, double* bias_ra, double* bias_dec);

#endif // !BIAS_H_INCLUDED

