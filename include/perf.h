/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   perf.h                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:51 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:51 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERF_H
# define PERF_H

# include "typedefs.h"
# include <sys/time.h>

struct s_perf {
	struct timeval	tv;
};

double	perf_time(void);
void	perf_start(t_perf *perf);
void	perf_split(t_perf *perf, const char *name);

#endif
