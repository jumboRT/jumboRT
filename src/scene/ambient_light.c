#include "scene.h"
#include <math.h>

const t_entity_vt
	*ambient_light_vt(void)
{
	static const t_entity_vt	vt = {
		NULL
	};

	return (&vt);
}
