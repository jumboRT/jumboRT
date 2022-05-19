#include "scene.h"
#include <math.h>

const t_entity_vt
	*plane_vt(void)
{
	static const t_entity_vt	vt = {
		NULL
	};

	return (&vt);
}

int
	plane_hit(t_entity *ent, t_ray ray, t_hit *hit)
{

}
