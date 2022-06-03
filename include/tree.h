#ifndef TREE_H
# define TREE_H

#include "rtmath.h"
#include "scene.h"

# ifndef RT_TREE_DIRECTIONS
#  define RT_TREE_DIRECTIONS 12
# endif

# ifndef RT_TREE_STEPS
#  define RT_TREE_STEPS 10
# endif

typedef struct s_tree		t_tree;
typedef struct s_tree_plane	t_tree_plane;

struct s_tree {
	t_vec		plane_pos;
	t_vec		plane_dir;
	t_entity	**list;
	size_t		count;
	t_tree		*front;
	t_tree		*back;
};

struct s_tree_plane {
	t_vec	pos;
	t_vec	dir;
};

t_tree			*tree_new(t_entity **list, size_t count);
t_tree			*tree_copy(t_tree *other);
void			tree_swap(t_tree *tree, t_tree *other);
FLOAT			tree_quality(t_tree *tree);
void			tree_optimize(t_tree *tree, int depth);
void			tree_destroy(t_tree *tree);
int				tree_hit(t_tree *tree, t_ray ray, t_hit *hit, FLOAT max_t);

t_tree_plane	*tree_gen_planes(t_entity **list, size_t count, size_t *out_count);

#endif
