#ifndef OPENCL_H
# define OPENCL_H

# ifdef RT_LINUX
#  include <CL/cl.h>
# else
#  include <OpenCL/cl.h>
# endif

#endif
