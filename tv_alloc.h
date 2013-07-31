#ifndef TV_ALLOC_H
#define TV_ALLOC_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "types.h"

tvpointer tv_alloc(tvuint size);
tvpointer tv_calloc(tvuint num, tvuint size);

#ifdef __cplusplus
}
#endif
#endif