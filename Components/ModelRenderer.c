#include "ModelRenderer.h"

static void Start(ModelRenderer* self);
static void Update(ModelRenderer* self);
static void Render(ModelRenderer* self);

void ModelRenderer_Load(ModelRenderer* self, char* file)
{
	self->model = tv_model_load_ply(file);
}

void Start(ModelRenderer* self) 
{
}

void Update(ModelRenderer* self) 
{
		
}

void Render(ModelRenderer* self)
{
	tv_mat4x4_push(main_cam->view_mat);
	tv_mat4x4_translate(main_cam->view_mat, -self->base.base.entity->pos.x,
		-self->base.base.entity->pos.y, self->base.base.entity->pos.z);
    main_cam->view_mat[0] *= self->base.base.entity->pos.x;
    main_cam->view_mat[5] *= self->base.base.entity->pos.y;
    main_cam->view_mat[10] *= self->base.base.entity->pos.z;

    /* Bind the models' vertex attribute object. */
    glBindVertexArray(self->model->vao);
    /* use the model's material's shader */
    glUseProgram(self->base.material->program);

    /* set matrices */
	glUniformMatrix4fv(self->base.material->model_mat, 1, GL_FALSE, 
            main_cam->model_mat);
	glUniformMatrix4fv(self->base.material->view_mat, 1, GL_FALSE, 
            main_cam->view_mat);
	glUniformMatrix4fv(self->base.material->projection_mat, 1, GL_FALSE, 
            main_cam->projection_mat);

    /* bind any samplers (textures) the material uses */
    //if(self->material->texture.id != 0) {
        //glUniform1i(self->material->texture.loc, 0); /* TODO: use glProgramUniform in material.c */
        //glActiveTexture(GL_TEXTURE0 + 0);
        //glBindTexture(GL_TEXTURE_2D, self->material->texture.id);
        //glBindSampler(0, self->material->texture.sampler); 
    //}

    /* bind attribute array and draw */
    glBindVertexArray(self->model->vao);
    /*
	glDrawArrays(self->model->primitive, 0, 
		self->model->num_vertices);
	*/
	glDrawElements(self->model->primitive, self->model->num_indices, 
		GL_UNSIGNED_SHORT, self->model->indices); 
    glBindVertexArray(0);

	tv_mat4x4_pop(main_cam->view_mat);
}

