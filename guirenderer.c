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

static void render_children(tv_component *self)
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
	/* only use transform if this widget is a component of an entity (the root widget) */
	if(((tv_component*)widget)->entity != NULL) {
		pos = ((tv_component*)widget)->entity->transform.pos;
		scale  = ((tv_component*)widget)->entity->transform.scale;
	}
	else {
		pos = widget->pos;
		scale.x = scale.y = scale.z = 1.0f;
	}

	if(widget->model != NULL) {
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
}

static void render(tv_component *self)
{	
	/* render each child on top of its parent */
	glDisable(GL_DEPTH_TEST);

	/* save the state of the modelview matrix and reset it */
	tv_mat4x4_push(&tv_camera_gui->modelview_mat);
	tv_mat4x4_load_identity(&tv_camera_gui->modelview_mat);
	
	/* render this widget and all its children */
	render_children(self);

	/* restore the modelview matrix */
	tv_mat4x4_pop(&tv_camera_gui->modelview_mat);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);	
}

void tv_gui_renderer_set_base_widget(tv_gui_renderer *renderer, tv_widget *w)
{
	renderer->widget = w;
}