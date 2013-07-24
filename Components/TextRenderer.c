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
    public Rect* rect;
    public char* font;
    public float font_size;
    public char* init_text;

    getset GString* text;

    Component_Model* model;
    Component_Transform* transform;
    Texture font_texture;

    void Start() 
    {
        self->font_size = 0.025f;
        self->transform = Component_GetAs(Transform);
        self->material = Component_GetAs(Material);
        if(self->font != NULL) {
            self->font_texture = Texture_LoadBMP(self->font);
        }
        self->text = g_string_new(NULL);
        if(self->init_text != NULL) {
            self->SetText(self, self->init_text);
        }
    }
    void Update() 
    {
        
    }
    void Collide(Entity* e)
    {
        
    }

    public void SetText(char* new_text)
    {
        int i;
        float texco[2];
        float vertex[3];
        float uv_w = 1.0f / 16.0f;
        float uv_h = 1.0f / 16.0f;
        float ch_w = self->font_size;
        float ch_h = self->font_size;
        
        g_string_assign(self->text, new_text);
        self->model = Component_Model_New(NULL);
        self->model->Start(self->model);

        vertex[0] = 0.0f;
        vertex[1] = 0.0f;
        vertex[2] = -1.0f;
        self->model->numVertices = 4 * self->text->len;
        self->model->AddAttribute(self->model, MODEL_ATTRIBUTE_VERTEX);
        self->model->AddAttribute(self->model, MODEL_ATTRIBUTE_TEXCO);

        for(i = 0; i < self->text->len; ++i) {
            /* upper left */
            texco[0] = (self->text->str[i] % 16) * uv_w;
            texco[1] = (self->text->str[i] / 16) * uv_h;
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

            /* move to the next line? */
            if(vertex[0] < self->rect->w) {
                vertex[1] -= ch_h;
                vertex[0] += ch_w;
            }
            else {
                vertex[0] = 0.0f;
            }
        }
        self->model->primitive = GL_QUADS;
        self->model->Optimize(self->model);
    }

    public void Render()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        Mat4x4Push(&main_cam->viewMat);
        Mat4x4LoadIdentity(&main_cam->viewMat);
        Mat4x4Translate(&main_cam->viewMat, -self->transform->pos.x, 
            -self->transform->pos.y, -1.0f); //self->transform->pos.z);
        main_cam->viewMat.a00 *= self->transform->scale.x;
        main_cam->viewMat.a11 *= self->transform->scale.y;

        /* use the model's material's shader */
        glUseProgram(self->material->program);

        /* set matrices */
        glUniformMatrix4fv(self->material->modelMatrixID, 1, GL_FALSE, 
                Mat4x4Pack(&main_cam->modelMat));
        glUniformMatrix4fv(self->material->viewMatrixID, 1, GL_FALSE, 
                Mat4x4Pack(&main_cam->viewMat));
        glUniformMatrix4fv(self->material->projectionMatrixID, 1, GL_FALSE, 
                Mat4x4Pack(&main_cam->projectionMat));

        /* bind the font texture */
        GLint loc = glGetUniformLocation(self->material->program, "tex");
        glActiveTexture(GL_TEXTURE0 + 0);
        glUniform1i(loc, 0); 
        glBindTexture(GL_TEXTURE_2D, self->font_texture.id);
        glEnable(GL_TEXTURE_2D);

        /* bind attribute array and draw */
        glBindVertexArray(self->model->vao);
        glDrawArrays(self->model->primitive, 0, 
                self->model->numVertices);
        glBindVertexArray(0);

        Mat4x4Pop(&main_cam->viewMat);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

