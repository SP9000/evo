#include "Camera.h"

static void Start(Camera* self);
static void Update(Camera* self);

void Perspective(Camera* self, float fov, float aspect, float nearZ, float farZ)
{
	tv_camera_perspective(self->cam, fov, aspect, nearZ, farZ);
}

void Orthographic(Camera* self, float left, float right, float top, float bottom,
        float nearZ, float farZ)
{
	tv_camera_orthographic(self->cam, left, right, top, bottom, nearZ, farZ);    
}

void Start(Camera* self)
{
}

void Update(Camera* self)
{
}
