#include "rtmath.h"

#include <math.h>

int
	quadratic_solve(const t_quadratic *quadratic, FLOAT solutions[2])
{
	FLOAT	a;
	FLOAT	b;
	FLOAT	c;
	FLOAT	discriminant;

	a = quadratic->a;
	b = quadratic->b;
	c = quadratic->c;
	discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0)
		return (0);
	discriminant = sqrt(discriminant);
	solutions[0] = (-b - discriminant) / (2 * a);
	solutions[1] = (-b + discriminant) / (2 * a);
	return ((discriminant == 0) + 1);
}	
