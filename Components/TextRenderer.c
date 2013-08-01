#include "TextRenderer.h"

static void Start(TextRenderer* self);
static void Update(TextRenderer* self);
static void Render(TextRenderer* self);

void Start(TextRenderer* self)
{
    self->font_size = 0.025f;
    if(self->font != NULL) {
        self->font_texture = tv_texture_load_bmp(self->font);
    }
	utstring_new(self->text);
    if(self->init_text != NULL) {
		TextRenederer_SetText(self, self->init_text);
    }
}

static void Update(TextRenderer* self)
{
        
}

void TextRenederer_SetText(TextRenderer* self, char* new_text)
{
    int i;
    float texco[2];
    float vertex[3];
    float uv_w = 1.0f / 16.0f;
    float uv_h = 1.0f / 16.0f;
    float ch_w = self->font_size;
    float ch_h = self->font_size;
        
	utstring_printf(self->text, new_text);
    self->model = tv_model_new();

    vertex[0] = 0.0f;
    vertex[1] = 0.0f;
    vertex[2] = -1.0f;
    self->model->num_vertices = 4 * self->text->len;
	tv_model_add_attribute(self->model, MODEL_ATTRIBUTE_VERTEX);
    tv_model_add_attribute(self->model, MODEL_ATTRIBUTE_TEXCO);

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
	tv_model_optimize(self->model);
}

void Render(TextRenderer* self)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	tv_mat4x4_push(main_cam->view_mat);
	tv_mat4x4_load_identity(main_cam->view_mat);
	tv_mat4x4_load_identity(&main_cam->view_mat, -self->transform->pos.x, 
        -self->transform->pos.y, -1.0f); //self->transform->pos.z);
    main_cam->view_mat[0] *= self->transform->scale.x;
    main_cam->view_mat[5] *= self->transform->scale.y;

    /* use the model's material's shader */
    glUseProgram(self->base.material->program);

    /* set matrices */
	glUniformMatrix4fv(self->base.material->model_mat, 1, GL_FALSE, 
		main_cam->model_mat);
    glUniformMatrix4fv(self->base.material->view_mat, 1, GL_FALSE, 
		main_cam->view_mat);
	glUniformMatrix4fv(self->base.material->projection_mat, 1, GL_FALSE, 
        main_cam->projection_mat);

    /* bind the font texture */
    GLint loc = glGetUniformLocation(self->base.material->program, "tex");
    glActiveTexture(GL_TEXTURE0 + 0);
    glUniform1i(loc, 0); 
    glBindTexture(GL_TEXTURE_2D, self->font_texture.id);
    glEnable(GL_TEXTURE_2D);

    /* bind attribute array and draw */
    glBindVertexArray(self->model->vao);
    glDrawArrays(self->model->primitive, 0, 
		self->model->num_vertices);
    glBindVertexArray(0);

    tv_mat4x4_pop(main_cam->view_mat);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
