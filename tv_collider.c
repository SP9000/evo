#include "tv_collider.h"


COMPONENT_NEW(TV_collider, tv_component)
END_COMPONENT_NEW(TV_collider)

COMPONENT_DESTROY(TV_collider)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(TV_collider)
END_COMPONENT_UPDATE

COMPONENT_START(TV_collider)
END_COMPONENT_START

/*****************************************************************************/
tvbool tv_collider_check_collision(TV_collider *c1, TV_collider *c2)
{
	switch(c1->type) {
	case TV_COLLIDER_BOX:
		return tv_collider_check_box_collision(c1, c2);		
	case TV_COLLIDER_SPHERE:
		return tv_collider_check_sphere_collision(c1, c2);
	}
}

tvbool tv_collider_check_box_collision(TV_collider *box, TV_collider *c2)
{
	switch(c2->type) {
	case TV_COLLIDER_BOX:
		/* TODO: */
		return FALSE;
	case TV_COLLIDER_SPHERE:
		/* TODO: */
		return FALSE;
	}
}

tvbool tv_collider_check_sphere_collision(TV_collider *sphere, TV_collider *c2)
{
	switch(c2->type) {
	case TV_COLLIDER_BOX: 
		/* TODO: */	
		return FALSE;
	case TV_COLLIDER_SPHERE:
		return (tv_vector3_distance(sphere->pos, c2->pos) < 
			(sphere->info.sphere.radius + c2->info.sphere.radius));
	}
	return FALSE;
}

tvbool tv_collider_check_line_collision(TV_collider *line, TV_collider *c2) 
{
	switch(c2->type) {
	case TV_COLLIDER_BOX: 
		/* TODO: */	
		return FALSE;
	case TV_COLLIDER_SPHERE:
		/* TODO: */
		return FALSE;
	}
	return FALSE;
}
