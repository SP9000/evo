#ifndef TV_MATH
#define TV_MATH

#ifdef __cplusplus
extern "C" {
#endif

#include <float.h>
#include <math.h>
#include "tv_types.h"

#define TV_INF FLT_MAX

static tvfloat tv_sqr(tvfloat f) {return f * f;}

#ifdef __cplusplus
}
#endif
#endif