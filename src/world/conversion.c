#include "world.h"

t_vec world_to_local(t_world_hit *hit, t_vec v) {
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = hit->rel_shading_normal;
	ss = vec_norm(hit->hit.dpdu);
	ts = vec_norm(hit->hit.dpdv);
	return (vec(vec_dot(v, ss),
				vec_dot(v, ts),
				vec_dot(v, ns),
				0.0));
	return vec_norm2(v);
}

t_vec local_to_world(t_world_hit *hit, t_vec v) {
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = hit->rel_shading_normal;
	ss = vec_norm(hit->hit.dpdu);
	ts = vec_norm(hit->hit.dpdv);
	return (vec_norm2(vec(
			x(ss) * x(v) + x(ts) * y(v) + x(ns) * z(v),
			y(ss) * x(v) + y(ts) * y(v) + y(ns) * z(v),
			z(ss) * x(v) + z(ts) * y(v) + z(ns) * z(v),
			0.0)));
}
