#include "bsdf.h"

float
	costheta(t_vec w)
{
	return (z(w));
}

float
	cos2theta(t_vec w)
{
	return (z(w) * z(w));
}

float
	sin2theta(t_vec w)
{
	return (rt_max(0.0f, 1.0f - cos2theta(w)));
}

float
	sintheta(t_vec w)
{
	return (rt_sqrt(sin2theta(w)));
}

float
	abscostheta(t_vec w)
{
	return (rt_abs(costheta(w)));
}
