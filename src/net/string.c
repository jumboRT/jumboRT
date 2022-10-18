#if RT_BONUS
#include "net.h"

#include "util.h"
#include <string.h>

void
	rt_string_create(struct s_string *dst, const char *str)
{

	dst->len = strlen(str);
	dst->str = rt_malloc(dst->len + 1);
	memcpy(dst->str, str, dst->len + 1);
}

void
	rt_string_destroy(struct s_string *string)
{
	string->len = 0;
	rt_free(string->str);
}
#endif
