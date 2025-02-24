#ifndef ELEM_OU2_H_INCLUDE
#define ELEM_OU2_H_INCLUDE

struct observe;
struct elements;

int put_elements_into_sof(char* obuff, const char* templat,
    const ELEMENTS* elem, const double* nongravs,
    const int n_obs, const OBSERVE* obs);                


#endif // !ELEM_OU2_H_INCLUDE

