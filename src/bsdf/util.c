#include "bsdf.h"

int
	same_hemi(t_vec wa, t_vec wb)
{
	return (z(wa) * z(wb) > 0);
}

float
	costheta(t_vec w)
{
	return (z(w));
}
