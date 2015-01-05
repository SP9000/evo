#include "tv_matrix.h"

static TvMat4x4 matrix_stack[MATRIX_STACK_SIZE*16];
static int matrix_sp = 0;

void tv_mat4x4_push(TvMat4x4* mat)
{
	memcpy(&matrix_stack[matrix_sp], mat, sizeof(TvMat4x4));
    ++matrix_sp;
}

void tv_mat4x4_pop(TvMat4x4* mat)
{
    --matrix_sp;
    if(matrix_sp < 0) {
        tv_error("Error: matrix stack pointer negative\n");
    }
	memcpy(mat, &matrix_stack[matrix_sp], sizeof(TvMat4x4));
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
    mat->b0 = 0;
    mat->c0 = 0;
    mat->d0 = 0;

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
	TvMat4x4 res;
	/* TODO: still think this is wrong...or something isn't matching up: see overlayrenderer for example */
	res.a0 = mat1.a0 * mat2.a0 + mat1.a1 * mat2.b0 + mat1.a2 * mat2.c0 + mat1.a3 * mat2.d0; 
	res.a1 = mat1.a0 * mat2.a1 + mat1.a1 * mat2.b1 + mat1.a2 * mat2.c1 + mat1.a3 * mat2.d1; 
	res.a2 = mat1.a0 * mat2.a2 + mat1.a1 * mat2.b2 + mat1.a2 * mat2.c2 + mat1.a3 * mat2.d2; 
	res.a3 = mat1.a0 * mat2.a3 + mat1.a1 * mat2.b3 + mat1.a2 * mat2.c3 + mat1.a3 * mat2.d3; 
	res.b0 = mat1.b0 * mat2.a0 + mat1.b1 * mat2.b0 + mat1.b2 * mat2.c0 + mat1.b3 * mat2.d0; 
	res.b1 = mat1.b0 * mat2.a1 + mat1.b1 * mat2.b1 + mat1.b2 * mat2.c1 + mat1.b3 * mat2.d1; 
	res.b2 = mat1.b0 * mat2.a2 + mat1.b1 * mat2.b2 + mat1.b2 * mat2.c2 + mat1.b3 * mat2.d2; 
	res.b3 = mat1.b0 * mat2.a3 + mat1.b1 * mat2.b3 + mat1.b2 * mat2.c3 + mat1.b3 * mat2.d3; 
	res.c0 = mat1.b0 * mat2.a0 + mat1.c1 * mat2.b0 + mat1.c2 * mat2.c0 + mat1.c3 * mat2.d0; 
	res.c1 = mat1.c0 * mat2.a1 + mat1.c1 * mat2.b1 + mat1.c2 * mat2.c1 + mat1.c3 * mat2.d1; 
	res.c2 = mat1.c0 * mat2.a2 + mat1.c1 * mat2.b2 + mat1.c2 * mat2.c2 + mat1.c3 * mat2.d2; 
	res.c3 = mat1.c0 * mat2.a3 + mat1.c1 * mat2.b3 + mat1.c2 * mat2.c3 + mat1.c3 * mat2.d3; 
	res.d0 = mat1.d0 * mat2.a0 + mat1.d1 * mat2.b0 + mat1.d2 * mat2.c0 + mat1.d3 * mat2.d0; 
	res.d1 = mat1.d0 * mat2.a1 + mat1.d1 * mat2.b1 + mat1.d2 * mat2.c1 + mat1.d3 * mat2.d1; 
	res.d2 = mat1.d0 * mat2.a2 + mat1.d1 * mat2.b2 + mat1.d2 * mat2.c2 + mat1.d3 * mat2.d2; 
	res.d3 = mat1.d0 * mat2.a3 + mat1.d1 * mat2.b3 + mat1.d2 * mat2.c3 + mat1.d3 * mat2.d3;
	return res;
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

tv_vector3 tv_mat4x4_multiply_vec3x1(TvMat4x4 mat, tv_vector3 vec)
{
	tv_vector3 ret;
	tv_vector4 v = {vec.x, vec.y, vec.z, 1.0f};
	v = tv_mat4x4_multiply_vec4x1(mat, v);
	ret.x = v.x;
	ret.y = v.y;
	ret.z = v.z;
	return ret;
}

tvfloat *tv_mat4x4_to_array(TvMat4x4 *mat)
{
	return (tvfloat*)mat;
}

tvfloat tv_mat4x4_det(TvMat4x4* mat)
{
	TvMat4x4 inv;
	tvfloat det;
    inv.a0 =  mat->b1 * mat->c2 * mat->d3 - mat->b1 * mat->d2 * mat->c3 - mat->b2 * mat->c1 * mat->d3 + mat->b2 * mat->d1 * mat->c3 + mat->b3 * mat->c1 * mat->d2 - mat->b3 * mat->d1 * mat->c2;
    inv.b0 = -mat->a1 * mat->c2 * mat->d3 + mat->a1 * mat->d2 * mat->c3 + mat->a2 * mat->c1 * mat->d3 - mat->a2 * mat->d1 * mat->c3 - mat->a3 * mat->c1 * mat->d2 + mat->a3 * mat->d1 * mat->c2;
    inv.c0 =  mat->a1 * mat->b2 * mat->d3 - mat->a1 * mat->d2 * mat->b3 - mat->a2 * mat->b1 * mat->d3 + mat->a2 * mat->d1 * mat->b3 + mat->a3 * mat->b1 * mat->d2 - mat->a3 * mat->d1 * mat->b2;
    inv.d0 = -mat->a1 * mat->b2 * mat->c3 + mat->a1 * mat->c2 * mat->b3 + mat->a2 * mat->b1 * mat->c3 - mat->a2 * mat->c1 * mat->b3 - mat->a3 * mat->b1 * mat->c2 + mat->a3 * mat->c1 * mat->b2;
    inv.a1 = -mat->b0 * mat->c2 * mat->d3 + mat->b0 * mat->d2 * mat->c3 + mat->b2 * mat->c0 * mat->d3 - mat->b2 * mat->d0 * mat->c3 - mat->b3 * mat->c0 * mat->d2 + mat->b3 * mat->d0 * mat->c2;
    inv.b1 =  mat->a0 * mat->c2 * mat->d3 - mat->a0 * mat->d2 * mat->c3 - mat->a2 * mat->c0 * mat->d3 + mat->a2 * mat->d0 * mat->c3 + mat->a3 * mat->c0 * mat->d2 - mat->a3 * mat->d0 * mat->c2;
    inv.c1 = -mat->a0 * mat->b2 * mat->d3 + mat->a0 * mat->d2 * mat->b3 + mat->a2 * mat->b0 * mat->d3 - mat->a2 * mat->d0 * mat->b3 - mat->a3 * mat->b0 * mat->d2 + mat->a3 * mat->d0 * mat->b2;
    inv.d1 =  mat->a0 * mat->b2 * mat->c3 - mat->a0 * mat->c2 * mat->b3 - mat->a2 * mat->b0 * mat->c3 + mat->a2 * mat->c0 * mat->b3 + mat->a3 * mat->b0 * mat->c2 - mat->a3 * mat->c0 * mat->b2;
    inv.a2 =  mat->b0 * mat->c1 * mat->d3 - mat->b0 * mat->d1 * mat->c3 - mat->b1 * mat->c0 * mat->d3 + mat->b1 * mat->d0 * mat->c3 + mat->b3 * mat->c0 * mat->d1 - mat->b3 * mat->d0 * mat->c1;
    inv.b2 = -mat->a0 * mat->c1 * mat->d3 + mat->a0 * mat->d1 * mat->c3 + mat->a1 * mat->c0 * mat->d3 - mat->a1 * mat->d0 * mat->c3 - mat->a3 * mat->c0 * mat->d1 + mat->a3 * mat->d0 * mat->c1;
    inv.c2 =  mat->a0 * mat->b1 * mat->d3 - mat->a0 * mat->d1 * mat->b3 - mat->a1 * mat->b0 * mat->d3 + mat->a1 * mat->d0 * mat->b3 + mat->a3 * mat->b0 * mat->d1 - mat->a3 * mat->d0 * mat->b1;
    inv.d2 = -mat->a0 * mat->b1 * mat->c3 + mat->a0 * mat->c1 * mat->b3 + mat->a1 * mat->b0 * mat->c3 - mat->a1 * mat->c0 * mat->b3 - mat->a3 * mat->b0 * mat->c1 + mat->a3 * mat->c0 * mat->b1;
    inv.a3 = -mat->b0 * mat->c1 * mat->d2 + mat->b0 * mat->d1 * mat->c2 + mat->b1 * mat->c0 * mat->d2 - mat->b1 * mat->d0 * mat->c2 - mat->b2 * mat->c0 * mat->d1 + mat->b2 * mat->d0 * mat->c1;
    inv.b3 =  mat->a0 * mat->c1 * mat->d2 - mat->a0 * mat->d1 * mat->c2 - mat->a1 * mat->c0 * mat->d2 + mat->a1 * mat->d0 * mat->c2 + mat->a2 * mat->c0 * mat->d1 - mat->a2 * mat->d0 * mat->c1;
    inv.c3 = -mat->a0 * mat->b1 * mat->d2 + mat->a0 * mat->d1 * mat->b2 + mat->a1 * mat->b0 * mat->d2 - mat->a1 * mat->d0 * mat->b2 - mat->a2 * mat->b0 * mat->d1 + mat->a2 * mat->d0 * mat->b1;
    inv.d3 =  mat->a0 * mat->b1 * mat->c2 - mat->a0 * mat->c1 * mat->b2 - mat->a1 * mat->b0 * mat->c2 + mat->a1 * mat->c0 * mat->b2 + mat->a2 * mat->b0 * mat->c1 - mat->a2 * mat->c0 * mat->b1;

	det = (mat->a0 * inv.a0) + (mat->a1 * inv.b0) + (mat->a2 * inv.c0) + (mat->a3 * inv.d0);
	return det;
}

tvbool tv_mat4x4_inverse(TvMat4x4 mat, TvMat4x4 *out)
{
	TvMat4x4 inv;
	tvfloat det;

	TvMat4x4 test_mat = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};


    inv.a0 =  mat.b1 * mat.c2 * mat.d3 - mat.b1 * mat.d2 * mat.c3 - mat.b2 * mat.c1 * mat.d3 + mat.b2 * mat.d1 * mat.c3 + mat.b3 * mat.c1 * mat.d2 - mat.b3 * mat.d1 * mat.c2;
    inv.a1 = -mat.a1 * mat.c2 * mat.d3 + mat.a1 * mat.d2 * mat.c3 + mat.a2 * mat.c1 * mat.d3 - mat.a2 * mat.d1 * mat.c3 - mat.a3 * mat.c1 * mat.d2 + mat.a3 * mat.d1 * mat.c2;
    inv.a2 =  mat.a1 * mat.b2 * mat.d3 - mat.a1 * mat.d2 * mat.b3 - mat.a2 * mat.b1 * mat.d3 + mat.a2 * mat.d1 * mat.b3 + mat.a3 * mat.b1 * mat.d2 - mat.a3 * mat.d1 * mat.b2;
    inv.a3 = -mat.a1 * mat.b2 * mat.c3 + mat.a1 * mat.c2 * mat.b3 + mat.a2 * mat.b1 * mat.c3 - mat.a2 * mat.c1 * mat.b3 - mat.a3 * mat.b1 * mat.c2 + mat.a3 * mat.c1 * mat.b2;
    inv.b0 = -mat.b0 * mat.c2 * mat.d3 + mat.b0 * mat.d2 * mat.c3 + mat.b2 * mat.c0 * mat.d3 - mat.b2 * mat.d0 * mat.c3 - mat.b3 * mat.c0 * mat.d2 + mat.b3 * mat.d0 * mat.c2;
    inv.b1 =  mat.a0 * mat.c2 * mat.d3 - mat.a0 * mat.d2 * mat.c3 - mat.a2 * mat.c0 * mat.d3 + mat.a2 * mat.d0 * mat.c3 + mat.a3 * mat.c0 * mat.d2 - mat.a3 * mat.d0 * mat.c2;
    inv.b2 = -mat.a0 * mat.b2 * mat.d3 + mat.a0 * mat.d2 * mat.b3 + mat.a2 * mat.b0 * mat.d3 - mat.a2 * mat.d0 * mat.b3 - mat.a3 * mat.b0 * mat.d2 + mat.a3 * mat.d0 * mat.b2;
    inv.b3 =  mat.a0 * mat.b2 * mat.c3 - mat.a0 * mat.c2 * mat.b3 - mat.a2 * mat.b0 * mat.c3 + mat.a2 * mat.c0 * mat.b3 + mat.a3 * mat.b0 * mat.c2 - mat.a3 * mat.c0 * mat.b2;
    inv.c0 =  mat.b0 * mat.c1 * mat.d3 - mat.b0 * mat.d1 * mat.c3 - mat.b1 * mat.c0 * mat.d3 + mat.b1 * mat.d0 * mat.c3 + mat.b3 * mat.c0 * mat.d1 - mat.b3 * mat.d0 * mat.c1;
    inv.c1 = -mat.a0 * mat.c1 * mat.d3 + mat.a0 * mat.d1 * mat.c3 + mat.a1 * mat.c0 * mat.d3 - mat.a1 * mat.d0 * mat.c3 - mat.a3 * mat.c0 * mat.d1 + mat.a3 * mat.d0 * mat.c1;
    inv.c2 =  mat.a0 * mat.b1 * mat.d3 - mat.a0 * mat.d1 * mat.b3 - mat.a1 * mat.b0 * mat.d3 + mat.a1 * mat.d0 * mat.b3 + mat.a3 * mat.b0 * mat.d1 - mat.a3 * mat.d0 * mat.b1;
    inv.c3 = -mat.a0 * mat.b1 * mat.c3 + mat.a0 * mat.c1 * mat.b3 + mat.a1 * mat.b0 * mat.c3 - mat.a1 * mat.c0 * mat.b3 - mat.a3 * mat.b0 * mat.c1 + mat.a3 * mat.c0 * mat.b1;
    inv.d0 = -mat.b0 * mat.c1 * mat.d2 + mat.b0 * mat.d1 * mat.c2 + mat.b1 * mat.c0 * mat.d2 - mat.b1 * mat.d0 * mat.c2 - mat.b2 * mat.c0 * mat.d1 + mat.b2 * mat.d0 * mat.c1;
    inv.d1 =  mat.a0 * mat.c1 * mat.d2 - mat.a0 * mat.d1 * mat.c2 - mat.a1 * mat.c0 * mat.d2 + mat.a1 * mat.d0 * mat.c2 + mat.a2 * mat.c0 * mat.d1 - mat.a2 * mat.d0 * mat.c1;
    inv.d2 = -mat.a0 * mat.b1 * mat.d2 + mat.a0 * mat.d1 * mat.b2 + mat.a1 * mat.b0 * mat.d2 - mat.a1 * mat.d0 * mat.b2 - mat.a2 * mat.b0 * mat.d1 + mat.a2 * mat.d0 * mat.b1;
    inv.d3 =  mat.a0 * mat.b1 * mat.c2 - mat.a0 * mat.c1 * mat.b2 - mat.a1 * mat.b0 * mat.c2 + mat.a1 * mat.c0 * mat.b2 + mat.a2 * mat.b0 * mat.c1 - mat.a2 * mat.c0 * mat.b1;

	det = (mat.a0 * inv.a0) + (mat.b0 * inv.a1) + (mat.c0 * inv.a2) + (mat.d0 * inv.a3);
	if(det == 0.0f) {
		return FALSE;
	}
	det = 1.0f / det;
	out->a0 = inv.a0 * det;
	out->a1 = inv.a1 * det;
	out->a2 = inv.a2 * det;
	out->a3 = inv.a3 * det;

	out->b0 = inv.b0 * det;
	out->b1 = inv.b1 * det;
	out->b2 = inv.b2 * det;
	out->b3 = inv.b3 * det;
	
	out->c0 = inv.c0 * det;
	out->c1 = inv.c1 * det;
	out->c2 = inv.c2 * det;
	out->c3 = inv.c3 * det;
	
	out->d0 = inv.d0 * det;
	out->d1 = inv.d1 * det;
	out->d2 = inv.d2 * det;
	out->d3 = inv.d3 * det;


	return TRUE;
}