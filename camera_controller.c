#include "camera_controller.h"


COMPONENT_NEW(app_camera_controller, tv_component)
	self->pan_speed = 0.5f;
	self->zoom = 1.0f;
	self->zoom_speed = 400.0f;
	self->zoom_limits.x = 0.0f;
	self->zoom_limits.y = -20.0f;
	self->pan_on_edges = TRUE;
	self->pan_edge_thresholds.x = 5;
	self->pan_edge_thresholds.y = 5;
	self->pan_edge_thresholds.z = 5;
	self->pan_edge_thresholds.w = 5;
END_COMPONENT_NEW(app_camera_controller)

COMPONENT_START(app_camera_controller)
	self->move_right_button.button = INPUT_KEY_RIGHT;
	self->move_left_button.button = INPUT_KEY_LEFT;
	self->move_up_button.button = INPUT_KEY_UP;
	self->move_down_button.button = INPUT_KEY_DOWN;
	self->zoom_in_button.button = TV_INPUT_MOUSE_WHEELUP;
	self->zoom_out_button.button = TV_INPUT_MOUSE_WHEELDOWN;
	self->rotate_down_button.button = INPUT_KEY_PAGEDOWN;
	self->rotate_up_button.button = INPUT_KEY_PAGEUP;

	self->move_right_button.type = TV_INPUT_KEYBOARD;
	self->move_left_button.type = TV_INPUT_KEYBOARD;
	self->move_up_button.type = TV_INPUT_KEYBOARD;
	self->move_down_button.type = TV_INPUT_KEYBOARD;
	self->rotate_down_button.type = TV_INPUT_KEYBOARD;
	self->rotate_up_button.type = TV_INPUT_KEYBOARD;

	self->zoom_in_button.type = TV_INPUT_MOUSE;
	self->zoom_out_button.type = TV_INPUT_MOUSE;

END_COMPONENT_START

COMPONENT_UPDATE(app_camera_controller)
	/* if this is really out of date, input is probably not relevant */
	if(tv_time_delta > 0.5f) {
		return;
	}
	if(tv_input_buttondown(self->move_right_button)) {
		if(main_cam->pos.x > self->pan_x_limits.x) {
			main_cam->pos.x -= self->pan_speed * tv_time_delta;
		}
	}
	if(tv_input_buttondown(self->move_left_button)) {
		if(main_cam->pos.x > self->pan_x_limits.y) {
			main_cam->pos.x += self->pan_speed * tv_time_delta;
		}
	}
	if(tv_input_buttondown(self->move_up_button)) {
		main_cam->pos.y -= self->pan_speed * tv_time_delta;
	}
	if(tv_input_buttondown(self->move_down_button)) {
		main_cam->pos.y += self->pan_speed * tv_time_delta;
	}
	if(tv_input_buttonpressed(self->zoom_in_button)) {
		if(main_cam->pos.z < self->zoom_limits.x) {
			main_cam->pos.z += self->zoom_speed * tv_time_delta;
		}
	}
	if(tv_input_buttonpressed(self->zoom_out_button)) {
		if(main_cam->pos.z > self->zoom_limits.y) {
			main_cam->pos.z -= self->zoom_speed * tv_time_delta;
		}
	}
	if(tv_input_buttondown(self->rotate_down_button)) {
		main_cam->rot.x += self->pan_speed * tv_time_delta;
	}
	if(tv_input_buttondown(self->rotate_up_button)) {
		main_cam->rot.x -= self->pan_speed * tv_time_delta;
	}
	if(self->pan_on_edges) {
		tv_vector2 mouse_pos = tv_input_mouse_pos();
		if(mouse_pos.x < self->pan_edge_thresholds.x) {
			main_cam->pos.x += self->pan_speed * tv_time_delta;
		}
		if(mouse_pos.y < self->pan_edge_thresholds.y) {
			main_cam->pos.y -= self->pan_speed * tv_time_delta;
		}
		if(mouse_pos.x > (screen->w - self->pan_edge_thresholds.z)) {
			main_cam->pos.x -= self->pan_speed * tv_time_delta;
		}
		if(mouse_pos.y > (screen->h - self->pan_edge_thresholds.w)) {
			main_cam->pos.y += self->pan_speed * tv_time_delta;
		}
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