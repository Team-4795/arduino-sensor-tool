
#ifndef _TINYSTATS_H_
#define _TINYSTATS_H_

#include <stdint.h>

class TinyStats {
	int32_t n;
	int32_t total;
public:
	int16_t min;
	int16_t max;

	void clear();
	void item(int16_t a);
	int16_t mean();
};

#endif
