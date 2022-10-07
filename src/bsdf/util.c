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

float
	f_dielectric(float costhetai, float etai, float etat)
{
	float	sinthetai;
	float	sinthetat;
	float	costhetat;
	float	tmp;
	float	rparl;
	float	rperp;


	costhetai = rt_clamp(costhetai, -1.0f, 1.0f);

	if (costhetai < 0) {
		tmp = etai;
		etai = etat;
		etat = tmp;
		costhetai = rt_abs(costhetai);
	}

	sinthetai = rt_sqrt(rt_max(0.0f, 1.0f - costhetai * costhetai));
	sinthetat = etai / etat * sinthetai;

	if (sinthetat >= 1.0f)
		return (1);
	costhetat = rt_sqrt(rt_max(0.0f, 1.0f - sinthetat * sinthetat));
	rparl = ((etat * costhetai) - (etai * costhetat)) /
		((etat * costhetai) + (etai * costhetat));
	rperp = ((etai * costhetai) - (etat * costhetat)) /
		((etai * costhetai) + (etat * costhetat));
	return ((rparl * rparl + rperp * rperp) / 2.0f);
}

