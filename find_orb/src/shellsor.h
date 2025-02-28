#ifndef SHELLSOR_H_INCLUDE
#define SHELLSOR_H_INCLUDE

void* bsearch_ext_r(const void* key, const void* base0, size_t nmemb,
    const size_t size, int (*compar)(const void*, const void*, void*),
    void* arg, bool* found);      

void* bsearch_ext(const void* key, const void* base0, size_t nmemb, const size_t size,
    int (*compar)(const void*, const void*), bool* found);

void shellsort_r(void* base, const size_t n_elements, const size_t elem_size,
    int (*compare)(const void*, const void*, void*), void* context);




#endif // !SHELLSOR_H_INCLUDE
