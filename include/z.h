/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   z.h                                            #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:55 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 13:08:46 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef Z_H
# define Z_H

# include "types.h"
# include "typedefs.h"
# include "vector.h"
# include <stdlib.h>

//#define ZWINDOW_SIZE (32768)
/*
# define ZWINDOW_SIZE (1024)
# define ZEMPTY ((t_int16) -1)
# define ZHASH_SIZE 2
# define ZTABLE_SIZE (1 << (ZHASH_SIZE * 8))
# define ZHASH_MASK (ZTABLE_SIZE - 1)
# define ZTOKEN_MIN_LENGTH 3
# define ZTOKEN_MAX_LENGTH 258
*/
# define ZWINDOW_SIZE (1024)
# define ZEMPTY -1
# define ZHASH_SIZE 2
# define ZTABLE_SIZE 65536
# define ZHASH_MASK 65535
# define ZTOKEN_MIN_LENGTH 3
# define ZTOKEN_MAX_LENGTH 258

# if ZHASH_SIZE >= 8
#  error "ZHASH_SIZE way to big"
# endif

# if ZHASH_SIZE > ZTOKEN_MIN_LENGTH
#  error "ZHASH_SIZE cannot be bigger than ZTOKEN_MIN_LENGTH"
# endif

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

union u_dc {
	unsigned int	distance;
	unsigned int	character;
};

struct s_ztoken {
	unsigned int	length;
	union u_dc		data;
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
	t_int16	first;
	t_int16	last;
};

struct s_ztable {
	struct s_ztable_entry	*data;
	size_t					size;
};

struct s_zchain {
	size_t		offset;
	t_uint64	hash;
	size_t		next_length;
	t_int16		next;
	size_t		next_best_length;
	t_int16		next_best;
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

struct s_zlzctx {
	t_zchain	*chain;
	t_zchain	*chain_ref;
	t_ztoken	best;
	size_t		current_length;
	size_t		prev_length;
	t_zstate	*state;
	t_uint32	hash;
};

struct s_deflate_fixed_trees {
	t_zwtree	*lt;
	t_zwtree	*dt;
};

struct s_deflate_dynamic_trees {
	t_zwtree	ct;
	t_zwtree	lt;
	t_zwtree	dt;
};

void			z_deflate_plant_trees(t_ztoken *tokens, size_t count,
					t_zwtree *lt, t_zwtree *dt);
unsigned int	z_deflate_analyze_lens(struct s_zwtree_token *tokens,
					unsigned char *lens, unsigned int size);
void			z_deflate_plant_special_tree(struct s_zwtree_token *tokens,
					unsigned int size, t_zwtree *ct);

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
void			zdecode_length(t_zbuf *ib, unsigned int code,
					unsigned int *value);
void			zdecode_dist(t_zbuf *ib, unsigned int code, unsigned int *dist);
void			zencode_length(unsigned int length, unsigned int *code_out,
					unsigned int *extra_bits_out, unsigned int *value_out);
void			zencode_dist(unsigned int dist, unsigned int *code_out,
					unsigned int *extra_bits_out, unsigned int *value_out);
t_ztoken_data	zencode_token(t_ztoken token);
unsigned int	zencode_zwtree_token(struct s_zwtree_token *data,
					unsigned int length, unsigned int count);

void			ztree_find_counts(unsigned int *counts, unsigned char *lens,
					unsigned int count);
void			ztree_find_codes(unsigned short *codes, unsigned int *counts,
					unsigned char *lens, unsigned int count);
void			ztree_default(t_ztree *tree, t_ztree **dst);
void			ztree_init(t_ztree *tree, unsigned char *lens,
					unsigned int count);
unsigned int	ztree_get(t_ztree *tree, t_zbuf *zb);

void			zwtree_find_codes(unsigned int *codes, unsigned int *counts,
					unsigned char *lens);
void			zwtree_default(t_zwtree *tree, t_zwtree **dst);
void			zwtree_init(t_zwtree *tree, size_t *weights,
					unsigned int max_len);
void			zwtree_put(t_zwtree *tree, t_zbuf *zb, unsigned int value);

void			*z_deflate(void *src, uint64_t src_size, uint64_t *dst_size,
					int level);
void			*z_inflate(void *src, uint64_t src_size, uint64_t *dst_size);

t_ztoken		*lz77_deflate(const void *src, size_t src_size,
					size_t *dst_size);

void			zring_create(t_zring *ring, size_t n);
void			zring_destroy(t_zring *ring);
t_zchain		*zring_at(t_zring *ring, t_int16 index);
int				zring_isfull(t_zring *ring);
void			zring_advance(t_zring *ring);

void			ztable_create(t_ztable *table, size_t n);
void			ztable_destroy(t_ztable *table);
t_int16			ztable_at(t_ztable *table, t_uint32 hash);

t_uint32		lz_hash(t_uint64 data);
t_uint64		lz_peek_next(const unsigned char *src, size_t offset,
					size_t src_size);
void			lz_advance(t_zstate *state, size_t amount);
void			lz77_init(t_zstate *state, const void *src, size_t src_size);
void			lz77_destroy(t_zstate *state);
t_ztoken		lz_deflate(t_zstate *state, t_uint32 hash);
t_ztoken		lz_encode(t_zstate *state, t_uint32 hash, t_vector *out);
t_ztoken		ztoken_at(t_zstate *state, size_t offset);
size_t			ztoken_mismatch(t_zstate *state, size_t offset_a,
					size_t offset_b, size_t min);

void			zwtree_find_lens(unsigned char *lens,
					struct s_zwtree_list *lists, unsigned int max_len);
int				zwtree_node_cmp(const void *a_ptr, const void *b_ptr);

unsigned int	reverse_bits(unsigned int x, unsigned int n);

#endif
