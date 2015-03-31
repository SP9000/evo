#include "tv_alloc.h"

/******************************************************************************
 * tv_alloc
 * Allocates a block of memory of the given size.  Equivalent to "malloc", 
 * however all calls should be made to this function to allow for variations 
 * to the implementation as necessary for porting/performances/etc.
 *****************************************************************************/
tvpointer tv_alloc(tvuint size)
{
	return malloc(size);
}
/******************************************************************************
 * tv_calloc
 * Equivalent to "calloc"
 *****************************************************************************/
tvpointer tv_calloc(tvuint num, tvuint size)
{
	return calloc(num, size);
}
/******************************************************************************
 * tv_calloc
 * Equivalent to "free"
 *****************************************************************************/
void tv_free(tvpointer ptr)
{
	free(ptr);
}