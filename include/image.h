/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   image.h                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:49 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:49 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_H
# define IMAGE_H

# include "typedefs.h"
# include "rtmath.h"

# include <stddef.h>

struct s_pixel {
	t_vec	color;
	size_t	samples;
};

struct s_image {
	t_pixel	*data;
	size_t	width;
	size_t	height;
};

#endif
