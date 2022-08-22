#ifndef Z_H
#define Z_H

#include "vector.h"
#include <stdlib.h>
#include <libft.h>

#define ZEMPTY 0
#define ZWINDOW_SIZE 32768 /* TODO USE ACTUAL VALUES */
#define ZTABLE_SIZE 16777216
#define ZHASH_SIZE 3

typedef struct s_zbuf	t_zbuf;
typedef struct s_ztree	t_ztree;
typedef struct s_ztoken	t_ztoken;
typedef struct s_ztable t_ztable;
typedef struct s_zchain	t_zchain;
typedef struct s_zstate t_zstate;

struct s_zbuf {
	unsigned char	*data;
	size_t			index;
	size_t			size;
	size_t			capacity;
	int				bit;
};

struct s_ztree {
	unsigned int	count;
	unsigned int	counts[17];
	unsigned int	codes[288];
};

struct s_ztoken {
	unsigned int		length;
	union {
		unsigned int	distance;
		unsigned int	character;
	} data;
};

struct s_ztable {
	t_zchain	**data;
	size_t		size;
};

struct s_zchain {
	size_t		offset;
	t_ztoken	token;
	t_zchain	*next;
};

struct s_zstate {
	const unsigned char	*src;
	const unsigned char	*win;
	t_ztable			table;
	t_vector			result;
	size_t				src_size;
};

void			zbuf_create(t_zbuf *zb, void *data, size_t size);
void			zbuf_next(t_zbuf *zb);
unsigned int	zbuf_read(t_zbuf *zb, int count);
void			zbuf_write(t_zbuf *zb, int count, unsigned int data);
void			zbuf_copy(t_zbuf *zb, t_zbuf *src, size_t size);
void			zbuf_repeat(t_zbuf *zb, size_t dist, size_t size);

void			ztree_decode_code(t_zbuf *ib, unsigned int code, unsigned int *n);
void			ztree_decode_length(t_zbuf *ib, unsigned int code, unsigned int *value);
void			ztree_decode_dist(t_zbuf *ib, unsigned int code, unsigned int *dist);
void			ztree_default(t_ztree *tree);

void			ztree_init(t_ztree *tree, unsigned char *lens);
unsigned int	ztree_get(t_ztree *tree, t_zbuf *zb);

void			*z_deflate(void *src, size_t src_size, size_t *dst_size);
void			*z_inflate(void *src, size_t src_size, size_t *dst_size);

t_ztoken		*lz77_deflate(void *src, size_t src_size, size_t *dst_size);

#endif
