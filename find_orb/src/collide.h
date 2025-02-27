#ifndef COLLIDE_H_INCLUDE
#define COLLIDE_H_INCLUDE

struct Elements;

double find_lat_lon_alt(const double ut, const double* ivect,
    const int planet_no, double* lat_lon, const bool geometric);
double find_collision_time(Elements* elem, double* latlon, const int is_impact);


#endif // !COLLIDE_H_INCLUDE
