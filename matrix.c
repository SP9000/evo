#include "matrix.h"

void Mat4x4LoadIdentity(Mat4x4 mat)
{
    int i;
    for(i = 0; i < 16; ++i) 
        mat[i] = 0.0f;
    for(i = 0; i < 4; i++) 
        mat[i + (i * 4)] = 1.0f;
}

void Mat4x4PerspMat(Mat4x4 mat, float fov, float aspect, float zNear, float zFar)
{
    Mat4x4LoadIdentity(mat);
    float f = 1.0f / (tan(fov * (PI / 360.0f)));
    mat[0] = f / aspect;
    mat[5] = f;
    mat[10] = (zFar + zNear) / (zNear - zFar);
    mat[14] = (2.0f * zFar * zNear) / (zNear - zFar);
    mat[11] = -1.0f;
    mat[15] = 0.0f;
}

void Mat4x4Translate(Mat4x4 mat, float x, float y, float z)
{
    Mat4x4 tmat;
    Mat4x4LoadIdentity(tmat);
    tmat[12] = x;
    tmat[13] = y;
    tmat[14] = z;
    Mat4x4Multiply(mat, tmat);
}

void Mat4x4Rotate(Mat4x4 mat, float angle, float x, float y, float z)
{
    Mat4x4 tMat;
    if(x != 0.0f) {
        Mat4x4LoadIdentity(tMat);
        tMat[5] = cos(angle);
        tMat[6] = -sin(angle);
        tMat[9] = sin(angle);
        tMat[10] = cos(angle);
        Mat4x4Multiply(mat, tMat);
    }
    if(y != 0.0f) {
        Mat4x4LoadIdentity(tMat);
    }
    if(z != 0.0f) {
        Mat4x4LoadIdentity(tMat);
    }
}

void Mat4x4OrthoMat(Mat4x4 mat, float left, float right, float top, float bottom, float near, float far)
{
    /* TODO: ??? */
    mat[0] = 2.0 / (right - left);
    mat[1] = 0;
    mat[2] = 0;
    mat[3] = 0;

    mat[4] = 0;
    mat[5] = 2.0f / (top - bottom);
    mat[6] = 0;
    mat[7] = 0;

    mat[8] = 0;
    mat[9] = 0;
    mat[10] = -2.0f / (far - near);
    mat[11] = 0;

    mat[12] = -(right+left) / (right-left);
    mat[13] = -(top+bottom) / (top-bottom);
    mat[14] = -(far+near)   / (far-near);
    mat[15] = 1;
}

void Mat4x4Multiply(Mat4x4 mat, Mat4x4 mat2)
{
    int i, j, k;
    float sum;

    for(i = 0; i < 4; ++i) {
        for(j = 0; j < 4; ++j) {
            sum = 0.0f;
            for(k = 0; k < 4; ++k) {
                sum += mat[i*4+k] * mat2[k*4+j];
            }
            mat[i*4 + j] = sum;
        }
    }
}
