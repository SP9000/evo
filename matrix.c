#include "matrix.h"

static TvMat4x4 matrix_stack[MATRIX_STACK_SIZE*16];
static int matrix_sp = 0;

void tv_mat4x4_push(TvMat4x4 mat)
{
    matrix_stack[matrix_sp] = mat;
    ++matrix_sp;
}

TvMat4x4 tv_mat4x4_pop()
{
    --matrix_sp;
    if(matrix_sp < 0) {
        fprintf(stderr, "Error: matrix stack pointer negative\n");
    }
	return matrix_stack[matrix_sp];
}

void tv_mat4x4_load_identity(TvMat4x4 *mat)
{
	mat->a1 = mat->a2 = mat->a3 = 0.0f;
	mat->b0 = mat->b2 = mat->b3 = 0.0f;
	mat->c0 = mat->c1 = mat->c3 = 0.0f;
	mat->d0 = mat->d1 = mat->d2 = 0.0f;
	mat->a0 = mat->b1 = mat->c2 = mat->d3 = 1.0f;
}

void tv_mat4x4_perspective(TvMat4x4 *mat, float fov, float aspect, float zNear, float zFar)
{
    float f = 1.0f / (float)(tan(fov * (PI / 360.0f)));	
    mat->a0 = f / aspect;
	mat->b0 = 0.0f;
	mat->c0 = 0.0f;
	mat->d0 = 0.0f;

	mat->a1 = 0.0f;
    mat->b1 = f;
	mat->c1 = 0.0f;
	mat->d1 = 0.0;

	mat->a2 = 0.0f;
	mat->b2 = 0.0f;
    mat->c2 = (zFar + zNear) / (zNear - zFar);
    mat->d2 = -1.0f;
	
	mat->a3 = 0.0f;
	mat->b3 = 0.0f;
	mat->c3 = (2.0f * zFar * zNear) / (zNear - zFar);
    mat->d3 = 0.0f;
}

void tv_mat4x4_translate(TvMat4x4 *mat, float x, float y, float z)
{
    TvMat4x4 tmat;
	tv_mat4x4_load_identity(&tmat);
    tmat.a3 = x;
    tmat.b3 = y;
    tmat.c3 = z;
	*mat = tv_mat4x4_multiply(*mat, tmat);
}

void tv_mat4x4_scale(TvMat4x4 *mat, float x, float y, float z)
{
    mat->a0 *= x;
    mat->b1 *= y;
    mat->c2 *= z;
}

void tv_mat4x4_rotate(TvMat4x4 *mat, float angle, float x, float y, float z)
{
	TvMat4x4 r_mat;
	/* get the cosine and sine of the angle (convert angle to radians first) */
	float c = cos(angle*0.0174532925f);
	float s = sin(angle*0.0174532925f);
	r_mat.a0 = x*x * (1-c) + c;
	r_mat.a1 = x*y * (1-c) - (z*s);
	r_mat.a2 = x*z * (1-c) + (y*s);
	r_mat.a3 = 0.0f;

	r_mat.b0 = y*x*(1-c) + (z*s);
	r_mat.b1 = y*y*(1-c) + c;
	r_mat.b2 = y*z*(1-c) - x*s;
	r_mat.b3 = 0.0f;

	r_mat.c0 = x*z*(1-c) - y*s;
	r_mat.c1 = y*z*(1-c) + x*s;
	r_mat.c2 = z*z*(1-c) + c;
	r_mat.c3 = 0.0f;

	r_mat.d0 = 0.0f;
	r_mat.d1 = 0.0f;
	r_mat.d2 = 0.0f;
	r_mat.d3 = 1.0f;
	*mat = tv_mat4x4_multiply(*mat, r_mat);
}

