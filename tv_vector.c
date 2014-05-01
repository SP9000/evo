#include "tv_vector.h"

const tv_vector2 tv_vector2_zero = {0, 0};
const tv_vector3 tv_vector3_zero = {0, 0, 0};
const tv_vector4 tv_vector4_zero = {0, 0, 0, 0};

tv_vector2 tv_vector2_add(tv_vector2 v1, tv_vector2 v2)
{
	tv_vector2 ret = {v1.x + v2.x, v1.x + v2.y};
	return ret;
}
tv_vector3 tv_vector3_add(tv_vector3 v1, tv_vector3 v2)
{
	tv_vector3 ret = {v1.x + v2.x, v1.x + v2.y, v1.z + v2.z};
	return ret;
}

tv_vector4 tv_vector4_add(tv_vector4 v1, tv_vector4 v2)
{
	tv_vector4 ret = {v1.w + v2.w, v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	return ret;
}

tvfloat tv_vector2_distance(tv_vector2 v1, tv_vector2 v2)
{

}
tvfloat tv_vector3_distance(tv_vector3 v1, tv_vector3 v2)
{
	return (tvfloat)sqrt((v2.x*v2.x)-(v1.x*v1.x) + (v2.y*v2.y)-(v1.y*v1.y) + (v2.z*v2.z)-(v1.z*v1.z));
}
tvfloat tv_vector4_distance(tv_vector4 v1, tv_vector4 v2)
{

}


tv_vector2 tv_vector2_lerp(tv_vector2 v1, tv_vector2 v2, float t)
{
}

tv_vector3 tv_vector3_lerp(tv_vector3 v1, tv_vector3 v2, float t)
{
}


int tv_rect_contains(tv_rect* r, tv_vector2* point)
{
    if((point->x >= r->x) &&  (point->x <= (r->x+r->w)) &&
            (point->x >= r->y) && (point->y <= (r->y+r->h))) {
        return 1;
    }
    return 0;
}

int tv_rect_overlaps(tv_rect* r1, tv_rect* r2)
{
    if((r1->x < (r2->x+r2->w)) &&
            ((r1->x+r1->w) > r2->x) &&
            (r1->y < (r2->y+r2->h)) &&
            ((r1->y+r1->h) > r2->y)) {
        return 1;
    }
    return 0;
}

