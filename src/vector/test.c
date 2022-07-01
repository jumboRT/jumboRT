#include "vector.h"

#include <stdio.h>
#include <time.h>

int
	compare(const void *a, const void *b, void *ctx)
{
	float	float_a;
	float	float_b;

	(void) ctx;
	float_a = *(float *) a;
	float_b = *(float *) b;
	return ((float_a > float_b) - (float_a < float_b));
}

int
	main(void)
{
	t_seed		seed;
	t_vector	vec;
	size_t		i;
	float		f;
	clock_t		start;
	clock_t		end;

	seed = 7549087012;
	vector_init(&vec, sizeof(float));
	i = 0;
	while (i < 10000000)
	{
		f = rt_random_float(&seed);
		vector_push_back(&vec, &f);
		i += 1;
	}
	start = clock();
	vector_sort(&vec, compare, NULL);
	end = clock();
	fprintf(stderr, "%f\n", (float) (end - start) / CLOCKS_PER_SEC);
	i = 0;
	while (i < vector_size(&vec))
	{
		printf("%f\n", *(float *) vector_at(&vec, i));
		i += 1;
	}
	vector_destroy(&vec, NULL);
}
