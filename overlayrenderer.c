#include "overlayrenderer.h"

typedef struct tv_my_vertex {
	tvfloat x;
	tvfloat y;
	tvfloat z;
}tv_my_vertex;

typedef struct tv_textrenderer_vertex {
	tv_my_vertex pos;
}tv_textrenderer_vertex;


void tv_overlay_renderer_set_model(tv_overlay_renderer *self, tv_model *model)
{
	self->model = model;
}

HANDLER_NEW(tv_overlay_renderer, tv_renderer, render, TV_UPDATE_STAGE_GUI_RENDER)
	self->base.render_func = render;
	self->base.material = NULL;
END_HANDLER_NEW(tv_overlay_renderer)

HANDLER_START(tv_overlay_renderer)
	if(self->base.material == NULL) {
		self->base.material = (tv_material*)tv_component_get((tv_component*)self, tv_material_id());
	}
END_HANDLER_START

HANDLER_UPDATE(tv_overlay_renderer)
END_HANDLER_UPDATE

static void render(tv_component *self)
{
	tv_overlay_renderer *renderer = (tv_overlay_renderer*)self;

	tv_vector3 pos; 
	tv_vector3 scale; 

	if(renderer->model == NULL) {
		return;
	}

	pos = self->entity->transform.pos;
	scale  = self->entity->transform.scale;

	glDisable(GL_DEPTH_TEST);
	tv_mat4x4_push(tv_camera_gui->modelview_mat);
	tv_mat4x4_load_identity(&tv_camera_gui->modelview_mat);
	tv_mat4x4_translate(&tv_camera_gui->modelview_mat, pos.x, pos.y, pos.z);
	tv_camera_gui->modelview_mat.a0 *= scale.x;
	tv_camera_gui->modelview_mat.b1 *= scale.y;

	/* use the model's material's shader */
	glUseProgram(((tv_renderer*)renderer)->material->program);

	/* set matrices */
	/* TODO: should be GL_FALSE */
	glUniformMatrix4fv(((tv_renderer*)renderer)->material->modelview_mat, 1, GL_TRUE, 
		tv_mat4x4_to_array(&tv_camera_gui->modelview_mat));
	glUniformMatrix4fv(((tv_renderer*)renderer)->material->projection_mat, 1, GL_FALSE, 
		tv_mat4x4_to_array(&tv_camera_gui->projection_mat));

	/* bind attribute array and draw */
	glBindVertexArray(renderer->model->vao);
	tv_draw_arrays(renderer->model->primitive, 0, utarray_len(renderer->model->vertices));
	glBindVertexArray(0);
	tv_camera_gui->modelview_mat = tv_mat4x4_pop();

	glEnable(GL_DEPTH_TEST);
}
