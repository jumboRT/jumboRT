#include "rtmath.h"

int
	quadratic_solve(const t_quadratic *quadratic, float solutions[2])
{
	float	a;
	float	b;
	float	c;
	float	q;
	float	discriminant;

	a = quadratic->a;
	b = quadratic->b;
	c = quadratic->c;
	discriminant = (b * b) - (4.0 * a * c);
	if (discriminant < 0.0)
		return (0);
	discriminant = rt_sqrt(discriminant);
	if (b < 0)
		q = -0.5 * (b - discriminant);
	else
		q = -0.5 * (b + discriminant);
	solutions[0] = q / a;
	solutions[1] = c / q;
	if (solutions[0] > solutions[1])
	{
		q = solutions[0];
		solutions[0] = solutions[1];
		solutions[1] = q;
	}
	return (1);
}
