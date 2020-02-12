
#include "tinystats.h"

void
TinyStats::clear()
{
	min = INT16_MAX;
	max = INT16_MIN;
	n = 0;
	total = 0;
}

void
TinyStats::item(int16_t a)
{
	if(a < min)
		min = a;
	if(a > max)
 		max = a;

	// what to do about overflow?
	n++;
	total += a; 
}

int16_t
TinyStats::mean()
{
	return total/n;
}
