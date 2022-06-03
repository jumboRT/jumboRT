#include "scene.h"

#include <math.h>
#include <stdlib.h>

const t_entity_vt
	*ambient_light_vt(void)
{
	static const t_entity_vt	vt = {
		NULL,
		ambient_light_destroy,
		NULL,
		NULL,
	};

	return (&vt);
}

void
	ambient_light_destroy(t_entity *ent)
{
	rt_free(ent);
}

