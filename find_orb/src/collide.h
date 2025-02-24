#ifndef COLLIDE_H_INCLUDE
#define COLLIDE_H_INCLUDE

double find_lat_lon_alt(const double ut, const double* ivect,
    const int planet_no, double* lat_lon, const bool geometric);

#endif // !COLLIDE_H_INCLUDE
