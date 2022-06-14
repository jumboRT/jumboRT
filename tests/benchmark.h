#ifndef RT_BENCHMARK_H
# define RT_BENCHMARK_H

#include <stdio.h>
#include <time.h>

# ifndef BENCHMARK_ITER
#  define BENCHMARK_ITER 10000000
# endif
# if BENCHMARK_ITER <= 0
#  error "BENCHMARK_ITER must be a postive integer"
# elif BENCHMARK_ITER > 1000000000
#  error "BENCHMARK_ITER is too large"
# endif

# define DO_NOT_OPTIMIZE(statement) \
	asm volatile("" : : "r,m"(statement) : "memory");

# define BENCHMARK(statement)											\
	clock_t	start = clock();											\
	clock_t	end;														\
	for (long i = 0; i < BENCHMARK_ITER; i++) {							\
		statement;														\
	}																	\
	end = clock();														\
	printf(#statement "took %e in %ld iterations\n", ((double) (end - start) / BENCHMARK_ITER) / CLOCKS_PER_SEC, (unsigned long) BENCHMARK_ITER);
	

#endif
