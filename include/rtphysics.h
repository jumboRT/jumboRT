#ifndef RTPHYSICS_H
# define RTPHYSICS_H

# include "rtmath.h"

FLOAT fresnel_dielectric(FLOAT costhetai, FLOAT etai, FLOAT etat);
FLOAT fresnel_conductor(FLOAT costhetai, t_vec etai, t_vec etat, t_vec k);

#endif
