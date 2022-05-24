#include "scene.h"

#include <math.h>
#include <stdlib.h>

const t_entity_vt
	*light_vt(void)
{
	static const t_entity_vt	vt = {
		NULL,
		light_destroy
	};

	return (&vt);
}

void
	light_destroy(t_entity *ent)
{
	rt_free(ent);
}
