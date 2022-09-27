#include "tex.h"

void
	texture_init(t_tex *tex)
{
	tex->type = RT_TEX_COLOR;
	tex->a.color = vec(0, 0, 0, 0);
}
