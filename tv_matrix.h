/*****************************************************************************/
/* matrix.h                                                                  */
/* This file contains definitions for the matrix type(s) as well as various  */
/* functions to manipulate them.                                             */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created April, 2013                                                       */
/*****************************************************************************/
#ifndef _MATRIX_H
#define _MATRIX_H
#ifdef __cplusplus 
extern "C" {
#endif

#define PI 3.1415926536
#define MATRIX_STACK_SIZE 256

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "tv_vector.h"

#ifdef _WIN32
#pragma pack(4)
#endif
typedef struct TvMat4x4 {
	tvfloat a0, b0, c0, d0;
	tvfloat a1, b1, c1, d1;
	tvfloat a2, b2, c2, d2;
	tvfloat a3, b3, c3, d3;
}TvMat4x4;

void tv_mat4x4_push(TvMat4x4 mat);
TvMat4x4 tv_mat4x4_pop();

void tv_mat4x4_load_identity(TvMat4x4 *mat);
void tv_mat4x4_scale(TvMat4x4 *mat, float x, float y, float z);
void tv_mat4x4_translate(TvMat4x4 *mat, float x, float y, float z);
void tv_mat4x4_rotate(TvMat4x4 *mat, float angle, float x, float y, float z);

tvfloat tv_mat4x4_det(TvMat4x4 mat);
tvbool tv_mat4x4_inverse(TvMat4x4 mat, TvMat4x4 *out);

TvMat4x4 tv_mat4x4_multiply(TvMat4x4 mat1, TvMat4x4 mat2);
tv_vector4 tv_mat4x4_multiply_vec4x1(TvMat4x4 mat, tv_vector4 vec);
tv_vector3 tv_mat4x4_multiply_vec3x1(TvMat4x4 mat, tv_vector3 vec);
void tv_mat4x4_perspective(TvMat4x4 *mat, float fov, float aspect, float zNear, float zFar);
void tv_mat4x4_orthographic(TvMat4x4 *mat, float left, float right, float top, float bottom, float nearZ, float farZ);

tvfloat *tv_mat4x4_to_array(TvMat4x4 *mat);

#ifdef __cplusplus 
}
#endif
#endif
