#ifndef ELEM2TLE_H_INCLUDE
#define ELEM2TLE_H_INCLUDE

struct elements;
struct tle_t;

int elements_to_tle(tle_t* tle, const elements* elem);
#endif // !ELEM2TLE_H_INCLUDE

