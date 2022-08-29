#ifndef Z_H
#define Z_H

#include "vector.h"
#include <stdlib.h>

//#define ZWINDOW_SIZE (32768)
#define ZWINDOW_SIZE (1024)
#define ZEMPTY ((int16_t) -1)
#define ZHASH_SIZE 2
#define ZTABLE_SIZE (1 << (ZHASH_SIZE * 8))
#define ZHASH_MASK (ZTABLE_SIZE - 1)
#define ZTOKEN_MIN_LENGTH 3
#define ZTOKEN_MAX_LENGTH 258

#if ZHASH_SIZE >= 8
#error "ZHASH_SIZE way to big"
#endif

#if ZHASH_SIZE > ZTOKEN_MIN_LENGTH
#error "ZHASH_SIZE cannot be bigger than ZTOKEN_MIN_LENGTH"
#endif

typedef struct s_zbuf	t_zbuf;
typedef struct s_ztree	t_ztree;
typedef struct s_zwtree	t_zwtree;
typedef struct s_ztoken	t_ztoken;
typedef struct s_ztable t_ztable;
typedef struct s_zchain	t_zchain;
typedef struct s_zstate t_zstate;
typedef struct s_zring	t_zring;

struct s_zbuf {
	unsigned char	*data;
	size_t			index;
	size_t			size;
	size_t			capacity;
	int				bit;
};

struct s_ztree {
	unsigned int	count;
	unsigned char	lens[288];
	unsigned short	codes[32768];
};

struct s_zwtree {
	unsigned int	count;
	unsigned char	lens[288];
	unsigned int	codes[288];
};

struct s_zwtree_node {
	struct s_zwtree_node	*left;
	struct s_zwtree_node	*right;
	size_t					weight;
	unsigned int			code;
};

struct s_zwtree_list {
	struct s_zwtree_node	nodes[575];
	size_t					size;
};

struct s_zwtree_token {
	unsigned int	code;
	unsigned int	extra_bits;
	unsigned int	value;
};

struct s_ztoken {
	unsigned int		length;
	union {
		unsigned int	distance;
		unsigned int	character;
	} data;
};

struct s_ztoken_data {
	unsigned int	length_code;
	unsigned int	dist_code;
	unsigned int	length_extra_bits;
	unsigned int	dist_extra_bits;
	unsigned int	length_value;
	unsigned int	dist_value;
};

struct s_ztable_entry {
	int16_t	first;
	int16_t	last;
};

struct s_ztable {
	struct s_ztable_entry	*data;
	size_t					size;
};

struct s_zchain {
	size_t		offset;
	uint64_t	hash;
	size_t		next_length;
	int16_t		next;
	size_t		next_best_length;
	int16_t		next_best;
};

struct s_zring {
	t_zchain	*data;
	size_t		size;
	size_t		index;
	size_t		int_size;
};

struct s_zstate {
	const unsigned char	*src;
	t_ztable			table;
	t_zring				ring;
	size_t				src_size;
	size_t				offset;
};

unsigned int	zfrob(unsigned int x);

void			zbuf_create(t_zbuf *zb, void *data, size_t size);
void			zbuf_next(t_zbuf *zb);
unsigned int	zbuf_read(t_zbuf *zb, int count);
void			zbuf_skip(t_zbuf *zb, int count);
unsigned int	zbuf_peek(t_zbuf *zb, int count, int *i);
void			zbuf_write(t_zbuf *zb, int count, unsigned int data);
void			zbuf_write_aligned(t_zbuf *zb, int count, unsigned int data);
void			zbuf_copy(t_zbuf *zb, t_zbuf *src, size_t size);
void			zbuf_repeat(t_zbuf *zb, size_t dist, size_t size);

void			zdecode_code(t_zbuf *ib, unsigned int code, unsigned int *n);
void			zdecode_length(t_zbuf *ib, unsigned int code, unsigned int *value);
void			zdecode_dist(t_zbuf *ib, unsigned int code, unsigned int *dist);
void			zencode_length(unsigned int length, unsigned int *code_out, unsigned int *extra_bits_out, unsigned int *value_out);
void			zencode_dist(unsigned int dist, unsigned int *code_out, unsigned int *extra_bits_out, unsigned int *value_out);
struct s_ztoken_data
				zencode_token(t_ztoken token);
unsigned int	zencode_zwtree_token(struct s_zwtree_token *data, unsigned int length, unsigned int count);

void			ztree_find_counts(unsigned int *counts, unsigned char *lens, unsigned int count);
void			ztree_find_codes(unsigned short *codes, unsigned int *counts, unsigned char *lens, unsigned int count);
void			ztree_default(t_ztree *tree, t_ztree **dst);
void			ztree_init(t_ztree *tree, unsigned char *lens, unsigned int count);
unsigned int	ztree_get(t_ztree *tree, t_zbuf *zb);

void			zwtree_find_codes(unsigned int *codes, unsigned int *counts, unsigned char *lens);
void			zwtree_default(t_zwtree *tree, t_zwtree **dst);
void			zwtree_init(t_zwtree *tree, size_t *weights, unsigned int max_len);
void			zwtree_put(t_zwtree *tree, t_zbuf *zb, unsigned int value);

void			*z_deflate(void *src, size_t src_size, size_t *dst_size);
void			*z_inflate(void *src, size_t src_size, size_t *dst_size);

t_ztoken		*lz77_deflate(const void *src, size_t src_size,
					size_t *dst_size);

unsigned int	reverse_bits(unsigned int x, unsigned int n);

#endif
