/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   ser.h                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:52 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:52 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef SER_H
# define SER_H

# include "types.h"
# include "rtmath.h"

# define RT_FLOAT_MANTISSA_BITS 8
# define RT_FLOAT_EXPONENT_BITS 8

void	*rt_packu64(void *dst, t_uint64 i);
void	*rt_packfl(void *dst, float f);
void	*rt_packvec(void *dst, t_vec vec);
void	*rt_packhfl(void *dst, float f);
void	*rt_packhvec(void *dst, t_vec vec);
void	*rt_upacku64(void *src, t_uint64 *i);
void	*rt_upackfl(void *dst, float *f);
void	*rt_upackvec(void *dst, t_vec *vec);
void	*rt_upackhfl(void *dst, float *f);
void	*rt_upackhvec(void *dst, t_vec *vec);

#endif
