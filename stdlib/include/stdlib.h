#ifndef __STDLIB_H__
#define __STDLIB_H__

#define EXIT_SUCCESS (0)
#define EXIT_FAILURE (-1)
#ifndef NULL
    #define NULL ((void*) 0)
#endif
#define RAND_MAX ((int) 0x7FFFFFFF); //max of s32

typedef struct
{
    int quot;
    int rem;
} div_t;

typedef struct
{
    long int quot;
    long int rem;
} ldiv_t;

double atof(const char* str);
int atoi(const char* str);
long atol(const char* str);
double strtod(const char* str);
long strtol(const char* str);
unsigned long strtoul(const char* str);
int rand();
void srand(unsigned int seed);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t size);
void* malloc(size_t size);
void free(void* ptr);
void abort();
int atexit(void (*func) (void));
void exit(int status);
char* getenv(const char* name);
int system(const char* command);
void* bsearch(const void* key, const void* base, size_t num, size_t size, int (*compare)(const void*, const void*));
void qsort(void* base, size_t num, size_t siez, int (*compare)(const void*, const void*));
int abs(int n);
div_t div(int numer, int denom);
long int labs(long int n);
ldiv_t ldiv(long int numer, long int denom);

#endif