void tv_mat4x4_orthographic(TvMat4x4 *mat, float left, float right, float top, float bottom, float nearZ, float farZ)
{
    /* TODO: ??? */
    mat->a0 = 2.0f; //2.0f / (right - left);
    mat->a1 = 0;
    mat->a2 = 0;
    mat->a3 = 0;

    mat->a1 = 0;
    mat->b1 = -2.0f; //2.0f / (top - bottom);
    mat->c1 = 0;
    mat->d1 = 0;

    mat->a2 = 0;
    mat->b2 = 0;
    mat->c2 = -2.0f / (farZ - nearZ);
    mat->d2 = 0;

    mat->a3 = -(right+left) / (right-left);
    mat->b3 = -(top+bottom) / (top-bottom);
    mat->c3 = -(farZ+nearZ) / (farZ-nearZ);
    mat->d3 = 1;
}
TvMat4x4 tv_mat4x4_multiply(TvMat4x4 mat1, TvMat4x4 mat2)
{
	TvMat4x4 dest;
	/* TODO: still think this is wrong...or something isn't matching up: see overlayrenderer for example */
	dest.a0 = mat1.a0 * mat2.a0 + mat1.a1 * mat2.b0 + mat1.a2 * mat2.c0 + mat1.a3 * mat2.d0; 
	dest.b0 = mat1.a0 * mat2.a1 + mat1.a1 * mat2.b1 + mat1.a2 * mat2.c1 + mat1.a3 * mat2.d1; 
	dest.c0 = mat1.a0 * mat2.a2 + mat1.a1 * mat2.b2 + mat1.a2 * mat2.c2 + mat1.a3 * mat2.d2; 
	dest.d0 = mat1.a0 * mat2.a3 + mat1.a1 * mat2.b3 + mat1.a2 * mat2.c3 + mat1.a3 * mat2.d3; 
	dest.a1 = mat1.b0 * mat2.a0 + mat1.b1 * mat2.b0 + mat1.b2 * mat2.c0 + mat1.b3 * mat2.d0; 
	dest.b1 = mat1.b0 * mat2.a1 + mat1.b1 * mat2.b1 + mat1.b2 * mat2.c1 + mat1.b3 * mat2.d1; 
	dest.c1 = mat1.b0 * mat2.a2 + mat1.b1 * mat2.b2 + mat1.b2 * mat2.c2 + mat1.b3 * mat2.d2; 
	dest.d1 = mat1.b0 * mat2.a3 + mat1.b1 * mat2.b3 + mat1.b2 * mat2.c3 + mat1.b3 * mat2.d3; 
	dest.a2 = mat1.b0 * mat2.a0 + mat1.c1 * mat2.b0 + mat1.c2 * mat2.c0 + mat1.c3 * mat2.d0; 
	dest.b2 = mat1.c0 * mat2.a1 + mat1.c1 * mat2.b1 + mat1.c2 * mat2.c1 + mat1.c3 * mat2.d1; 
	dest.c2 = mat1.c0 * mat2.a2 + mat1.c1 * mat2.b2 + mat1.c2 * mat2.c2 + mat1.c3 * mat2.d2; 
	dest.d2 = mat1.c0 * mat2.a3 + mat1.c1 * mat2.b3 + mat1.c2 * mat2.c3 + mat1.c3 * mat2.d3; 
	dest.a3 = mat1.d0 * mat2.a0 + mat1.d1 * mat2.b0 + mat1.d2 * mat2.c0 + mat1.d3 * mat2.d0; 
	dest.b3 = mat1.d0 * mat2.a1 + mat1.d1 * mat2.b1 + mat1.d2 * mat2.c1 + mat1.d3 * mat2.d1; 
	dest.c3 = mat1.d0 * mat2.a2 + mat1.d1 * mat2.b2 + mat1.d2 * mat2.c2 + mat1.d3 * mat2.d2; 
	dest.d3 = mat1.d0 * mat2.a3 + mat1.d1 * mat2.b3 + mat1.d2 * mat2.c3 + mat1.d3 * mat2.d3;
	return dest;
}

tv_vector4 tv_mat4x4_multiply_vec4x1(TvMat4x4 mat, tv_vector4 vec)
{
	tv_vector4 ret = {
		mat.a0 * vec.x + mat.a1 * vec.y + mat.a2 * vec.z + mat.a3 * vec.w, 
		mat.b0 * vec.x + mat.b1 * vec.y + mat.b2 * vec.z + mat.b3 * vec.w,
		mat.c0 * vec.x + mat.c1 * vec.y + mat.c2 * vec.z + mat.c3 * vec.w,
		mat.d0 * vec.x + mat.d1 * vec.y + mat.d2 * vec.z + mat.d3 * vec.w
	};
	return ret;
}

tvfloat *tv_mat4x4_to_array(TvMat4x4 *mat)
{
	return (tvfloat*)mat;
}