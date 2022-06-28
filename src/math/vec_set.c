#include "rtmath.h"

t_vec
	vec_set(t_vec v, int axis, FLOAT val)
{
	if (axis == 0) {
		return (vec(val, y(v), z(v)));
	}
	if (axis == 1) {
		return (vec(x(v), val, z(v)));
	}
	if (axis == 2) {
		return (vec(x(v), y(v), val));
	}
	return (vec_0());
}
