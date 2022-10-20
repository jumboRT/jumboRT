#include "z.h"

#include "util.h"

void
	ztable_create(t_ztable *table, size_t n)
{
	table->data = rt_malloc(sizeof(*table->data) * n);
	table->size = n;
	while (n--)
	{
		table->data[n].first = ZEMPTY;
		table->data[n].last = ZEMPTY;
	}
}

void
	ztable_destroy(t_ztable *table)
{
	rt_free(table->data);
	table->size = 0;
}

int16_t
	ztable_at(t_ztable *table, uint32_t hash)
{
	return (table->data[hash].first);
}
