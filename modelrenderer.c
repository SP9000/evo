#include "modelrenderer.h"

static void set_model(tv_model_renderer *self, tv_model *model);


void tv_model_renderer_set_model(tv_model_renderer *self, tv_model *model)
{
	self->model = model;
}

HANDLER_NEW(tv_model_renderer, tv_renderer, render, 1)
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
END_HANDLER_UPDATE

static void render(tv_component* self)
{
	tvuint i;
	tv_model_renderer *renderer = (tv_model_renderer*)self;
	if(renderer->model == NULL) {
		return;
	}
	tv_mat4x4_push(main_cam->view_mat);
	tv_mat4x4_translate(main_cam->view_mat, -self->entity->transform.pos.x,
		-self->entity->transform.pos.y, self->entity->transform.pos.z);
    main_cam->view_mat[0] *= self->entity->transform.scale.x;
    main_cam->view_mat[5] *= self->entity->transform.scale.y;
    main_cam->view_mat[10] *= self->entity->transform.scale.z;

    /* Bind the models' vertex attribute object. */
    glBindVertexArray(renderer->model->vao);
    /* use the model's material's shader */
    glUseProgram(renderer->base.material->program);

    /* set matrices */
	glUniformMatrix4fv(renderer->base.material->model_mat, 1, GL_FALSE, 
            main_cam->model_mat);
	glUniformMatrix4fv(renderer->base.material->view_mat, 1, GL_FALSE, 
            main_cam->view_mat);
	glUniformMatrix4fv(renderer->base.material->projection_mat, 1, GL_FALSE, 
            main_cam->projection_mat);

    /* bind any samplers (textures) the material uses */
    //if(self->material->texture.id != 0) {
        //glUniform1i(self->material->texture.loc, 0); /* TODO: use glProgramUniform in material.c */
        //glActiveTexture(GL_TEXTURE0 + 0);
        //glBindTexture(GL_TEXTURE_2D, self->material->texture.id);
        //glBindSampler(0, self->material->texture.sampler); 
    //}

    /* bind attribute array and draw */
	//glEnableVertexAttribArray(renderer->model->vao);

    glBindVertexArray(renderer->model->vao);
	glDrawElements(renderer->model->primitive, (GLsizei)utarray_len(renderer->model->indices),
		GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

	tv_mat4x4_pop(main_cam->view_mat);
}
