#include "camera_controller.h"


COMPONENT_NEW(app_camera_controller, tv_component)
	self->pan_speed = 0.5f;
	self->zoom = 1.0f;
END_COMPONENT_NEW(app_camera_controller)

COMPONENT_START(app_camera_controller)
END_COMPONENT_START

COMPONENT_UPDATE(app_camera_controller)
	if(tv_input_keydown(INPUT_KEY_RIGHT)) {
		if(main_cam->pos.x > self->pan_x_limits.x) {
			main_cam->pos.x -= self->pan_speed * tv_time_delta;
		}
	}
	if(tv_input_keydown(INPUT_KEY_LEFT)) {
		if(main_cam->pos.x > self->pan_x_limits.y) {
			main_cam->pos.x += self->pan_speed * tv_time_delta;
		}
	}
	if(tv_input_keydown(INPUT_KEY_UP)) {
		main_cam->pos.y -= self->pan_speed * tv_time_delta;
	}
	if(tv_input_keydown(INPUT_KEY_DOWN)) {
		main_cam->pos.y += self->pan_speed * tv_time_delta;
	}
	if(tv_input_keydown(INPUT_KEY_EQUALS)) {
		main_cam->pos.z += self->pan_speed * tv_time_delta;
	}
	if(tv_input_keydown(INPUT_KEY_MINUS)) {
		main_cam->pos.z -= self->pan_speed * tv_time_delta;
	}
	if(tv_input_keydown(INPUT_KEY_PAGEUP)) {
		main_cam->rot.x += self->pan_speed * tv_time_delta;
	}
	if(tv_input_keydown(INPUT_KEY_PAGEDOWN)) {
		main_cam->rot.x -= self->pan_speed * tv_time_delta;
	}
	END_COMPONENT_UPDATE

void app_camera_controller_set_pan_speed(app_camera_controller *controller, tvfloat speed)
{
	controller->pan_speed = speed;
}

void app_camera_controller_set_zoom(app_camera_controller *controller, tvfloat zoom)
{
	main_cam->pos.z = zoom;
}

void app_camera_controller_set_pan_limits(app_camera_controller *controller, tv_vector2 x_lim, tv_vector2 y_lim, tv_vector2 zoom_lim)
{
	controller->pan_x_limits = x_lim;
	controller->pan_y_limits = y_lim;
}