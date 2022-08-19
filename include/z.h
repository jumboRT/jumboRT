#ifndef Z_H
#define Z_H

#include <stdlib.h>

typedef struct s_zbuf	t_zbuf;
typedef struct s_ztree	t_ztree;

struct s_zbuf {
	unsigned char	*data;
	size_t			index;
	size_t			size;
	size_t			capacity;
	int				bit;
};

struct s_ztree {
	unsigned int	count;
	unsigned int	counts[16];
	unsigned int	codes[288];
};

void			zbuf_create(t_zbuf *zb, void *data, size_t size);
void			zbuf_next(t_zbuf *zb);
unsigned int	zbuf_read(t_zbuf *zb, int count);
void			zbuf_write(t_zbuf *zb, int count, unsigned int data);
void			zbuf_copy(t_zbuf *zb, t_zbuf *src, size_t size);
void			zbuf_repeat(t_zbuf *zb, size_t dist, size_t size);

void			ztree_decode_code(t_zbuf *ib, unsigned int code, unsigned int *length);
void			ztree_decode_length(t_zbuf *ib, unsigned int code, unsigned int *value);
void			ztree_decode_dist(t_zbuf *ib, unsigned int code, unsigned int *dist);
void			ztree_default(t_ztree *tree);

void			ztree_init(t_ztree *tree, unsigned char *lens);
unsigned int	ztree_get(t_ztree *tree, t_zbuf *zb);

void			*z_deflate(void *src, size_t src_size, size_t *dst_size);
void			*z_inflate(void *src, size_t src_size, size_t *dst_size);

#endif
