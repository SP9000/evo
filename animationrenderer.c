#include "animationrenderer.h"

static void set_model(tv_animation_renderer *self, tv_model *model);

void tv_animation_renderer_set_animation(tv_animation_renderer *self, tv_animation *animation)
{
	self->animation = animation;
}

HANDLER_NEW(tv_animation_renderer, tv_renderer, render, TV_UPDATE_STAGE_MAIN_RENDER)
	tv_component *c = (tv_component*)self;
	self->base.render_func = render;
	self->base.material = NULL;
	self->animation = NULL;
END_HANDLER_NEW(tv_animation_renderer)

HANDLER_START(tv_animation_renderer)
	if(self->base.material == NULL) {
		self->base.material = (tv_material*)tv_component_get((tv_component*)self, tv_material_id());
	}
	if(self->animation == NULL) {
		self->animation = (tv_animation*)tv_component_get((tv_component*)self, tv_animation_id());
	}
END_HANDLER_START

HANDLER_UPDATE(tv_animation_renderer)
END_HANDLER_UPDATE

static void translate_by_bone_(tv_animation_renderer* renderer, tvuint bone_id)
{
	tv_vector3 pos = renderer->animation->bones[bone_id].position;
	tv_vector4 rot = renderer->animation->bones[bone_id].rotation;

	tv_mat4x4_translate(&main_cam->modelview_mat, pos.x, pos.y, pos.z);
	tv_mat4x4_rotate(&main_cam->modelview_mat, rot.x, 1.0f, 0.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat, rot.y, 0.0f, 1.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat, rot.z, 0.0f, 0.0f, 1.0f);
}
static void render_bone_(tv_animation_renderer* renderer, tvuint bone_id)
{
	tv_animation* animation = renderer->animation;
	/* Bind the models' vertex attribute object. */
	glBindVertexArray(animation->bones[bone_id].model->vao);
	/* use the model's material's shader */
	glUseProgram(animation->bones[bone_id].material->program);
	/* set matrices */
	glUniformMatrix4fv(animation->bones[bone_id].material->modelview_mat, 1, GL_FALSE, 
			tv_mat4x4_to_array(&main_cam->modelview_mat));
	glUniformMatrix4fv(animation->bones[bone_id].material->projection_mat, 1, GL_FALSE, 
			tv_mat4x4_to_array(&main_cam->projection_mat));

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	if(utarray_len(animation->bones[bone_id].model->indices) > 0) {
		tv_draw_elements(animation->bones[bone_id].model->primitive, utarray_len(animation->bones[bone_id].model->indices),
			GL_UNSIGNED_SHORT, 0);
	}
	else {
		tv_draw_arrays(animation->bones[bone_id].model->primitive, 0, utarray_len(animation->bones[bone_id].model->vertices));
	}
}

/* TODO: matrix push/pop might be screwy order */
static void render_r_(tv_animation_renderer* renderer, tvuint i)
{
	tv_animation *animation = renderer->animation;
	tv_component *self = (tv_component*)renderer;
	
	tv_mat4x4_push(main_cam->modelview_mat);
	translate_by_bone_(renderer, i);
	render_bone_(renderer, i);
	/* foreach child */
	if(animation->bones[i].next_child != TV_ANIMATION_BONE_END) {
		render_r_(renderer, animation->bones[i].next_child);
	}
	main_cam->modelview_mat = tv_mat4x4_pop();

	/* foreach sibling */
	if(animation->bones[i].next_sibling != TV_ANIMATION_BONE_END) {
		render_r_(renderer, animation->bones[i].next_sibling);
		
	}
	glBindVertexArray(0);
}

static void render(tv_component* self)
{
	tv_animation_renderer *renderer = (tv_animation_renderer*)self;
	tv_vector3 scale = tv_vector3_add(self->entity->transform.scale, main_cam->scale);
	tv_vector3 pos = tv_vector3_add(self->entity->transform.pos, main_cam->pos);
	tv_vector4 rot = tv_vector4_add(self->entity->transform.rot, main_cam->rot);
	
	tv_mat4x4_push(main_cam->modelview_mat);
	tv_mat4x4_load_identity(&main_cam->modelview_mat);
	tv_mat4x4_scale(&main_cam->modelview_mat, scale.x, scale.y, scale.z);
	tv_mat4x4_translate(&main_cam->modelview_mat, pos.x, pos.y, pos.z);
	tv_mat4x4_rotate(&main_cam->modelview_mat, rot.x, 1.0f, 0.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat, rot.y, 0.0f, 1.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat, rot.z, 0.0f, 0.0f, 1.0f);

	render_r_(renderer, 0);
	main_cam->modelview_mat = tv_mat4x4_pop();
}
	