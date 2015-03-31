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


tvbool tv_collider_check_box_collision(TV_collider *box, TV_collider *c2, tv_collision* c);
tvbool tv_collider_check_sphere_collision(TV_collider *sphere, TV_collider *c2, tv_collision* c);
tvbool tv_collider_check_line_collision(TV_collider *line, TV_collider *c2, tv_collision* c);
tvbool tv_collider_check_quad_collision(TV_collider *quad, TV_collider *c2, tv_collision* c);

/*****************************************************************************/
tvbool tv_collider_check_collision(TV_collider *c1, TV_collider *c2, tv_collision* c)
{
	switch(c1->type) {
	case TV_COLLIDER_BOX:
		return tv_collider_check_box_collision(c1, c2, c);		
	case TV_COLLIDER_SPHERE:
		return tv_collider_check_sphere_collision(c1, c2, c);
	case TV_COLLIDER_LINE:
		return tv_collider_check_line_collision(c1, c2, c);
	}
	return FALSE;
}

tvbool tv_collider_check_box_collision(TV_collider *box, TV_collider *c2, tv_collision* c)
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

tvbool tv_collider_check_sphere_collision(TV_collider *sphere, TV_collider *c2, tv_collision* c)
{
	switch(c2->type) {
	case TV_COLLIDER_BOX: 
		/* TODO: */	
		return FALSE;
	case TV_COLLIDER_SPHERE:
		if(tv_vector3_distance(sphere->pos, c2->pos) < 
			(sphere->info.sphere.radius + c2->info.sphere.radius)) {
				/* TODO: location of intersection not found */
				c->col1 = ((tv_component*)sphere)->entity;
				c->col2 = ((tv_component*)c2)->entity;
				return TRUE;
		}
	}
	return FALSE;
}

/*****************************************************************************/
/* line <-> triangle */
#define triangle_to_line(tri, c2, collision ) line_to_triangle( c2, tri, collision );
tvbool line_to_triangle(TV_collider *line, TV_collider *c2, tv_collision* collision)
{
	tv_vector3 e1, e2, h, s, q;
	tvfloat a, f, u, v, t;
	tv_vector3 dir = line->info.line.dir;
	tv_vector3 o = line->info.line.start;

	tv_vector3 v0 = c2->info.triangle.v0;
	tv_vector3 v1 = c2->info.triangle.v1;
	tv_vector3 v2 = c2->info.triangle.v2;

	tv_vector3_sub(v1, v0, &e1);
	tv_vector3_sub(v2, v0, &e2);

	tv_vector3_cross(&dir, &e2, &h);
	a = tv_vector3_dot(e1, h);

	if(a > -0.00001 && a < 0.00001) {
		return FALSE;
	}
	f = 1.0f / a;
	tv_vector3_sub(o, v0, &s);
	u = f * tv_vector3_dot(s, h);

	if(u < 0.0f || u > 1.0f) {
		return FALSE;
	}

	tv_vector3_cross(&s, &e1, &q);
	v = f * tv_vector3_dot(dir, q);

	if(v < 0.0f || u + v > 1.0f) {
		return FALSE;
	}

	/* a line intersection has occurred - find out where */
	t = f * tv_vector3_dot(e2, q);
	if(t > 0.00001) {
		/* follow the line's direction for "t" length to find intersection */
		tv_vector3_normalize(dir, &dir);
		tv_vector3_scale(&dir, t);
		tv_vector3_add(line->info.line.start, dir, &(collision->location));
		collision->col1 = ((tv_component*)line)->entity;
		collision->col2 = ((tv_component*)c2)->entity;
		return TRUE;
	}
	return FALSE;
}
/*****************************************************************************/
/* line <-> quad */
tvbool line_to_quad(TV_collider *line, TV_collider *c2, tv_collision* collision)
{
	tvfloat w = c2->info.quad.w;
	tvfloat h = c2->info.quad.h;

	/* temporary triangle collider */
	TV_collider t0;

	/* TODO: apply transformations (rot/scale/trans) to the quad */
	tv_vector3 ll_corner, lr_corner, ul_corner, ur_corner;

	ll_corner = c2->base.transform->pos;
	tv_vector3_add(ll_corner, tv_vector3_new(w,0,0), &lr_corner); 
	tv_vector3_add(ll_corner, tv_vector3_new(0,0,-h), &ul_corner);
	tv_vector3_add(ll_corner, tv_vector3_new(w,0,-h), &ur_corner);

	/* decompose plane into triangles and test those for intersection */
	/* TODO: obviously this is far from the best way */
	t0 = *c2;	/* copy our quad colliders attributes (entity, etc.) */
	tv_collider_triangle(&t0, ul_corner, lr_corner, ll_corner);

	/* check the first triangle */
	if(line_to_triangle(line, &t0, collision)) {
		return TRUE;
	}
	/* check the second triangle */
	tv_collider_triangle(&t0, ul_corner, ur_corner, lr_corner);
	return line_to_triangle(line, &t0, collision);
}

tvbool tv_collider_check_line_collision(TV_collider *line, TV_collider *c2, tv_collision* collision) 
{
	/* origin of line */
	tv_vector3 o = line->info.line.start;
	/* center of sphere */
	tv_vector3 c = ((tv_component*)c2)->transform->pos;
	/* unit vector direction of line */
	tv_vector3 dir = line->info.line.dir;

	tvfloat a, b;
	tv_vector3 o_minus_c;
	
	tv_vector3_sub(o, c, &o_minus_c);

	switch(c2->type) {
	case TV_COLLIDER_BOX: 
		/* TODO: */	
		return FALSE;
	case TV_COLLIDER_SPHERE:
		a = tv_vector3_mag(&o_minus_c);
		a *= a;
		b = tv_vector3_dot(dir, o_minus_c);
		b *= b;
		return (b - a + (c2->info.sphere.radius*c2->info.sphere.radius)) >= 0.0f;
	case TV_COLLIDER_LINE:
		/* TODO */
		return FALSE;
	case TV_COLLIDER_QUAD:
		puts("testing a quad\n");
		return line_to_quad(line, c2, collision);
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
void tv_collider_triangle(TV_collider* col, tv_vector3 v0, tv_vector3 v1, tv_vector3 v2)
{
	col->type = TV_COLLIDER_TRIANGLE;
	col->info.triangle.v0 = v0;
	col->info.triangle.v1 = v1;
	col->info.triangle.v2 = v2;
}
void tv_collider_quad(TV_collider* col, tv_vector2 dimensions)
{
	col->type = TV_COLLIDER_QUAD;
	col->info.quad.w = dimensions.x;
	col->info.quad.h = dimensions.y;
}
