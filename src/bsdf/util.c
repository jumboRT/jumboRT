/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   util.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:18 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:18 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "bsdf.h"

int
	same_hemi(t_vec wa, t_vec wb)
{
	return (z(wa) * z(wb) > 0);
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
	float	r[2];

	costhetai = rt_clamp(costhetai, -1.0f, 1.0f);
	if (costhetai < 0)
	{
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
	r[0] = ((etat * costhetai) - (etai * costhetat))
		/ ((etat * costhetai) + (etai * costhetat));
	r[1] = ((etai * costhetai) - (etat * costhetat))
		/ ((etai * costhetai) + (etat * costhetat));
	return ((r[0] * r[0] + r[1] * r[1]) / 2.0f);
}
