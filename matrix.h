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

typedef float Mat4x4[16];

void Mat4x4Push(Mat4x4 mat);
void Mat4x4Pop(Mat4x4 mat);

void Mat4x4LoadIdentity(Mat4x4 mat);
void Mat4x4Scale(Mat4x4 mat, float x, float y, float z);
void Mat4x4Translate(Mat4x4 mat, float x, float y, float z);
void Mat4x4Multiply(Mat4x4 mat1, Mat4x4 mat2);
void Mat4x4PerspMat(Mat4x4 mat, float fov, float aspect, float zNear, float zFar);
void Mat4x4OrthoMat(Mat4x4 mat, float left, float right, float top, float bottom, float nearZ, float farZ);

#ifdef __cplusplus 
}
#endif
#endif
