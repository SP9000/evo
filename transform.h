#ifndef TRANSFORM_H
#define TRANSFORM_H
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "tv_vector.h"

typedef struct tv_transform {
	tv_vector3 pos;
	tv_vector4 rot;
	tv_vector3 scale;
}tv_transform;

#ifdef __cplusplus
}
#endif
#endif