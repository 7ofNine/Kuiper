#ifndef MISCELL_H_INCLUDE
#define MISCELL_H_INCLUDE

#include <iosfwd>
#include <cstdint>

FILE* fopen_ext(const char* filename, const char* permits);
char* make_config_dir_name(char* oname, const char* iname); 
uint64_t parse_bit_string(const char* istr); 
const char* write_bit_string(char* ibuff, const uint64_t bits, const size_t max_bitstring_len);



#endif
