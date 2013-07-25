#include "matrix.h"

static TvMat4x4 matrix_stack[MATRIX_STACK_SIZE];
static int matrix_sp = 0;

void tv_mat4x4_push(TvMat4x4* mat)
{
    matrix_stack[matrix_sp] = *mat;
    ++matrix_sp;
}

void tv_mat4x4_pop(TvMat4x4* mat)
{
    --matrix_sp;
    if(matrix_sp < 0) {
        fprintf(stderr, "Error: matrix stack pointer negative\n");
        return;
    }
    *mat = matrix_stack[matrix_sp];
}

void tv_mat4x4_load_identity(TvMat4x4* mat)
{
    mat->a00 = mat->a11 = mat->a22 = mat->a33 = 1.0f;
    mat->a01 = mat->a02 = mat->a03 = mat->a10 = mat->a12 = mat->a13 = mat->a20 = mat->a21 = mat->a23 = mat->a30 = mat->a31 = mat->a32 = 0.0f;
}

void tv_mat4x4_perspective(TvMat4x4* mat, float fov, float aspect, float zNear, float zFar)
{
	float f;
    tv_mat4x4_load_identity(mat);
    f = 1.0f / (float)(tan(fov * (PI / 360.0f)));
    mat->a00 = f / aspect;
    mat->a11 = f;
    mat->a22 = (zFar + zNear) / (zNear - zFar);
    mat->a32 = (2.0f * zFar * zNear) / (zNear - zFar);
    mat->a23 = -1.0f;
    mat->a33 = 0.0f;
}

void tv_mat4x4_translate(TvMat4x4* mat, float x, float y, float z)
{
    TvMat4x4 tmat;
    tv_mat4x4_load_identity(&tmat);
    tmat.a30 = x;
    tmat.a31 = y;
    tmat.a32 = z;
    tv_mat4x4_load_identity(mat, &tmat);
}

void tv_mat4x4_scale(TvMat4x4* mat, float x, float y, float z)
{
    mat->a00 *= x;
    mat->a11 *= y;
    mat->a22 *= z;
}

void tv_mat4x4_rotate(TvMat4x4* mat, float angle, float x, float y, float z)
{
    /* TODO: dafuq???? */
    TvMat4x4 tMat;
    if(x != 0.0f) {
        tv_mat4x4_load_identity(&tMat);
        tMat.a11 = (float)cos(angle);
        tMat.a12 = -(float)sin(angle);
        tMat.a21 = (float)sin(angle);
        tMat.a22 = (float)cos(angle);
        tv_mat4x4_multiply(mat, &tMat);
    }
    if(y != 0.0f) {
        tv_mat4x4_load_identity(&tMat);
    }
    if(z != 0.0f) {
        tv_mat4x4_load_identity(&tMat);
    }
}

void tv_mat4x4_orthographic(TvMat4x4* mat, float left, float right, float top, float bottom, float nearZ, float farZ)
{
    /* TODO: ??? */
    mat->a00 = 2.0f / (right - left);
    mat->a01 = 0;
    mat->a02 = 0;
    mat->a03 = 0;

    mat->a10 = 0;
    mat->a11 = 2.0f / (top - bottom);
    mat->a12 = 0;
    mat->a13 = 0;

    mat->a20 = 0;
    mat->a21 = 0;
    mat->a22 = -2.0f / (farZ - nearZ);
    mat->a23 = 0;

    mat->a30 = -(right+left) / (right-left);
    mat->a31 = -(top+bottom) / (top-bottom);
    mat->a32 = -(farZ+nearZ)   / (farZ-nearZ);
    mat->a33 = 1;
}

void tv_mat4x4_multiply(TvMat4x4* A, TvMat4x4* B)
{
    TvMat4x4 C;
    C.a00 = A->a00 * B->a00 + A->a01 * B->a10 + A->a02 * B->a20 + A->a03 * B->a30;
    C.a01 = A->a00 * B->a01 + A->a01 * B->a11 + A->a02 * B->a21 + A->a03 * B->a31;
    C.a02 = A->a00 * B->a02 + A->a01 * B->a12 + A->a02 * B->a22 + A->a03 * B->a32;
    C.a03 = A->a00 * B->a03 + A->a01 * B->a13 + A->a02 * B->a23 + A->a03 * B->a33;
    C.a10 = A->a10 * B->a00 + A->a11 * B->a10 + A->a12 * B->a20 + A->a13 * B->a30;
    C.a11 = A->a10 * B->a01 + A->a11 * B->a11 + A->a12 * B->a21 + A->a13 * B->a31;
    C.a12 = A->a10 * B->a02 + A->a11 * B->a12 + A->a12 * B->a22 + A->a13 * B->a32;
    C.a13 = A->a10 * B->a03 + A->a11 * B->a13 + A->a12 * B->a23 + A->a13 * B->a33;
    C.a20 = A->a20 * B->a00 + A->a21 * B->a10 + A->a22 * B->a20 + A->a23 * B->a30;
    C.a21 = A->a20 * B->a01 + A->a21 * B->a11 + A->a22 * B->a21 + A->a23 * B->a31;
    C.a22 = A->a20 * B->a02 + A->a21 * B->a12 + A->a22 * B->a22 + A->a23 * B->a32;
    C.a23 = A->a20 * B->a03 + A->a21 * B->a13 + A->a22 * B->a23 + A->a23 * B->a33;
    C.a30 = A->a30 * B->a00 + A->a31 * B->a10 + A->a32 * B->a20 + A->a33 * B->a30;
    C.a31 = A->a30 * B->a01 + A->a31 * B->a11 + A->a32 * B->a21 + A->a33 * B->a31;
    C.a32 = A->a30 * B->a02 + A->a31 * B->a12 + A->a32 * B->a22 + A->a33 * B->a32;
    C.a33 = A->a30 * B->a03 + A->a31 * B->a13 + A->a32 * B->a23 + A->a33 * B->a33;
    *A = C;
}

#ifndef JANKY
float* Mat4x4Pack(TvMat4x4* mat)
{
    mat->packed[0] = mat->a00;
    mat->packed[1] = mat->a01;
    mat->packed[2] = mat->a02;
    mat->packed[3] = mat->a03;

    mat->packed[4] = mat->a10;
    mat->packed[5] = mat->a11;
    mat->packed[6] = mat->a12;
    mat->packed[7] = mat->a13;

    mat->packed[8] = mat->a20;
    mat->packed[9] = mat->a21;
    mat->packed[10] = mat->a22;
    mat->packed[11] = mat->a23;

    mat->packed[12] = mat->a30;
    mat->packed[13] = mat->a31;
    mat->packed[14] = mat->a32;
    mat->packed[15] = mat->a33;
    return mat->packed;
}
#endif
