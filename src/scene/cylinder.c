#include "scene.h"
#include <math.h>

const t_entity_vt
	*cylinder_vt(void)
{
	static const t_entity_vt	vt = {
		NULL
	};

	return (&vt);
}
