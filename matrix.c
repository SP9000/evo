#include "matrix.h"

static float matrix_stack[MATRIX_STACK_SIZE*16];
static int matrix_sp = 0;

void tv_mat4x4_push(TvMat4x4 mat)
{
    int i;
    int offset = matrix_sp * 16;
    for(i = 0; i < 16; ++i) {
        matrix_stack[offset+i] = mat[i];
    }
    ++matrix_sp;
}

void tv_mat4x4_pop(TvMat4x4 mat)
{
    int i;
	int offset;

    --matrix_sp;
    if(matrix_sp < 0) {
        fprintf(stderr, "Error: matrix stack pointer negative\n");
        return;
    }
    offset = matrix_sp * 16;
    for(i = 0; i < 16; ++i) {
        mat[i] = matrix_stack[offset+i];
    }

}

void tv_mat4x4_load_identity(TvMat4x4 mat)
{
    int i;
    for(i = 0; i < 16; ++i) 
        mat[i] = 0.0f;
    for(i = 0; i < 4; i++) 
        mat[i + (i * 4)] = 1.0f;
}

void tv_mat4x4_perspective(TvMat4x4 mat, float fov, float aspect, float zNear, float zFar)
{
    float f = 1.0f / (float)(tan(fov * (PI / 360.0f)));
	
	tv_mat4x4_load_identity(mat);
    mat[0] = f / aspect;
    mat[5] = f;
    mat[10] = (zFar + zNear) / (zNear - zFar);
    mat[14] = (2.0f * zFar * zNear) / (zNear - zFar);
    mat[11] = -1.0f;
    mat[15] = 0.0f;
}

void tv_mat4x4_translate(TvMat4x4 mat, float x, float y, float z)
{
    TvMat4x4 tmat;
	tv_mat4x4_load_identity(tmat);
    tmat[12] = x;
    tmat[13] = y;
    tmat[14] = z;
	tv_mat4x4_multiply(mat, tmat);

}

void tv_mat4x4_scale(TvMat4x4 mat, float x, float y, float z)
{
    mat[0] *= x;
    mat[5] *= y;
    mat[10] *= z;
}

void tv_mat4x4_rotate(TvMat4x4 mat, float angle, float x, float y, float z)
{
    TvMat4x4 tMat;
	TvMat4x4 r_mat;

	/* get the cosine and sine of the angle (convert angle to radians first) */
	float c = cos(angle*0.0174532925f);
	float s = sin(angle*0.0174532925f);
	tv_mat4x4_load_identity(r_mat);
	r_mat[0] = x*y*(1-c) + c;
	r_mat[1] = x*y*(1-c);
	r_mat[2] = y*s;
	r_mat[4] = y*x*(1-c);
	r_mat[5] = y*y*(1-c) + c;
	r_mat[6] = -x * s;
	r_mat[8] = -y * s;
	r_mat[9] = x * s;
	r_mat[10] = c;
	tv_mat4x4_multiply(mat, r_mat);
}

void tv_mat4x4_orthographic(TvMat4x4 mat, float left, float right, float top, float bottom, float nearZ, float farZ)
{
    /* TODO: ??? */
    mat[0] = 2.0f / (right - left);
    mat[1] = 0;
    mat[2] = 0;
    mat[3] = 0;

    mat[4] = 0;
    mat[5] = 2.0f / (top - bottom);
    mat[6] = 0;
    mat[7] = 0;

    mat[8] = 0;
    mat[9] = 0;
    mat[10] = -2.0f / (farZ - nearZ);
    mat[11] = 0;

    mat[12] = -(right+left) / (right-left);
    mat[13] = -(top+bottom) / (top-bottom);
    mat[14] = -(farZ+nearZ)   / (farZ-nearZ);
    mat[15] = 1;
}

/* TODO: did I seriously modify as I went? */
void tv_mat4x4_multiply(TvMat4x4 mat1, TvMat4x4 mat2)
{
    int i, j, k;
    float sum;

    for(i = 0; i < 4; ++i) {
        for(j = 0; j < 4; ++j) {
            sum = 0.0f;
            for(k = 0; k < 4; ++k) {
                sum += mat1[i*4+k] * mat2[k*4+j];
            }
            mat1[i*4 + j] = sum;
        }
    }
}
