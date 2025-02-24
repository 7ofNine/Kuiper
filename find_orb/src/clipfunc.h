#ifndef CLIPFUNC_H_INCLUDE
#define CLIPFUNC_H_INCLUDE

int copy_buffer_to_clipboard(const char* contents, const long length);
int copy_file_to_clipboard(const char* filename); 
int clipboard_to_file(const char* filename, const int append, const bool use_selection);

#endif
