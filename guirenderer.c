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

	self->animation = NULL;
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
	/*
	if(self->animation == NULL) {
		self->animation = (tv_animation*)tv_component_get(self_component, tv_animation_id());
	}
	else if(tv_component_get(self_component, tv_model_id())) {
		self->animation = tv_animation_new();
		tv_animation_set_root(self->animation, (tv_model*)tv_component_get(self_component, tv_model_id()), self->base.material);
	}
	*/
END_HANDLER_START

HANDLER_UPDATE(tv_gui_renderer)
END_HANDLER_UPDATE

/*****************************************************************************/
/* the animation renderer */
static void translate_by_bone_(tv_gui_renderer* renderer, tvuint bone_id)
{
	tv_vector3 pos = renderer->widget->animation->bones[bone_id].position;
	tv_vector4 rot = renderer->widget->animation->bones[bone_id].rotation;

	tv_mat4x4_translate(&tv_camera_gui->modelview_mat, pos.x, pos.y, pos.z);
	tv_mat4x4_rotate(&tv_camera_gui->modelview_mat, rot.x, 1.0f, 0.0f, 0.0f);
	tv_mat4x4_rotate(&tv_camera_gui->modelview_mat, rot.y, 0.0f, 1.0f, 0.0f);
	tv_mat4x4_rotate(&tv_camera_gui->modelview_mat, rot.z, 0.0f, 0.0f, 1.0f);
}
static void render_bone_(tv_gui_renderer* renderer, tvuint bone_id)
{
	tv_animation* animation = renderer->widget->animation;
	tv_material* mat = animation->bones[bone_id].material;
	tvuint i;

	/* use the renderer's material if bone has none */
	if(mat == NULL) {
		mat = ((tv_renderer*)renderer)->material;
	}
	/* Bind the models' vertex attribute object. */
	glBindVertexArray(animation->bones[bone_id].model->vao);
	/* render all passes of the bone */
	for(i = 0; i < mat->num_passes; ++i) {
		tv_material_do_pass_gui(mat, i, animation->bones[bone_id].model);
	}
}

/* TODO: matrix push/pop might be screwy order */
static void render_r_(tv_gui_renderer* renderer, tvuint i)
{
	tv_animation *animation = renderer->widget->animation;
	tv_component *self = (tv_component*)renderer;

	tv_mat4x4_push(&tv_camera_gui->modelview_mat);
	//translate_by_bone_(renderer, i);
	render_bone_(renderer, i);

	/* foreach child */
	if(animation->bones[i].next_child != TV_ANIMATION_BONE_END) {
		render_r_(renderer, animation->bones[i].next_child);
	}
	tv_mat4x4_pop(&tv_camera_gui->modelview_mat);

	/* foreach sibling */
	if(animation->bones[i].next_sibling != TV_ANIMATION_BONE_END) {
		render_r_(renderer, animation->bones[i].next_sibling);
	}
	glBindVertexArray(0);
}

static void anim_render(tv_component* self)
{
	tv_vector3 scale, pos;
	tv_vector4 rot;
	tv_gui_renderer* renderer = (tv_gui_renderer*)self;
	tv_animation* anim = renderer->animation;

	tv_mat4x4_push(&tv_camera_gui->modelview_mat);
	tv_mat4x4_translate(&tv_camera_gui->modelview_mat, pos.x, pos.y, pos.z);
	tv_mat4x4_scale(&tv_camera_gui->modelview_mat, scale.x, scale.y, scale.z);
	tv_mat4x4_rotate(&tv_camera_gui->modelview_mat, rot.x, 1.0f, 0.0f, 0.0f);
	tv_mat4x4_rotate(&tv_camera_gui->modelview_mat, rot.y, 0.0f, 1.0f, 0.0f);
	tv_mat4x4_rotate(&tv_camera_gui->modelview_mat, rot.z, 0.0f, 0.0f, 1.0f);
	render_r_(renderer, 0);
	tv_mat4x4_pop(&tv_camera_gui->modelview_mat);
}

/*****************************************************************************/
/* widget renderer */
static void render_children(tv_component *self)
{
	tvuint i;
	tv_gui_renderer *renderer = (tv_gui_renderer*)self;
	tv_widget *widget = renderer->widget;
	tv_widget **children = tv_widget_children(widget); 
	tv_vector3 pos, scale;
	tv_vector4 rot;

	if(widget == NULL) {
		return;
	}
#if 0
	if(widget->animation != NULL) {
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
		glBindVertexArray(widget->a->vao);
		tv_draw_arrays(widget->model->primitive, 0, utarray_len(widget->model->vertices));
	}
#endif
	if(widget->animation != NULL) {
		/* only use transform if this widget is a component of an entity (the root widget) */
		if(((tv_component*)widget)->entity != NULL) {
			pos = ((tv_component*)widget)->entity->transform.pos;
			scale  = ((tv_component*)widget)->entity->transform.scale;
			rot = ((tv_component*)widget)->entity->transform.rot;
		}
		else {
			pos = widget->pos;
			scale = widget->scale;
			rot = widget->rot;
		}
		/* apply the widget transformations and render the widget's animation */
		tv_mat4x4_translate(&tv_camera_gui->modelview_mat, pos.x, pos.y, pos.z);
		tv_mat4x4_rotate(&tv_camera_gui->modelview_mat, rot.x, 1.0f, 0.0f, 0.0f);
		tv_mat4x4_rotate(&tv_camera_gui->modelview_mat, rot.y, 0.0f, 1.0f, 0.0f);
		tv_mat4x4_rotate(&tv_camera_gui->modelview_mat, rot.z, 0.0f, 0.0f, 1.0f);
		tv_mat4x4_scale(&tv_camera_gui->modelview_mat, scale.x, scale.y, 1.0f);
		render_r_(renderer, 0);
		//anim_render(self);
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
	tv_camera_orthographic(tv_camera_gui, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 100.0f);
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