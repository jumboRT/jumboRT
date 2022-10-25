/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   types.h                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:52 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:52 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# ifndef RT_OPENCL

#  include <stdint.h>

#  define GLOBAL

typedef void			t_void;

typedef int8_t			t_int8;
typedef int16_t			t_int16;
typedef int32_t			t_int32;
typedef int64_t			t_int64;
typedef uint8_t			t_uint8;
typedef uint16_t		t_uint16;
typedef uint32_t		t_uint32;
typedef uint64_t		t_uint64;

# else

typedef void			t_void;

typedef signed char		t_int8;
typedef signed short	t_int16;
typedef signed int		t_int32;
typedef signed long		t_int64;
typedef unsigned char	t_uint8;
typedef unsigned short	t_uint16;
typedef unsigned int	t_uint32;
typedef unsigned long	t_uint64;

# endif

#endif
