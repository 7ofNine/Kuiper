#ifndef ELEM_OUT_H_INCLUDE
#define ELEM_OUT_H_INCLUDE

#include <iosfwd>

#include <ephem0.h>  //for ephem_option_t which is just an uint64

char* fgets_trimmed(char* buff, size_t max_bytes, FILE* ifile);
char* iso_time(char* buff, const double jd, const int precision);
const char* get_find_orb_text(const int index);
int store_defaults(const ephem_option_t ephemeris_output_options, const int element_format,
    const int element_precision, const double max_residual_for_filtering, const double noise_in_sigmas);           


#endif
