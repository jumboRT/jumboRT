#include "world.h"

int prim_is_infinite(const t_primitive *prim) {
	return (prim_type(prim) ==  RT_SHAPE_PLANE);
}
