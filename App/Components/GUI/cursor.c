#include "cursor.h"
#include "Abilities/ability.h"

COMPONENT_NEW(app_cursor, tv_component)
	utarray_new(self->selected_units, &ut_ptr_icd);
END_COMPONENT_NEW(app_cursor)

COMPONENT_START(app_cursor)
	self->start.x = -1;
	self->model = NULL;
	//self->renderer = (tv_overlay_renderer*)tv_component_get(self_component, tv_overlay_renderer_id());
	GET(renderer, tv_overlay_renderer);
	
	/* button assignments */
	self->select_button.button = INPUT_MOUSEBUTTON_0;
	self->select_button.type   = TV_INPUT_MOUSE;
	self->move_button.button   = INPUT_MOUSEBUTTON_2;
	self->move_button.type     = TV_INPUT_MOUSE;

	tv_overlay_renderer_set_model(self->renderer, NULL);
	tv_renderer_set_material((tv_renderer*)self->renderer, (tv_material*)tv_component_get(self_component, tv_material_id()));
END_COMPONENT_START

COMPONENT_DESTROY(app_cursor)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(app_cursor)
	tv_model_vertex vertex_format = {2, {TV_MODEL_PROPERTY_FLOAT, TV_MODEL_PROPERTY_FLOAT}, {4,4}};
	
	if(tv_input_buttonpressed(self->move_button)) {
		tv_array* hits;
		tv_collision **c;
		hits = tv_scene_raypick(tv_input_mouse_pos());
		for(c = (tv_collision**)utarray_front(hits); c != NULL; c = (tv_collision**)utarray_next(hits, c)) {
			if((*c)->col2->tags & APP_TAG_TERRAIN) {
				app_ability_target target;
				app_unit** u;
				/* move all selected units to the selected point */
				target.target = (*c)->location;
				for(u = (app_unit**)utarray_front(self->selected_units); u != NULL; u = (app_unit**)utarray_next(self->selected_units, u)) {
					app_unit_move_to(*u, &target);
				}
				//printf("* entity <%s>  @ (%f, %f, %f)\n", (*c)->col2->name, (*c)->location.x, (*c)->location.y, (*c)->location.z);
			}
		}
		tv_free(hits);
	}

	/* mouse down */
	if(tv_input_buttonpressed(self->select_button)) {
		self->start.x = tv_input_mouse_x_normalized();
		self->start.y = tv_input_mouse_y_normalized();
	}
	/* mouse up */
	else if(tv_input_buttonreleased(self->select_button)) {
		tv_array *units;
		app_unit **u;

		self->rect.x = self->start.x;
		self->rect.y = self->start.y, 
		self->rect.w = tv_input_mouse_x_normalized() - self->start.x;
		self->rect.h = tv_input_mouse_y_normalized() - self->start.y;
		if(self->rect.w < 0.0f) {
			self->rect.w = -self->rect.w;
			self->rect.x = tv_input_mouse_x_normalized();
		}
		if(self->rect.h < 0.0f) {
			self->rect.h = -self->rect.h;
			self->rect.y = tv_input_mouse_y_normalized();
		}		
		self->start.x = -1;
		DESTROY(self->model);
		self->model = NULL;
		tv_overlay_renderer_set_model(self->renderer, NULL);

		/* get all units and find the ones that lie within the selection box */
		units = tv_component_get_all_of_type(app_unit_id());
		if(units != NULL) {
			for(u = (app_unit**)utarray_front(units); u != NULL; u = (app_unit**)utarray_next(units, u)) {
				tv_vector3 pos = ((tv_component*)*u)->transform->pos;
				if(tv_rect_contains(self->rect, tv_scene_to_screen_coordinates(pos))) {
					app_hud_add_unit(self->hud, *u);
					utarray_push_back(self->selected_units, u);
					printf("YES\n");
				}
			}
		}
	}

	if(self->start.x > 0) {
		tv_vector2 rect_dims;
		rect_dims.x = (tv_input_mouse_x() / (tvfloat)screen->w) - self->start.x;
		rect_dims.y = (tv_input_mouse_y() / (tvfloat)screen->h) - self->start.y;
		if(rect_dims.x  < 0) {
			self_component->transform->pos.x = (tv_input_mouse_x() / (tvfloat)screen->w);
			rect_dims.x *= -1;
		}
		else {
			self_component->transform->pos.x = self->start.x;
		}
		if(rect_dims.y < 0) {
			self_component->transform->pos.y = (tv_input_mouse_y() / (tvfloat)screen->h);
			rect_dims.y *= -1;
		}
		else {
			self_component->transform->pos.y = self->start.y;
		}

		if(self->model) {
			DESTROY(self->model);
		}
		self->model = tv_modelgen_quad(rect_dims, vertex_format);
		tv_overlay_renderer_set_model(self->renderer, self->model);
	}

END_COMPONENT_UPDATE

void app_cursor_set_hud(app_cursor* self, app_hud* hud)
{
	self->hud = hud;
}
