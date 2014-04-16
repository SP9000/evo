/*****************************************************************************/
/* Camera.c																	 */
/* The camera component - contains information to represent the window(s) to */
/* render within.                                                            */
/*                                                                           */
/* Bryce Wilson                                                              */
/* July 1, 2013                                                              */
/*****************************************************************************/
#ifndef APP_CAMERA_H
#define APP_CAMERA_H

#include "All.h"

typedef struct Camera;
typedef struct Camera {
	TvComponent base;
	/* the camera from the engine, which this component is a wrapper for */
	TvCamera* cam;
} Camera;

void Perspective(Camera* self, float fov, float aspect, float nearZ, float farZ);
void Camera_Orthographic(Camera* self, float left, float right, float top, float bottom,
        float nearZ, float farZ);

#endif
