#ifndef CAMERA_H
#define CAMERA_H
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "tv_alloc.h"
#include "component.h"

typedef struct tagTvCameraPerspective {
	float fov;
	float aspect;
	float nearZ;
	float farZ;
}TvCameraPerspective;

typedef struct tagTvCameraOrthographic {
	tvuint w;
	tvuint h;
}TvCameraOrthographic;

typedef union tagTvCameraProperties {
	TvCameraOrthographic ortho;
	TvCameraPerspective perspective;
}TvCameraProperties;

COMPONENT(tv_camera, tv_component)
	TvVector3 pos;
	TvVector3 rot;
	TvVector3 scale;
	tvbool ortho;
	tvu32 render_layers;

	TvMat4x4 model_mat;
	TvMat4x4 view_mat;
	TvMat4x4 projection_mat;
	TvCameraProperties properties;
ENDCOMPONENT(tv_camera)

void tv_camera_perspective(tv_camera* cam, float fov, float aspect, float nearZ, float farZ);
void tv_camera_orthographic(tv_camera* cam, float left, float right, float top, float bottom,
            float nearZ, float farZ);

#ifdef __cplusplus
}
#endif
#endif