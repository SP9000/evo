#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

COMPONENT(app_camera_controller, tv_component)
	tvfloat zoom;
	tv_vector2 pan_x_limits;
	tv_vector2 pan_y_limits;
	tv_vector2 zoom_limits;
	tvfloat pan_speed;
ENDCOMPONENT(app_camera_controller)

void app_camera_controller_set_pan_limits(app_camera_controller *controller, tv_vector2 x_lim, tv_vector2 y_lim, tv_vector2 zoom_lim);
void app_camera_controller_set_pan_speed(app_camera_controller *controller, tvfloat speed);
void app_camera_controller_set_zoom(app_camera_controller *controller, tvfloat zoom);

#ifdef __cplusplus
}
#endif
#endif