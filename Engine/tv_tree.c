#include "tv_tree.h"

void tv_tree_new(TV_tree *tree, tvuint element_size)
{
	UT_icd icd = {element_size, 0, 0, 0};
	UT_icd ticd = {sizeof(TvTreeNode), 0, 0};
	utarray_new(tree->data, &icd);
	utarray_new(tree->tree, &ticd);
}
tvuint tv_tree_add(TV_tree *tree, tvuint parent)
{
	tvuint i, ret;
	return ret;
}
tvuint tv_tree_insert(TV_tree *tree, tvuint handle)
{
}
void tv_tree_remove(TV_tree *tree, tvuint handle)
{

}