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

typedef float TvMat4x4[16];

void tv_mat4x4_push(TvMat4x4 mat);
void tv_mat4x4_pop(TvMat4x4 mat);

void tv_mat4x4_load_identity(TvMat4x4 mat);
void tv_mat4x4_scale(TvMat4x4 mat, float x, float y, float z);
void tv_mat4x4_translate(TvMat4x4 mat, float x, float y, float z);
void tv_mat4x4_rotate(TvMat4x4 mat, float angle, float x, float y, float z);
void tv_mat4x4_multiply(TvMat4x4 mat1, TvMat4x4 mat2);
void tv_mat4x4_perspective(TvMat4x4 mat, float fov, float aspect, float zNear, float zFar);
void tv_mat4x4_orthographic(TvMat4x4 mat, float left, float right, float top, float bottom, float nearZ, float farZ);

#ifdef __cplusplus 
}
#endif
#endif
