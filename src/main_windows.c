/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main_windows.c                                 #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:08 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:08 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#if defined RT_WINDOWS
# include <winsock.h>
#endif

#if defined RT_WINDOWS

void
	windows_init(void)
{
	WSADATA	wsa_data;

	if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
	{
		fprintf(stderr, "WSAStartup failed\n");
		exit(EXIT_FAILURE);
	}
}

void
	windows_exit(void)
{
	WSACleanup();
}

#else

void
	windows_init(void)
{
}

void
	windows_exit(void)
{
}

#endif
