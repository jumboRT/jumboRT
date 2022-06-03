#ifndef TREE_H
#define TREE_H

#include "rtmath.h"
#include "scene.h"

typedef struct s_tree	t_tree;

struct s_tree {
	t_vec		plane_pos;
	t_vec		plane_dir;
	t_entity	**list;
	size_t		size;
	t_tree		*left;
	t_tree		*right;
};

#endif
