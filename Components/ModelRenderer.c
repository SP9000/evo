/*****************************************************************************/
/* ModelRenderer.c                                                           */
/* One of the more basic renderers.  This just renders a single model.       */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 19, 2013                                                    */
/*****************************************************************************/
#include "../draw.h"
COMPONENT ModelRenderer : Renderer {
    getset Component_Model* model;
    Component_Transform* transform;

    void Start() 
    {
        self->material = Component_GetAs(Material);
        self->model = Component_GetAs(Model);
        self->transform = Component_GetAs(Transform);
    }
    void Update() 
    {
		
    }
    void Collide(Entity* e)
    {

    }
    public void Render()
    {
        Mat4x4Push(&main_cam->viewMat);
        Mat4x4Translate(&main_cam->viewMat, -self->model->transform->pos.x, 
            -self->model->transform->pos.y, self->model->transform->pos.z);
        main_cam->viewMat.a00 *= self->transform->scale.x;
        main_cam->viewMat.a11 *= self->transform->scale.y;
        main_cam->viewMat.a22 *= self->transform->scale.z;

        /* Bind the models' vertex attribute object. */
        glBindVertexArray(self->model->vao);
        /* use the model's material's shader */
        glUseProgram(self->material->program);

        /* set matrices */
        glUniformMatrix4fv(self->material->modelMatrixID, 1, GL_FALSE, 
                Mat4x4Pack(&main_cam->modelMat));
        glUniformMatrix4fv(self->material->viewMatrixID, 1, GL_FALSE, 
                Mat4x4Pack(&main_cam->viewMat));
        glUniformMatrix4fv(self->material->projectionMatrixID, 1, GL_FALSE, 
                Mat4x4Pack(&main_cam->projectionMat));

        /* bind any samplers (textures) the material uses */
        //if(self->material->texture.id != 0) {
            //glUniform1i(self->material->texture.loc, 0); /* TODO: use glProgramUniform in material.c */
            //glActiveTexture(GL_TEXTURE0 + 0);
            //glBindTexture(GL_TEXTURE_2D, self->material->texture.id);
            //glBindSampler(0, self->material->texture.sampler); 
        //}

        /* bind attribute array and draw */
        glBindVertexArray(self->model->vao);
        glDrawArrays(self->model->primitive, 0, 
                self->model->numVertices);
        glBindVertexArray(0);

        Mat4x4Pop(&main_cam->viewMat);
    }
}

