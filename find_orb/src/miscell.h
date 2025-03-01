#ifndef MISCELL_H_INCLUDE
#define MISCELL_H_INCLUDE

#include <iosfwd>
#include <cstdint>

FILE* fopen_ext(const char* filename, const char* permits);
char* make_config_dir_name(char* oname, const char* iname); 
uint64_t parse_bit_string(const char* istr); 
const char* write_bit_string(char* ibuff, const uint64_t bits, const size_t max_bitstring_len);
void ensure_config_directory_exists(); 
int fetch_astrometry_from_mpc(FILE* ofile, const char* desig);
int reset_astrometry_filename(int* argc, const char** argv);
int download_a_file(const char* ofilename, const char* url);
int pattern_match(const char* pattern, const char* string);   /* miscell.c */




#endif
