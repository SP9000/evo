#include "modelrenderer.h"

void tv_model_renderer_set_model(tv_model_renderer *self, tv_model *model)
{
	self->model = model;
}

HANDLER_NEW(tv_model_renderer, tv_renderer, render, TV_UPDATE_STAGE_MAIN_RENDER)
	tv_component *c = (tv_component*)self;
	self->base.render_func = render;
	self->base.material = NULL;
	self->model = NULL;
END_HANDLER_NEW(tv_model_renderer)

HANDLER_START(tv_model_renderer)
	if(self->base.material == NULL) {
		self->base.material = (tv_material*)tv_component_get((tv_component*)self, tv_material_id());
	}
	if(self->model == NULL) {
		self->model = (tv_model*)tv_component_get((tv_component*)self, tv_model_id());
	}
END_HANDLER_START

HANDLER_UPDATE(tv_model_renderer)
	/* if this renderer has no model yet, check if one has been added */
	if(self->model == NULL) {
		GET(model, tv_model);
	}
END_HANDLER_UPDATE

static void render(tv_component* self)
{
	tv_vector3 test = {self->entity->transform.pos.x, self->entity->transform.pos.y, self->entity->transform.pos.z};
	//tv_vector3 test = {-2.0f,0.0f,-7.0f};
	tv_model_renderer *renderer = (tv_model_renderer*)self;
	tv_material* default_mat;
	tv_material** mat;
	tv_model** mesh;
	tvuint i;

	if(renderer->model == NULL) {
		return;
	}
	//tv_scene_to_screen_coordinates(test);

	tv_mat4x4_push(&main_cam->modelview_mat);
	tv_mat4x4_load_identity(&main_cam->modelview_mat);
	tv_mat4x4_scale(&main_cam->modelview_mat, self->entity->transform.scale.x, 
		self->entity->transform.scale.y,
		self->entity->transform.scale.z);
	tv_mat4x4_translate(&main_cam->modelview_mat, self->entity->transform.pos.x + main_cam->pos.x,
		self->entity->transform.pos.y + main_cam->pos.y, self->entity->transform.pos.z + main_cam->pos.z);
	tv_mat4x4_rotate(&main_cam->modelview_mat, -self->entity->transform.rot.x + main_cam->rot.x, 1.0f, 0.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat, -self->entity->transform.rot.y + main_cam->rot.y, 0.0f, 1.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat, -self->entity->transform.rot.z + main_cam->rot.z, 0.0f, 0.0f, 1.0f);

    /* bind any samplers (textures) the material uses */
    //if(self->material->texture.id != 0) {
        //glUniform1i(self->material->texture.loc, 0); /* TODO: use glProgramUniform in material.c */
        //glActiveTexture(GL_TEXTURE0 + 0);
        //glBindTexture(GL_TEXTURE_2D, self->material->texture.id);
        //glBindSampler(0, self->material->texture.sampler); 
    //}

    /* bind attribute array and draw */
	//glEnableVertexAttribArray(renderer->model->vao);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(TRUE);

	default_mat = renderer->base.material;
	assert(default_mat);

	/* render each pass of the primary mesh's material */
	glBindVertexArray(renderer->model->vao);
	for(i = 0; i < default_mat->num_passes; ++i) {
		tv_material_do_pass(default_mat, i, renderer->model);
	}
	/* if this mesh has submeshes, render them. */
	if(utarray_len(renderer->model->submeshes) > 0) {
		/* render the submeshs bind the models' vertex attribute object. */
		for(mesh = (tv_model**)utarray_front(renderer->model->submeshes),
			mat = (tv_material**)utarray_front(renderer->base.materials); 
			mesh != NULL;
			mesh = (tv_model**)utarray_next(renderer->model->submeshes, mesh)) {
				if(mat == NULL) {
					/* no material exists, use default material */
					*mat = renderer->base.material;
				}
				/* render each pass of the submesh's material */
				glBindVertexArray((*mesh)->vao);
				for(i = 0; i < (*mat)->num_passes; ++i) {
					tv_material_do_pass(*mat, i, *mesh);
				}
				/* get the next material in the material array */
				if(mat != NULL) {
					mat = (tv_material**)utarray_next(renderer->base.materials, mat);
				} 
		}
	}
	/* if material is lit, set light uniforms (TODO: UBO?) */
	/* TODO:
	if(renderer->base.material->lit) {
		tv_array *lights = tv_scene_lights_at(self->entity->transform.pos);
		tv_light *l;
		for(l = (tv_light*)utarray_front(lights); l != NULL; l = (tv_light*)utarray_next(lights, l)) {
			glUniform3fv(renderer->base.material->lights[i], 1,
				(GLfloat*)(&l->base.entity->transform.pos));
			glUniform3fv(renderer->base.material->lights[i+1], 1,
				(GLfloat*)(&l->color));
			glUniform3fv(renderer->base.material->lights[i+2], 1,
				(GLfloat*)(&l->dir));
		}
	}
	*/
    glBindVertexArray(0);
	tv_mat4x4_pop(&main_cam->modelview_mat);
}
