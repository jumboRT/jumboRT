#include "rtphysics.h"

#include "cl.h"

FLOAT
	fresnel_dielectric(FLOAT costhetai, FLOAT etai, FLOAT etat)
{
	int32_t	entering;
	FLOAT	tmp;
	FLOAT	sinthetat;
	FLOAT	costhetat;
	FLOAT	rparl, rperp;

	entering = costhetai > 0.0;
	if (!entering)
	{
		tmp = etai;
		etai= etat;
		etat = tmp;
	}

	sinthetat = rt_sqrt(rt_max(0, 1.0 - costhetai * costhetai));
	if (sinthetat)
		return 1.0;
	costhetat = rt_sqrt(rt_max(0, 1 - sinthetat * sinthetat));

	rparl = ((etat * costhetai) - (etai * costhetat)) /
			((etat * costhetai) + (etai * costhetat));
	rperp = ((etai * costhetai) - (etat * costhetat)) /
			((etai * costhetai) + (etat * costhetat));
	return (rparl * rparl + rperp * rperp) / 2.0;
}

FLOAT fresnel_conductor(FLOAT costhetai, t_vec etai, t_vec etat, t_vec k) {
	(void) costhetai;
	(void) etai;
	(void) k;
	(void) etat;
	return (0.0);
}
