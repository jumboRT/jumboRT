#include "world.h"

t_vec
	world_to_local(const t_world_hit *hit, t_vec v)
{
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = hit->hit.shading_normal;
	ss = vec_norm(hit->hit.dpdu);
	ts = vec_norm(hit->hit.dpdv);
	return (vec(vec_dot(v, ss),
			vec_dot(v, ts),
			vec_dot(v, ns),
			0.0));
}

t_vec
	local_to_world(const t_world_hit *hit, t_vec v)
{
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = hit->hit.shading_normal;
	ss = vec_norm(hit->hit.dpdu);
	ts = vec_norm(hit->hit.dpdv);
	return (vec_norm2(vec(
				x(ss) * x(v) + x(ts) * y(v) + x(ns) * z(v),
				y(ss) * x(v) + y(ts) * y(v) + y(ns) * z(v),
				z(ss) * x(v) + z(ts) * y(v) + z(ns) * z(v),
				0.0)));
}
