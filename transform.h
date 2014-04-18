#ifndef TRANSFORM_H
#define TRANSFORM_H
#ifdef __cplusplus
extern "C" {
#endif

#include "component.h"
#include "types.h"


COMPONENT(tv_transform, tv_component) 
tv_vector3 pos;
tv_vector3 rot;
tv_vector3 scale;
ENDCOMPONENT(tv_transform)

#ifdef __cplusplus
}
#endif
#endif