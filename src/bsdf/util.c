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

float
	cosphi(t_vec w)
{
	float	sinthetaw;

	sinthetaw = sintheta(w);
	if (sinthetaw == 0.0f)
		return (1.0f);
	return (rt_clamp(x(w) / sinthetaw, -1.0f, 1.0f));
}

float
	sinphi(t_vec w)
{
	float	sinthetaw;

	sinthetaw = sintheta(w);
	if (sinthetaw == 0.0f)
		return (0.0f);
	return (rt_clamp(y(w) / sinthetaw, -1.0f, 1.0f));
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

