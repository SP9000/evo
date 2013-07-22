/*****************************************************************************/
/* TextRenderer.c                                                            */
/* This component is designed for rendering text, but really can be used to  */
/* render any sequence of models.                                            */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 19, 2013                                                    */
/*****************************************************************************/

/*  TODO: could make this actually do text..make another component for more  */
/* general stuff.                                                            */

#include "../draw.h"
COMPONENT TextRenderer : Renderer {
    public char* font;
    public int len;

    char* text;
    Component_Model* model;
    Component_Texture* font_texture;
    Component_Transform* transform;

    void Start() 
    {
        if(self->font != NULL) {
            self->font_texture->LoadBMP(self->font_texture, self->font);
        }
        if(self->text != NULL) {
            self->SetText(self, self->text);
        }
    }
    void Update() 
    {
        
    }
    void Collide(Entity* e)
    {
        
    }

    void SetText(char* new_text)
    {
        int i;
        float texco[2];
        float vertex[3];
        float uv_w = 1.0f / 16.0f;
        float uv_h = 1.0f / 16.0f;
        float ch_w = 0.5f;
        float ch_h = 0.5f;

        self->text = new_text;
        self->model->Free(self->model);
        vertex[2] = 0.0f;
        vertex[1] = 0.0f;
        for(i = 0; i < strlen(self->text); ++i) {
            /* upper left */
            texco[0] = (self->text[i] % 16);
            texco[1] = (self->text[i] / 16);
            vertex[0] = i;
            self->model->SetAttribute(self->model, MODEL_ATTRIBUTE_VERTEX, i*4, vertex);
            self->model->SetAttribute(self->model, MODEL_ATTRIBUTE_TEXCO, i*4, texco);

            /* upper right corner */
            texco[0] += uv_w;
            vertex[0] += ch_w;
            self->model->SetAttribute(self->model, MODEL_ATTRIBUTE_VERTEX, i*4+1, vertex);
            self->model->SetAttribute(self->model, MODEL_ATTRIBUTE_TEXCO, i*4+1, texco);

            /* lower right corner */
            texco[1] += uv_h;
            vertex[1] += ch_h;
            self->model->SetAttribute(self->model, MODEL_ATTRIBUTE_VERTEX, i*4+2, vertex);
            self->model->SetAttribute(self->model, MODEL_ATTRIBUTE_TEXCO, i*4+2, texco);

            /* lower left corner */
            texco[0] -= uv_w;
            vertex[0] -= ch_w;
            self->model->SetAttribute(self->model, MODEL_ATTRIBUTE_VERTEX, i*4+3, vertex);
            self->model->SetAttribute(self->model, MODEL_ATTRIBUTE_TEXCO, i*4+3, texco);
        }
    }

    void Render()
    {
        Mat4x4Push(main_cam->viewMat);
        Mat4x4Translate(main_cam->viewMat, -self->model->transform->pos.x, 
            -self->model->transform->pos.y, self->model->transform->pos.z);

        /* Bind the models' vertex attribute object. */
        glBindVertexArray(self->model->vao);
        /* use the model's material's shader */
        glUseProgram(self->material->program);

        /* set matrices */
        glUniformMatrix4fv(self->material->modelMatrixID, 1, GL_FALSE, 
                main_cam->modelMat);
        glUniformMatrix4fv(self->material->viewMatrixID, 1, GL_FALSE, 
                main_cam->viewMat);
        glUniformMatrix4fv(self->material->projectionMatrixID, 1, GL_FALSE, 
                main_cam->projectionMat);

        /* bind any samplers (textures) the material uses */
        if(self->material->texture.id != 0) {
            glUniform1i(self->material->texture.loc, 0); /* TODO: use glProgramUniform in material.c */
            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, self->material->texture.id);
            glBindSampler(0, self->material->texture.sampler); 
        }

        /* bind attribute array and draw */
        glBindVertexArray(self->model->vao);
        glDrawArrays(self->model->primitive, 0, 
                self->model->numVertices);
        glBindVertexArray(0);

        Mat4x4Pop(main_cam->viewMat);
    }
}


