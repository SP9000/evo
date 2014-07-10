#include "tv_alloc.h"

tvpointer tv_alloc(tvuint size)
{
	return malloc(size);
}
tvpointer tv_calloc(tvuint num, tvuint size)
{
	return calloc(num, size);
}

void tv_free(tvpointer ptr)
{
	free(ptr);
}