#include "world.h"

int prim_is_infinite(const t_primitive *prim) {
	if (prim_type(prim) == RT_SHAPE_PLANE)
		return (1);
	if (prim_type(prim) == RT_SHAPE_PARABOLOID)
		return (1);
	if (prim_type(prim) == RT_SHAPE_HYPERBOLOID)
		return (1);
	return (0);
}
