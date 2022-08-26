#include "rtmath.h"

t_vec
	vec_set(t_vec v, int axis, float val)
{
	if (axis == 0) {
		return (vec(val, y(v), z(v), w(v)));
	}
	if (axis == 1) {
		return (vec(x(v), val, z(v), w(v)));
	}
	if (axis == 2) {
		return (vec(x(v), y(v), val, w(v)));
	}
	if (axis == 3) {
		return (vec(x(v), y(v), z(v), val));
	}
	return (vec_0());
}
