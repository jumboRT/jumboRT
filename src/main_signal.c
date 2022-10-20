#include "util.h"

#include <signal.h>

sig_atomic_t
	should_exit(sig_atomic_t incr)
{
	static sig_atomic_t	val = 0;

	val += incr;
	return (val);
}

#if RT_BONUS && !defined RT_JOINC

static void
	sigint_handler(int sig)
{
	(void) sig;
	should_exit(1);
}

void
	setup_sighandlers(void)
{
	struct sigaction	action;

	rt_assert(sigemptyset(&action.sa_mask) != -1, "failed to empty sa_mask");
	action.sa_handler = sigint_handler;
	action.sa_flags = SA_NODEFER | SA_RESTART;
	rt_assert(sigaction(SIGINT, &action, NULL) != -1,
		"failed to install signal handler");
}

#else

void
	setup_sighandlers(void)
{
	printf("warning: you're using the mandatory configuration or running on"
		"windows. NEVER forcefully kill this process!\n");
}

#endif
