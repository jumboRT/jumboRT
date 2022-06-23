#ifndef WORK_COMPUTE_H
# define WORK_COMPUTE_H

# include "world.h"
# include "cl.h"

typedef struct s_context	t_context;
typedef struct s_result		t_result;

struct s_context {
	t_seed		seed;
};

struct s_result {
	uint64_t	index;
	t_vec		color;
};

t_result	work_compute(GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index);

#endif
