#include "scene.h"

#include <math.h>
#include <stdlib.h>

const t_entity_vt
	*camera_vt(void)
{
	static const t_entity_vt	vt = {
		NULL,
		camera_destroy
	};

	return (&vt);
}

void
	camera_destroy(t_entity *ent)
{
	rt_free(ent);
}
