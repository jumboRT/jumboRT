/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main_worker.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:16 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:16 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "net.h"
#include <ft_printf.h>

#if RT_BONUS

void
	main_worker(t_options *options)
{
	union u_client	client;

	rt_worker_create(&client, *options, options->net_ip, options->net_port);
	rt_client_start(&client);
	rt_client_destroy(&client);
}

#else

void
	main_worker(t_options *options)
{
	(void) options;
	ft_printf("Can't run in worker mode for mandatory part\n");
}

#endif
