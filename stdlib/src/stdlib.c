#include "stdlib.h"

void* malloc(size_t size)
{
	return mmAlloc(size);
}

void* calloc(size_t num, size_t size)
{
	byte* ptr = (byte*) mmAlloc(num * size);
	if(ptr != NULL)
	{
		for(size_t i = 0; i < num * size; i++)
		{
			ptr[i] = 0;
		}
	}
	return ptr;
}

//for now just to the trivial thing - free and reallocate
void* realloc(void* ptr, size_t newSize)
{
	free(ptr);
	return mmAlloc(newSize);
}

void free(void* ptr)
{
	mmFree(ptr);
}

double atof(const char* str)
{
    double rv = 1;
    //Find the first non-whitespace character
    char* start = str + strspn(str, " \n\t");
    if(*start == '-')
	rv *= -1;
    start++;
    long long int mantissa;
    long long int pot = 1;
    //find the last decimal digit before the exponent or end of number
    char* manEnd = start + strspn(start, "0123456789");
    manEnd--;
    char* digIt = manEnd; //iterator over the digit chars, backwards
    while(digIt <= start)
    {
        int digit = digIt - '0';
		mantissa += digit * pot;
		pot *= 10;
		digIt--;
    }
    manEnd++;
    if(tolower(*manEnd) == 'e')
    {
		
    }
}
