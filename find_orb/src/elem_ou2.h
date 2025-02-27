#ifndef ELEM_OU2_H_INCLUDE
#define ELEM_OU2_H_INCLUDE

struct Observe;
struct elements;

int put_elements_into_sof(char* obuff, const char* templat,
    const Elements* elem, const double* nongravs,
    const int n_obs, const Observe *obs);


#endif // !ELEM_OU2_H_INCLUDE

