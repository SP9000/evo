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
	TV_COLLIDER_LINE,
	TV_COLLIDER_QUAD,
	TV_COLLIDER_TRIANGLE
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
 * A structure containing information needed to detect line collision.
 */
typedef struct {
	tv_vector3 start;
	tv_vector3 dir;
	tvfloat len;
}TV_collider_line_collider_info;

/**
 * A structure containing information needed to detect quad collision.
 */
typedef struct {
	tvfloat w, h;
}TV_collider_quad_collider_info;

/**
 * A structure containing information needed to detect triangle collision.
 */
typedef struct {
	tv_vector3 v0;
	tv_vector3 v1;
	tv_vector3 v2;
}TV_collider_triangle_collider_info;

/**
 * A structure containing information needed to detect collider collision.
 */
typedef union TV_collider_info {
	TV_collider_box_collider_info box;
	TV_collider_sphere_collider_info sphere;
	TV_collider_line_collider_info line;
	TV_collider_quad_collider_info quad;
	TV_collider_triangle_collider_info triangle;
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
 * The msesage that is sent by collision signals.
 */
typedef struct TV_collision_message {
	/* the collider that triggered the  */
	TV_collider *into;
}TV_collision_message;

void tv_collider_sphere(TV_collider *col, tvfloat radius);
void tv_collider_line(TV_collider *col, tv_vector3 start, tv_vector3 dir, tvfloat len);
void tv_collider_triangle(TV_collider* col, tv_vector3 v0, tv_vector3 v1, tv_vector3 v2); 
void tv_collider_quad(TV_collider* col, tv_vector2 dimensions);

#ifdef __cplusplus
}
#endif
#endif