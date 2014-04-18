#ifndef TV_VECTOR_H
#define TV_VECTOR_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * A basic type for representing a point in 2-dimensional space.
 */
typedef struct tv_vector2 {
    float x, y;
}tv_vector2;

/**
 * A basic type for representing a point in 3-dimensional space.
 */
typedef struct tv_vector3 {
    float x, y, z;
}tv_vector3;

/**
 * A basic type for representing a point in 4-dimensional space
 */
typedef struct tv_vector4 {
	float w, x, y, z;
}tv_vector4;
typedef tv_vector4 tv_quaternion;

extern const tv_vector2 tv_vector2_zero;
extern const tv_vector3 tv_vector3_zero;
extern const tv_vector4 tv_vector4_zero;


tv_vector2 tv_vector2_add(tv_vector2 v1, tv_vector2 v2);
tv_vector3 tv_vector3_add(tv_vector3 v1, tv_vector3 v2);
tv_vector4 tv_vector4_add(tv_vector4 v1, tv_vector4 v2);

/**
 * Linearly interpolates between the given vectors.
 * @param v1 the first vector to interpolate from.
 * @param v2 the vector to interpolate to.
 * @param t the time (0-1)
 * @return the new vector 
 */
tv_vector2 tv_vector2_lerp(tv_vector2 v1, tv_vector2 v2, float t);

/**
 * Linearly interpolates between the given 3-D vectors.
 * @param v1 the "start" vector.
 * @param v2 the "destination" vector.
 * @param t the time (0-1)
 * @return the new vector
 */
tv_vector3 tv_vector3_lerp(tv_vector3 v1, tv_vector3 v2, float t);

/**
 * Linearly interpolates between the given 4-D vectors.
 * @param v1 the "start" vector.
 * @param v2 the "destination" vector.
 * @param t the time (0-1)
 * @return the new vector
 */
tv_vector4 tv_vector4_lerp(tv_vector4 v1, tv_vector4 v2, float t);

#ifdef __cplusplus
}
#endif
#endif