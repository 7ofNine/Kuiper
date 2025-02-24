#ifndef HEALPIX_H_INCLUDE
#define HEALPIX_H_INCLUDE

void ra_dec_to_xy(const double ra, const double dec, double* x, double* y);
unsigned xy_to_healpix(const double x, const double y, const unsigned N);

#endif // !HEALPIX_H_INCLUDE

