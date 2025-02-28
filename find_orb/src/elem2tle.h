#ifndef ELEM2TLE_H_INCLUDE
#define ELEM2TLE_H_INCLUDE

struct elements;
struct tle_t;

int elements_to_tle(tle_t* tle, const elements* elem);
int write_tle_from_vector(char* buff, const double* state_vect,
    const double epoch, const char* norad_desig, const char* intl_desig);
int vector_to_tle(tle_t* tle, const double* state_vect, const double epoch);



#endif // !ELEM2TLE_H_INCLUDE

