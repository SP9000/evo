#ifndef TV_PHYSICS
#define TV_PHYSICS
#ifdef __cplusplus
extern "C" {
#endif

#include "tv_collider.h"
#include "tv_types.h"
#include "tv_vector.h"

typedef struct TV_physics_hit_info {
	tv_vector3 hit_loc;
	TV_collider *collider;
}TV_physics_hit_info;

tvint tv_physics_init();
/**
 * Cast a ray from the given coordiante in the given direction for the given
 * length.
 * @param src the location to cast the ray from.
 * @param dir the direction to cast the ray in.
 * @param len the length of the ray to cast.
 * @return information about the hit of the raycast.
 */
TV_physics_hit_info tv_physics_raycast(tv_vector3 src, tv_vector3 dir, tvfloat len);

#ifdef __cplusplus
}
#endif
#endif