#ifndef TV_ALLOC_H
#define TV_ALLOC_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "tv_types.h"

tvpointer tv_alloc(tvuint size);
tvpointer tv_calloc(tvuint num, tvuint size);
void tv_free(tvpointer ptr);

#ifdef __cplusplus
}
#endif
#endif