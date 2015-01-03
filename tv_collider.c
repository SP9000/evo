#include "tv_collider.h"
#include "tv_collision.h"

COMPONENT_NEW(TV_collider, tv_component)
END_COMPONENT_NEW(TV_collider)

COMPONENT_DESTROY(TV_collider)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(TV_collider)
END_COMPONENT_UPDATE

COMPONENT_START(TV_collider)
	tv_collision_register_collider(self);
END_COMPONENT_START

/*****************************************************************************/
tvbool tv_collider_check_collision(TV_collider *c1, TV_collider *c2)
{
	switch(c1->type) {
	case TV_COLLIDER_BOX:
		return tv_collider_check_box_collision(c1, c2);		
	case TV_COLLIDER_SPHERE:
		return tv_collider_check_sphere_collision(c1, c2);
	case TV_COLLIDER_LINE:
		return tv_collider_check_line_collision(c1, c2);
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
	/* origin of line */
	tv_vector3 o = line->info.line.start;
	/* center of sphere */
	tv_vector3 c = ((tv_component*)c2)->transform->pos;
	/* unit vector direction of line */
	tv_vector3 dir = line->info.line.dir;

	tvfloat a, b;
	tv_vector3 o_minus_c = tv_vector3_sub(o, c);

	switch(c2->type) {
	case TV_COLLIDER_BOX: 
		/* TODO: */	
		return FALSE;
	case TV_COLLIDER_SPHERE:
		a = tv_vector3_mag(o_minus_c);
		a *= a;
		b = tv_vector3_dot(dir, o_minus_c);
		b *= b;
		return (b - a + (c2->info.sphere.radius*c2->info.sphere.radius)) >= 0.0f;
	case TV_COLLIDER_LINE:
		/* TODO */
		return FALSE;
	}
	return FALSE;
}

void tv_collider_sphere(TV_collider *col, tvfloat radius)
{
	col->type = TV_COLLIDER_SPHERE;
	col->info.sphere.radius = radius;
}
void tv_collider_line(TV_collider *col, tv_vector3 start, tv_vector3 dir, tvfloat len)
{
	col->type = TV_COLLIDER_LINE;
	col->info.line.start = start;
	col->info.line.dir = dir;
	col->info.line.len = len;
}