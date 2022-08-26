#ifndef RTPHYSICS_H
# define RTPHYSICS_H

# include "rtmath.h"

float fresnel_dielectric(float costhetai, float etai, float etat);
float fresnel_conductor(float costhetai, t_vec etai, t_vec etat, t_vec k);

t_vec fresnel_dielectric_eval(float costheta);

#endif
