#include "guirenderer.h"

#include "textrenderer.h"

typedef struct tv_my_vertex {
	tvfloat x;
	tvfloat y;
	tvfloat z;
}tv_my_vertex;
typedef struct tv_my_texco {
	tvfloat u;
	tvfloat v;
	tvfloat dummy;
}tv_my_texco;

typedef struct tv_textrenderer_vertex {
	tv_my_vertex pos;
	tv_my_texco tex;
}tv_textrenderer_vertex;

HANDLER_NEW(tv_gui_renderer, tv_renderer, render, TV_UPDATE_STAGE_GUI_RENDER)
	utstring_new(self->text);
	self->font = NULL;
	self->font_size = 0.025f;
	self->font_texture = tv_texture_new();
	self->base.render_func = render;
	self->base.material = NULL;

	self->widget = NULL;
END_HANDLER_NEW(tv_gui_renderer)

HANDLER_START(tv_gui_renderer)
    if(self->font != NULL) {
        self->font_texture = tv_texture_load_bmp((char*)self->font);
    }
	if(self->base.material == NULL) {
		self->base.material = (tv_material*)tv_component_get((tv_component*)self, tv_material_id());
	}
	if(self->widget == NULL) {
		self->widget = (tv_widget*)tv_component_get(self_component, tv_widget_id());
	}
END_HANDLER_START

HANDLER_UPDATE(tv_gui_renderer)
END_HANDLER_UPDATE

static void render(tv_component *self)
{
	tvuint i;
	tv_gui_renderer *renderer = (tv_gui_renderer*)self;
	tv_widget *widget = renderer->widget;
	tv_widget **children = tv_widget_children(widget); 

	tv_vector3 pos; 
	tv_vector3 scale; 

	if(widget == NULL) {
		return;
	}
	pos = ((tv_component*)widget)->entity->transform.pos;
	scale  = ((tv_component*)widget)->entity->transform.scale;

	if(widget->model != NULL) {
		glDisable(GL_DEPTH_TEST);
		tv_mat4x4_push(tv_camera_gui->modelview_mat);
		tv_mat4x4_load_identity(&tv_camera_gui->modelview_mat);
		tv_mat4x4_translate(&tv_camera_gui->modelview_mat, pos.x, pos.y, pos.z);
		tv_mat4x4_scale(&tv_camera_gui->modelview_mat, scale.x, scale.y, 1.0f);
		/* use the model's material's shader */
		glUseProgram(widget->material->program);

		/* set matrices */
		glUniformMatrix4fv(widget->material->modelview_mat, 1, GL_FALSE, 
			tv_mat4x4_to_array(&tv_camera_gui->modelview_mat));
		glUniformMatrix4fv(widget->material->projection_mat, 1, GL_FALSE, 
			tv_mat4x4_to_array(&tv_camera_gui->projection_mat));

		/* bind attribute array and draw */
		glBindVertexArray(widget->model->vao);
		tv_draw_arrays(widget->model->primitive, 0, utarray_len(widget->model->vertices));
		glBindVertexArray(0);
		tv_camera_gui->modelview_mat = tv_mat4x4_pop();
	}
	/* recursively render all the widgets */
	if(children == NULL) {
		return;
	}
	for(i = 0; i < tv_widget_num_children(widget); ++i) {
		renderer->widget = children[i];
		render(self);
	}
	/* restore widget */
	renderer->widget = widget;
	glEnable(GL_DEPTH_TEST);
}

void tv_gui_renderer_set_base_widget(tv_gui_renderer *renderer, tv_widget *w)
{
	renderer->widget = w;
}