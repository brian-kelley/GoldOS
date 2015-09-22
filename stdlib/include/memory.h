#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "globalDefines.h"

void initMM(); //initialization
void* mmAlloc(size_t size);
void mmFree(void* mem);

#endif