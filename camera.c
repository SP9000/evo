#include "camera.h"

COMPONENT_NEW(tv_camera, tv_component)
	self->pos.x = 0.0f;
	self->pos.y = 0.0f;
	self->pos.z = 0.0f;
	self->rot.x = 0.0f;
	self->rot.y = 0.0f;
	self->rot.z = 0.0f;
	self->rot.w = 0.0f;
	tv_mat4x4_load_identity(&self->modelview_mat);
	tv_mat4x4_load_identity(&self->projection_mat);
END_COMPONENT_NEW(tv_camera)

COMPONENT_START(tv_camera)
END_COMPONENT_START

COMPONENT_UPDATE(tv_camera)
END_COMPONENT_UPDATE

void tv_camera_perspective(tv_camera* cam, float fov, float aspect, float nearZ, float farZ)
{
	tv_mat4x4_perspective(&cam->projection_mat, fov, aspect, nearZ, farZ);
    cam->ortho = false;
	cam->properties.perspective.fov = fov;
	cam->properties.perspective.aspect = aspect;
	cam->properties.perspective.nearZ = nearZ;
	cam->properties.perspective.farZ = farZ;
}

void tv_camera_orthographic(tv_camera* cam, float left, float right, float top, float bottom, float nearZ, float farZ)
{
	tv_mat4x4_orthographic(&cam->projection_mat, left, right, top, bottom, nearZ, farZ);
    cam->ortho = true;
}
