#include <CL/cl.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

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
}

void
	write_file(char *str, size_t len, char *file)
{
	int		fd;
	ssize_t	n;
	size_t	i;

	fd = open(file, O_WRONLY | O_CREAT);
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

void
	write_program(cl_program program, char *file)
{
	cl_int			status;
	unsigned char	*bin;
	size_t			size;

	status = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size), &size, NULL);
	assert(status == CL_SUCCESS);
	bin = malloc(size);
	assert(bin != NULL);
	status = clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(bin), &bin, NULL);
	assert(status == CL_SUCCESS);
	write_file((char *) bin, size, file);
	free(bin);
}

void
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
	write_program(program, "kernel.bin");
	status = clReleaseProgram(program);
	assert(status == CL_SUCCESS);
	free_files(strings, lengths, count);
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
	context = clCreateContextFromType(context_props, CL_DEVICE_TYPE_DEFAULT, NULL, NULL, &status);
	assert(status == CL_SUCCESS);
	return (context);
}

int
	main(int argc, char **argv)
{
	cl_int					status;
	cl_context				context;
	cl_platform_id			platform;
	cl_device_id			device;

	status = clGetPlatformIDs(1, &platform, NULL);
	assert(status == CL_SUCCESS);
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
	assert(status == CL_SUCCESS);
	context = create_context(platform);
	create_program(context, device, argv + 1, argc - 1);
	status = clReleaseContext(context);
	assert(status == CL_SUCCESS);
}

