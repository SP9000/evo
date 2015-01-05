#include "tv_vector.h"

UT_icd ut_vector2_icd = {sizeof(tv_vector2), NULL, NULL, NULL};
UT_icd ut_vector3_icd = {sizeof(tv_vector3), NULL, NULL, NULL};
UT_icd ut_vector4_icd = {sizeof(tv_vector4), NULL, NULL, NULL};

const tv_vector2 tv_vector2_zero = {0, 0};
const tv_vector3 tv_vector3_zero = {0, 0, 0};
const tv_vector4 tv_vector4_zero = {0, 0, 0, 0};

const tv_vector3 tv_vector3_forward = {0.0f, 0.0f, 1.0f};
const tv_vector4 tv_vector4_forward = {0.0f, 0.0f, 1.0f, 1.0f};

/*****************************************************************************/
/* add */
void tv_vector2_add(tv_vector2 v1, tv_vector2 v2, tv_vector2* result)
{
	result->x = v1.x + v2.x;
	result->y = v1.y + v2.y;
}
void tv_vector3_add(tv_vector3 v1, tv_vector3 v2, tv_vector3* result)
{
	result->x = v1.x + v2.x;
	result->y = v1.y + v2.y;
	result->z = v1.z + v2.z;
}
void tv_vector4_add(tv_vector4 v1, tv_vector4 v2, tv_vector4* result)
{
	result->x = v1.x + v2.x;
	result->y = v1.y + v2.y;
	result->z = v1.z + v2.z;
	result->w = v1.w + v2.w;
}

/*****************************************************************************/
/* subtract */
void tv_vector2_sub(tv_vector2 v1, tv_vector2 v2, tv_vector2* result) 
{
	result->x = v1.x - v2.x;
	result->y = v1.y - v2.y;
}
void tv_vector3_sub(tv_vector3 v1, tv_vector3 v2, tv_vector3* result)
{
	result->x = v1.x - v2.x;
	result->y = v1.y - v2.y;
	result->z = v1.z - v2.z;
}
void tv_vector4_sub(tv_vector4 v1, tv_vector4 v2, tv_vector4* result)
{
	result->x = v1.x - v2.x;
	result->y = v1.y - v2.y;
	result->z = v1.z - v2.z;
	result->w = v1.w - v2.w;
}

/*****************************************************************************/
/* scale */
void tv_vector2_scale(tv_vector2* v, tvfloat factor)
{
	v->x *= factor;
	v->y *= factor;
}
void tv_vector3_scale(tv_vector3* v, tvfloat factor)
{
	v->x *= factor;
	v->y *= factor;
	v->z *= factor;
}
void tv_vector4_scale(tv_vector4* v, tvfloat factor)
{
	v->x *= factor;
	v->y *= factor;
	v->z *= factor;
	v->w *= factor;
}

/*****************************************************************************/
/* dot product */
tvfloat tv_vector2_dot(tv_vector2 v1, tv_vector2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
tvfloat tv_vector3_dot(tv_vector3 v1, tv_vector3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
tvfloat tv_vector4_dot(tv_vector4 v1, tv_vector4 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

/*****************************************************************************/
/* normalize */
void tv_vector2_normalize(tv_vector2 v, tv_vector2* result)
{
	tvfloat mag = tv_vector2_mag(&v);
	assert(mag != 0.0f);
	result->x = v.x / mag;
	result->y = v.y / mag;
}
void tv_vector3_normalize(tv_vector3 v, tv_vector3* result)
{
	tvfloat mag = tv_vector3_mag(&v);
	assert(mag != 0.0f);
	result->x = v.x / mag;
	result->y = v.y / mag;
	result->z = v.z / mag;
}

/*****************************************************************************/
/* magnitude */
tvfloat tv_vector2_mag(tv_vector2* v)
{
	return (tvfloat)sqrt(v->x*v->x + v->y*v->y);
}
tvfloat tv_vector3_mag(tv_vector3* v)
{
	return (tvfloat)sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}

/*****************************************************************************/
/* distance */
tvfloat tv_vector2_distance(tv_vector2 v1, tv_vector2 v2)
{
	return (tvfloat)sqrt((v2.x*v2.x)-(v1.x*v1.x) + (v2.y*v2.y)-(v1.y*v1.y));
}
tvfloat tv_vector3_distance(tv_vector3 v1, tv_vector3 v2)
{
	return (tvfloat)sqrt((v2.x*v2.x)-(v1.x*v1.x) + (v2.y*v2.y)-(v1.y*v1.y) + (v2.z*v2.z)-(v1.z*v1.z));
}
tvfloat tv_vector4_distance(tv_vector4 v1, tv_vector4 v2)
{
	return (tvfloat)sqrt((v2.x*v2.x)-(v1.x*v1.x) + (v2.y*v2.y)-(v1.y*v1.y) + (v2.z*v2.z)-(v1.z*v1.z) + (v2.w*v2.w)-(v1.w*v1.w));
}

/*****************************************************************************/
/*lerp */
tv_vector2 tv_vector2_lerp(tv_vector2 v1, tv_vector2 v2, float t)
{
	/* TODO: */
	tv_vector2 ret = {0.0f, 0.0f};
	return ret;
}
tv_vector3 tv_vector3_lerp(tv_vector3 v1, tv_vector3 v2, float t)
{
	/* TODO: */
	tv_vector3 ret = {0.0f, 0.0f, 0.0f};
	return ret;
}
tv_vector4 tv_vector4_lerp(tv_vector4 v1, tv_vector4 v2, float t)
{
	/* TODO: */
	tv_vector4 ret = {0.0f, 0.0f, 0.0f, 0.0f};
	return ret;
}

/*****************************************************************************/
/* direction */
void tv_vector3_direction(tv_vector3 v1, tv_vector3 v2, tv_vector3* result)
{
	result->x = v1.x - v2.x;
	result->y = v1.y - v2.y;
	result->z = v1.z - v2.z;
	tv_vector3_normalize(*result, result);
}

tvbool tv_rect_contains(tv_rect r, tv_vector2 point)
{
    if((point.x >= r.x) &&  (point.x <= (r.x+r.w)) &&
            (point.y >= r.y) && (point.y <= (r.y+r.h))) {
        return TRUE;
    }
    return FALSE;
}
tvbool tv_rect_overlaps(tv_rect* r1, tv_rect* r2)
{
    if((r1->x < (r2->x+r2->w)) &&
            ((r1->x+r1->w) > r2->x) &&
            (r1->y < (r2->y+r2->h)) &&
            ((r1->y+r1->h) > r2->y)) {
        return TRUE;
    }
    return FALSE;
}

