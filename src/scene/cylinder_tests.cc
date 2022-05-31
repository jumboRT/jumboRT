extern "C" {
#include "cylinder.c"
}

#include "simple_tests.hh"
#include <cstdlib>

void rt_free(void *ptr) {
	free(ptr);
}

SIMPLE_TEST(isparallel) {
	t_cylinder	cyl;
}

int main(int argc, char **argv) {
	SIMPLE_INIT(argc, argv);
	SIMPLE_RUN_ALL_TESTS();
	SIMPLE_CLEANUP();
	return (0);
}
