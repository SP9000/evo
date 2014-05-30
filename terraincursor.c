#include "camera_controller.h"
#include "terraincursor.h"


COMPONENT_NEW(app_terrain_cursor, tv_component)
	tv_vector3 dim = {1.0f, 1.0f, 1.0f};
	tv_model_vertex format;	//TODO: format not used in modelgen...yet...might be in future..if not, this isn't necessary
	format.count = 0;	//TODO: just to keep vs' debugger from whining 


	self->move_left_key.type = TV_INPUT_KEYBOARD;
	self->move_right_key.type = TV_INPUT_KEYBOARD;
	self->move_forward_key.type = TV_INPUT_KEYBOARD;
	self->move_back_key.type = TV_INPUT_KEYBOARD;
	self->move_up_key.type = TV_INPUT_KEYBOARD;
	self->move_down_key.type = TV_INPUT_KEYBOARD;
	self->select_key.type = TV_INPUT_KEYBOARD;

	self->move_left_key.button = INPUT_KEY_h;
	self->move_right_key.button = INPUT_KEY_l;
	self->move_forward_key.button = INPUT_KEY_j;
	self->move_back_key.button = INPUT_KEY_k;
	self->move_up_key.button = INPUT_KEY_u;
	self->move_down_key.button = INPUT_KEY_n;
	self->select_key.button = INPUT_KEY_RETURN;

	self->model = tv_modelgen_tetrahedron(dim, format);

	self->x_step = 1.0f;
	self->y_step = 1.0f;
	self->z_step = 1.0f;

	self->timer.set = APP_TERRAIN_CURSOR_SPEED;
	self->timer.current = APP_TERRAIN_CURSOR_SPEED;
	self->timer.max_rollover = 1;
END_COMPONENT_NEW(app_terrain_cursor)

COMPONENT_START(app_terrain_cursor)
	self->renderer = (tv_model_renderer*)tv_component_get(self_component, tv_model_renderer_id());
	tv_model_renderer_set_model(self->renderer, self->model);
END_COMPONENT_START

COMPONENT_UPDATE(app_terrain_cursor)
if(!tv_timer_update(&self->timer)) {
	return;
}
if(tv_input_buttondown(self->move_left_key)) {
	self_component->transform->pos.x -= self->x_step;
}
else if(tv_input_buttondown(self->move_right_key)) {
	self_component->transform->pos.x += self->x_step;
}
else if(tv_input_buttondown(self->move_forward_key)) {
	self_component->transform->pos.z += self->z_step;
}
else if(tv_input_buttondown(self->move_back_key)) {
	self_component->transform->pos.z -= self->z_step;
}
else if(tv_input_buttondown(self->move_down_key)) {
	self_component->transform->pos.y -= self->y_step;
}
else if(tv_input_buttondown(self->move_up_key)) {
	self_component->transform->pos.y += self->y_step;
}
else if(tv_input_buttondown(self->select_key)) {
} 
END_COMPONENT_UPDATE