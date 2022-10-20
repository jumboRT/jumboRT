#include "util.h"

#if RT_BONUS
# if defined RT_WINDOWS

int
	rt_poll(struct pollfd fds[], int nfds, int timeout)
{
	return (WSAPoll(fds, nfds, timeout));
}

# else

int
	rt_poll(struct pollfd fds[], int nfds, int timeout)
{
	return (poll(fds, nfds, timeout));
}

# endif
#endif
