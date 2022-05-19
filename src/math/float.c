#include "rtmath.h"

int
	float_eq(FLOAT a, FLOAT b, FLOAT error)
{
	return (a < b + error && a > b - error);
}
