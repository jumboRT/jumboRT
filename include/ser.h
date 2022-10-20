#ifndef SER_H
# define SER_H

# include "types.h"
# include "rtmath.h"

# define RT_FLOAT_MANTISSA_BITS 8
# define RT_FLOAT_EXPONENT_BITS 8

void	*rt_packu64(void *dst, uint64_t i);
void	*rt_packfl(void *dst, float f);
void	*rt_packvec(void *dst, t_vec vec);
void	*rt_packhfl(void *dst, float f);
void	*rt_packhvec(void *dst, t_vec vec);
void	*rt_upacku64(void *src, uint64_t *i);
void	*rt_upackfl(void *dst, float *f);
void	*rt_upackvec(void *dst, t_vec *vec);
void	*rt_upackhfl(void *dst, float *f);
void	*rt_upackhvec(void *dst, t_vec *vec);

#endif
