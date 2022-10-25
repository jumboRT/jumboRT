/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   image_bin.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:17 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:17 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "state.h"
#include "rtmath.h"
#include "net.h"
#include "ser.h"

#include <ft_printf.h>
#include <stdio.h>
#include <math.h>

size_t
	rt_write_pxm(const char *path, const t_image *image,
			size_t (*proc)(char**, const t_image*))
{
	char	*error;
	size_t	size;
	char	*ppm;

	size = proc(&ppm, image);
	if (rt_writefile(path, &error, ppm, size) < 0)
		rt_assert(0, error);
	rt_free(ppm);
	return (size);
}

size_t
	rt_write_ppm(const char *path, const t_image *image)
{
	return (rt_write_pxm(path, image, rt_image_to_ppm));
}

size_t
	rt_write_pfm(const char *path, const t_image *image)
{
	return (rt_write_pxm(path, image, rt_image_to_pfm));
}
