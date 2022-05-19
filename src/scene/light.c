#include "scene.h"
#include <math.h>

const t_entity_vt
	*light_vt(void)
{
	static const t_entity_vt	vt = {
		NULL
	};

	return (&vt);
}
