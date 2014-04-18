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


tv_vector2 tv_vector2_lerp(tv_vector2 v1, tv_vector2 v2, float t)
{
}

tv_vector3 tv_vector3_lerp(tv_vector3 v1, tv_vector3 v2, float t)
{
}