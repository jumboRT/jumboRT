#include "rtmath.h"

int
	quadratic_solve(const t_quadratic *quadratic, float solutions[2])
{
	float	a;
	float	b;
	float	c;
	float	discriminant;

	a = quadratic->a;
	b = quadratic->b;
	c = quadratic->c;
	discriminant = (b * b) - (4.0 * a * c);
	if (discriminant < 0.0)
		return (0);
	discriminant = rt_sqrt(discriminant);
	a = 1.0 / (2.0 * a);
	solutions[0] = (-b + discriminant) * a;
	solutions[1] = (-b - discriminant) * a;
	return ((discriminant == 0.0) + 1);
}
