#ifndef TV_COLLIDER
#define TV_COLLIDER
#ifdef __cplusplus
extern "C" {
#endif

#include "tv_message.h"
#include "tv_component.h"
#include "tv_math.h"

/**
 * An enumeration of the types of colliders.
 */
typedef enum {
	TV_COLLIDER_BOX,
	TV_COLLIDER_SPHERE,
}TV_collider_type;

/**
 * A structure containing information needed to detect box collision.
 */
typedef struct {
	tvfloat w, h, d;
}TV_collider_box_collider_info;

/**
 * A structure containing information needed to detect sphere collision.
 */
typedef struct {
	tvfloat radius;
}TV_collider_sphere_collider_info;

/**
 * A structure containing information needed to detect collider collision.
 */
typedef union TV_collider_info {
	TV_collider_box_collider_info box;
	TV_collider_sphere_collider_info sphere;
}TV_collider_info;

COMPONENT(TV_collider, tv_component)
	tv_vector3 pos;
	/* the type of the collider */
	TV_collider_type type;
	/* the data needed by this collider to check collisions */
	TV_collider_info info;

	/* SIGNALS */
	/* a signal that is emitted while this collider overlaps another */
	TV_message_signal *on_collision;
	/* a signal that is emitted when collision with another collider begins */
	TV_message_signal *on_collision_enter;
	/* a signal that is emitted when collision with another collider ceases */
	TV_message_signal *on_collision_exit;
ENDCOMPONENT(TV_collider)

/**
 * Check for collision between the two colliders.
 * @param c1 the first collider.
 * @param c2 the second collider.
 * @return TRUE if the colliders are overlapping, FALSE if not.
 */
tvbool tv_collider_check_collision(TV_collider *c1, TV_collider *c2);
tvbool tv_collider_check_box_collision(TV_collider *box, TV_collider *c2);
tvbool tv_collider_check_sphere_collision(TV_collider *sphere, TV_collider *c2);

#ifdef __cplusplus
}
#endif
#endif