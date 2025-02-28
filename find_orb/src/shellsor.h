#ifndef SHELLSOR_H_INCLUDE
#define SHELLSOR_H_INCLUDE

void* bsearch_ext_r(const void* key, const void* base0, size_t nmemb,
    const size_t size, int (*compar)(const void*, const void*, void*),
    void* arg, bool* found);      
void* bsearch_ext(const void* key, const void* base0, size_t nmemb, const size_t size,
    int (*compar)(const void*, const void*), bool* found);



#endif // !SHELLSOR_H_INCLUDE
