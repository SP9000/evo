#ifndef TV_TREE_H
#define TV_TREE_H
#ifdef __cplusplus
extern "C" {
#endif

#define TV_TREE_END UINT_MAX
#include "tv_types.h"
#include "tv_alloc.h"

typedef struct TV_tree_node {
	tvuint parent;
	tvuint sibling;
	tvuint child;
	tvuint id;
}TvTreeNode;

/**
 * A structure for representing a K-ary tree.
 * The TV_Tree structure is optimized for sets of data where it is often
 * convienient to have separate data and metadata for each element.  Within
 * the engine this can be seen in the animation system, where data regarding
 * the heirarchy and identification of bones is handy to store, but wasteful
 * to pass to the skeletal animation system.
 */
typedef struct TV_tree{
	/* contains the tree structure */
	tv_array *tree;
	/* contains the actual data */
	tv_array *data;
}TV_tree;

void tv_tree_new(TV_tree *tree, tvuint element_size);
tvuint tv_tree_add(TV_tree *tree, tvuint parent);
tvuint tv_tree_insert(TV_tree *tree, tvuint handle);
void tv_tree_remove(TV_tree *tree, tvuint handle);

#ifdef __cplusplus
}
#endif
#endif
