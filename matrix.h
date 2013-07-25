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

#define JANKY

#ifdef JANKY
#define Mat4x4Pack(M) \
    (float*)M
#endif

#define PI 3.1415926536
#define MATRIX_STACK_SIZE 256

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct tagMat4x4 {
    float a00;
    float a01;
    float a02;
    float a03;
    float a10;
    float a11;
    float a12;
    float a13;
    float a20;
    float a21;
    float a22;
    float a23;
    float a30;
    float a31;
    float a32;
    float a33;
#ifndef JANKY
    float packed[16];
#endif
}TvMat4x4;

void tv_mat4x4_push(TvMat4x4* mat);
void tv_mat4x4_pop(TvMat4x4* mat);

void tv_mat4x4_load_identity(TvMat4x4* mat);
void tv_mat4x4_scale(TvMat4x4* mat, float x, float y, float z);
void tv_mat4x4_translate(TvMat4x4* mat, float x, float y, float z);
void tv_mat4x4_rotate(TvMat4x4* mat, float angle, float x, float y, float z);
void tv_mat4x4_multiply(TvMat4x4* mat1, TvMat4x4* mat2);
void tv_mat4x4_perspective(TvMat4x4* mat, float fov, float aspect, float zNear, float zFar);
void tv_mat4x4_orthographic(TvMat4x4* mat, float left, float right, float top, float bottom, float nearZ, float farZ);

#ifndef JANKY
float* Mat4x4Pack(TvMat4x4* mat);
#endif

#ifdef __cplusplus 
}
#endif
#endif
