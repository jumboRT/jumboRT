#include "material.h"
#include "scene.h"
#include <math.h>

const t_material_vt
	*emitter_vt(void)
{
	static const t_material_vt	vt = {
		emitter_scatter,
		emitter_destroy
	};

	return (&vt);
}

int
	emitter_scatter(t_material *mat, t_ray in, t_hit *hit, t_scatter *scatter, t_thread_ctx *ctx)
{
	t_emitter	*emitter;
	int			tmp;
	t_vec		sample;

	emitter = (t_emitter *) mat;
	tmp = 0;
	if (emitter->child != NULL)
		tmp = emitter->child->vt->scatter(emitter->child, in, hit, scatter, ctx);
	else
		scatter->emittance = vec(0, 0, 0, 0);
	sample = emitter->emittance->vt->sample(emitter->emittance, hit->uv);
	scatter->emittance = vec_add(scatter->emittance, vec_scale(sample, emitter->brightness));
	return (tmp);
}

void
	emitter_destroy(t_material *mat)
{
	t_emitter	*emitter;

	emitter = (t_emitter *) mat;
	emitter->child->vt->destroy(emitter->child);
	emitter->emittance->vt->destroy(emitter->emittance);
	rt_free(mat);
}
