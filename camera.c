#include "camera.h"

void tv_camera_perspective(TvCamera* cam, float fov, float aspect, float nearZ, float farZ)
{
	tv_mat4x4_perspective(cam->projection_mat, fov, aspect, nearZ, farZ);
    cam->ortho = false;
	cam->properties.perspective.fov = fov;
	cam->properties.perspective.aspect = aspect;
	cam->properties.perspective.nearZ = nearZ;
	cam->properties.perspective.farZ = farZ;
}

void tv_camera_orthographic(TvCamera* cam, float left, float right, float top, float bottom, float nearZ, float farZ)
{
	tv_mat4x4_orthographic(cam->projection_mat, left, right, top, bottom, nearZ, farZ);
    cam->ortho = true;
}