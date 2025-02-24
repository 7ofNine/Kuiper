#ifndef ELEM_OUT_H_INCLUDE
#define ELEM_OUT_H_INCLUDE

#include <cstdio>

char* fgets_trimmed(char* buff, size_t max_bytes, FILE* ifile);
char* iso_time(char* buff, const double jd, const int precision);

#endif
