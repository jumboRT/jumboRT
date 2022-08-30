#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

# if defined RT_LINUX
#  include <CL/cl.h>
# elif defined RT_WINDOWS
#  include <CL/cl.h>
# else
#  include <OpenCL/cl.h>
# endif

#define READ_SIZE 1024
#define BUILD_FLAGS "-I include -D RT_OPENCL -D GLOBAL=__global -cl-fast-relaxed-math"

void
	read_file(char **str, size_t *len, char *file)
{
	int		fd;
	ssize_t	n;

	fd = open(file, O_RDONLY);
	assert(fd >= 0);
	*str = NULL;
	*len = 0;
	while (1)
	{
		*str = realloc(*str, *len + READ_SIZE);
		assert(*str != NULL);
		n = read(fd, *str + *len, READ_SIZE);
		if (n < 0)
			assert(errno == EINTR);
		if (n > 0)
			*len += n;
		if (n == 0)
			break ;
	}
	close(fd);
	*str = realloc(*str, *len + 1);
	(*str)[*len] = '\0';
}

void
	write_file(char *str, size_t len, char *file)
{
	int		fd;
	ssize_t	n;
	size_t	i;

	fd = open(file, O_WRONLY | O_CREAT, 0660);
	assert(fd >= 0);
	i = 0;
	while (1)
	{
		n = write(fd, str + i, len - i);
		if (n < 0)
			assert(errno == EINTR);
		if (n > 0)
			i += n;
		if (n == 0)
			break ;
	}
	close(fd);
}

void
	read_files(char ***str, size_t **len, char **files, size_t count)
{
	size_t	i;

	*str = malloc(sizeof(**str) * count);
	*len = malloc(sizeof(**len) * count);
	assert(*str != NULL && *len != NULL);
	i = 0;
	while (i < count)
	{
		read_file(*str + i, *len + i, files[i]);
		i += 1;
	}
}

void
	free_files(char **str, size_t *len, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(str[i]);
		i += 1;
	}
	free(str);
	free(len);
}

void
	check_program(cl_program program, cl_device_id device)
{
	cl_int	status;
	size_t	size;
	char	*str;

	status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
	assert(status == CL_SUCCESS);
	str = malloc(size);
	assert(str != NULL);
	status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size, str, NULL);
	assert(status == CL_SUCCESS);
	write(STDOUT_FILENO, str, size);
	free(str);
	exit(EXIT_FAILURE);
}

char
	*platform_name(cl_platform_id platform)
{
	cl_int	status;
	char	*name;
	size_t	size;

	status = clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, NULL, &size);
	assert(status == CL_SUCCESS);
	name = malloc(size + 1);
	assert(name != NULL);
	status = clGetPlatformInfo(platform, CL_PLATFORM_NAME, size, name, NULL);
	assert(status == CL_SUCCESS);
	name[size] = '\0';
	return (name);
}

char
	*device_name(cl_device_id device)
{
	cl_int	status;
	char	*name;
	size_t	size;

	status = clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &size);
	assert(status == CL_SUCCESS);
	name = malloc(size + 1);
	assert(name != NULL);
	status = clGetDeviceInfo(device, CL_DEVICE_NAME, size, name, NULL);
	assert(status == CL_SUCCESS);
	name[size] = '\0';
	return (name);
}

char
	*clean_name(char *name)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (name[i] != '\0')
	{
		if (isalnum(name[i]))
			name[j++] = name[i];
		i++;
	}
	name[j] = '\0';
	return (name);
}

char
	*device_file(cl_platform_id platform, cl_device_id device)
{
	char	*pname;
	char	*dname;
	char	*file;

	pname = clean_name(platform_name(platform));
	dname = clean_name(device_name(device));
	file = malloc(strlen(pname) + strlen(dname) + 13);
	assert(file != NULL);
	file[0] = '\0';
	strcat(file, "kernel-");
	strcat(file, pname);
	strcat(file, "-");
	strcat(file, dname);
	strcat(file, ".bin");
	free(pname);
	free(dname);
	return (file);
}

void
	write_program(cl_program program, cl_platform_id platform, cl_device_id device)
{
	cl_int			status;
	unsigned char	*bin;
	size_t			size;
	char			*file;

	status = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size), &size, NULL);
	assert(status == CL_SUCCESS);
	bin = malloc(size);
	assert(bin != NULL);
	status = clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(bin), &bin, NULL);
	assert(status == CL_SUCCESS);
	file = device_file(platform, device);
	write_file((char *) bin, size, file);
	free(file);
	free(bin);
}

cl_context
	create_context(cl_platform_id platform)
{
	cl_int					status;
	cl_context_properties	context_props[3];
	cl_context				context;
	
	context_props[0] = CL_CONTEXT_PLATFORM;
	context_props[1] = (cl_context_properties) platform;
	context_props[2] = 0;
	context = clCreateContextFromType(context_props, CL_DEVICE_TYPE_ALL, NULL, NULL, &status);
	assert(status == CL_SUCCESS);
	return (context);
}

cl_program
	create_program(cl_context context, cl_device_id device, char **files, size_t count)
{
	cl_int		status;
	cl_program	program;
	char		**strings;
	size_t		*lengths;

	read_files(&strings, &lengths, files, count);
	program = clCreateProgramWithSource(context, count, (const char **) strings, lengths, &status);
	assert(status == CL_SUCCESS);
	status = clBuildProgram(program, 1, &device, BUILD_FLAGS, NULL, NULL);
	if (status != CL_SUCCESS)
		check_program(program, device);
	free_files(strings, lengths, count);
	return (program);
}

void
	main_device(int argc, char **argv, cl_platform_id platform, cl_device_id device)
{
	cl_int			status;
	cl_context		context;
	cl_program		program;
	
	context = create_context(platform);
	program = create_program(context, device, argv + 1, argc - 1);
	write_program(program, platform, device);
	status = clReleaseProgram(program);
	assert(status == CL_SUCCESS);
	status = clReleaseContext(context);
	assert(status == CL_SUCCESS);
}

void
	main_platform(int argc, char **argv, cl_platform_id platform)
{
	cl_int			status;
	cl_uint			index;
	cl_uint			num_devices;
	cl_device_id	*devices;
	
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
	assert(status == CL_SUCCESS);
	devices = malloc(sizeof(*devices) * num_devices);
	assert(devices != NULL);
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
	assert(status == CL_SUCCESS);
	index = 0;
	while (index < num_devices)
	{
		main_device(argc, argv, platform, devices[index]);
		index += 1;
	}
	free(devices);
}

int
	main(int argc, char **argv)
{
	cl_int			status;
	cl_uint			index;
	cl_uint			num_platforms;
	cl_platform_id	*platforms;

	status = clGetPlatformIDs(0, NULL, &num_platforms);
	assert(status == CL_SUCCESS);
	platforms = malloc(sizeof(*platforms) * num_platforms);
	assert(platforms != NULL);
	status = clGetPlatformIDs(num_platforms, platforms, NULL);
	assert(status == CL_SUCCESS);
	index = 0;
	while (index < num_platforms)
	{
		main_platform(argc, argv, platforms[index]);
		index += 1;
	}
	free(platforms);
}

