#include "cl_impl.h"
#include "util.h"

#include <libft.h>

#if RT_USE_OPENCL

static char
	*platform_name(cl_platform_id platform)
{
	cl_int	status;
	char	*name;
	size_t	size;

	status = clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, NULL, &size);
	rt_assert(status == CL_SUCCESS, "clGetPlatformInfo failed");
	name = rt_malloc(size + 1);
	status = clGetPlatformInfo(platform, CL_PLATFORM_NAME, size, name, NULL);
	rt_assert(status == CL_SUCCESS, "clGetPlatformInfo failed");
	name[size] = '\0';
	return (name);
}

static char
	*device_name(cl_device_id device)
{
	cl_int	status;
	char	*name;
	size_t	size;

	status = clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &size);
	rt_assert(status == CL_SUCCESS, "clGetDeviceInfo failed");
	name = rt_malloc(size + 1);
	status = clGetDeviceInfo(device, CL_DEVICE_NAME, size, name, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceInfo failed");
	name[size] = '\0';
	return (name);
}

static char
	*clean_name(char *name)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (name[i] != '\0')
	{
		if (ft_isalnum(name[i]))
			name[j++] = name[i];
		i++;
	}
	name[j] = '\0';
	return (name);
}

char
	*device_token(cl_platform_id platform, cl_device_id device)
{
	size_t	size;
	char	*pname;
	char	*dname;
	char	*file;

	pname = clean_name(platform_name(platform));
	dname = clean_name(device_name(device));
	size = ft_strlen(pname) + ft_strlen(dname) + 2;
	file = rt_malloc(size);
	file[0] = '\0';
	ft_strlcat(file, pname, size);
	ft_strlcat(file, "-", size);
	ft_strlcat(file, dname, size);
	rt_free(pname);
	rt_free(dname);
	return (file);
}

char
	*device_file(const char *prefix,
			cl_platform_id platform, cl_device_id device)
{
	size_t	size;
	char	*pname;
	char	*dname;
	char	*file;

	pname = clean_name(platform_name(platform));
	dname = clean_name(device_name(device));
	size = ft_strlen(pname) + ft_strlen(dname) + ft_strlen(prefix) + 7;
	file = rt_malloc(size);
	file[0] = '\0';
	ft_strlcat(file, prefix, size);
	ft_strlcat(file, "-", size);
	ft_strlcat(file, pname, size);
	ft_strlcat(file, "-", size);
	ft_strlcat(file, dname, size);
	ft_strlcat(file, ".bin", size);
	rt_free(pname);
	rt_free(dname);
	return (file);
}

#endif
