#ifndef CL_H
# define CL_H

# ifndef RT_OPENCL

#  include <stdint.h>

#  define GLOBAL

# else

typedef signed char		int8_t;
typedef signed short	int16_t;
typedef signed int		int32_t;
typedef signed long		int64_t;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned long	uint64_t;

# endif

#endif
