#include "textrenderer.h"


HANDLER_NEW(tv_text_renderer, tv_renderer, render, 1)
END_HANDLER_NEW(tv_text_renderer)

HANDLER_START(tv_text_renderer)
    self->font_size = 0.025f;
    if(self->font != NULL) {
        self->font_texture = tv_texture_load_bmp((char*)self->font);
    }
	utstring_new(self->text);
END_HANDLER_START

HANDLER_UPDATE(tv_text_renderer)
END_HANDLER_UPDATE

static void render(tv_component *self)
{
	tv_text_renderer *renderer = (tv_text_renderer*)self;
	tv_vector3 pos = self->entity->transform.pos;
	tv_vector3 scale = self->entity->transform.scale;

	GLint loc;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	tv_mat4x4_push(main_cam->view_mat);
	tv_mat4x4_load_identity(main_cam->view_mat);
	tv_mat4x4_translate(main_cam->view_mat, -pos.x, -pos.y, -1.0f); //pos.z);
	main_cam->view_mat[0] *= scale.x;
    main_cam->view_mat[5] *= scale.y;

    /* use the model's material's shader */
    glUseProgram(renderer->base.material->program);

    /* set matrices */
	glUniformMatrix4fv(renderer->base.material->model_mat, 1, GL_FALSE, 
		main_cam->model_mat);
    glUniformMatrix4fv(renderer->base.material->view_mat, 1, GL_FALSE, 
		main_cam->view_mat);
	glUniformMatrix4fv(renderer->base.material->projection_mat, 1, GL_FALSE, 
        main_cam->projection_mat);

    /* bind the font texture */
    loc = glGetUniformLocation(renderer->base.material->program, "tex");
    glActiveTexture(GL_TEXTURE0 + 0);
    glUniform1i(loc, 0); 
    glBindTexture(GL_TEXTURE_2D, renderer->font_texture->id);
    glEnable(GL_TEXTURE_2D);

    /* bind attribute array and draw */
    glBindVertexArray(renderer->model->vao);
    glDrawArrays(renderer->model->primitive, 0, 
		renderer->model->num_vertices);
    glBindVertexArray(0);

    tv_mat4x4_pop(main_cam->view_mat);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

}

static void set_text(tv_text_renderer *self, const char *text)
{
	int i;
    float texco[2];
    float vertex[3];
    float uv_w = 1.0f / 16.0f;
    float uv_h = 1.0f / 16.0f;
    float ch_w = self->font_size;
    float ch_h = self->font_size;
        
	utstring_clear(self->text);
	utstring_printf(self->text, text);

    self->model = tv_model_new();

    vertex[0] = 0.0f;
    vertex[1] = 0.0f;
    vertex[2] = -1.0f;
    self->model->num_vertices = 4 * utstring_len(self->text);
	tv_model_add_attribute(self->model, MODEL_ATTRIBUTE_VERTEX);
    tv_model_add_attribute(self->model, MODEL_ATTRIBUTE_TEXCO);

    for(i = 0; i < utstring_len(self->text); ++i) {
        /* upper left */
        texco[0] = (utstring_body(self->text)[i] % 16) * uv_w;
        texco[1] = (utstring_body(self->text)[i] / 16) * uv_h;
		tv_model_set_attribute(self->model, MODEL_ATTRIBUTE_VERTEX, i*4, vertex);
        tv_model_set_attribute(self->model, MODEL_ATTRIBUTE_TEXCO, i*4, texco);

        /* upper right corner */
        texco[0] += uv_w;
        vertex[0] += ch_w;
        tv_model_set_attribute(self->model, MODEL_ATTRIBUTE_VERTEX, i*4+1, vertex);
        tv_model_set_attribute(self->model, MODEL_ATTRIBUTE_TEXCO, i*4+1, texco);

        /* lower right corner */
        texco[1] += uv_h;
        vertex[1] += ch_h;
        tv_model_set_attribute(self->model, MODEL_ATTRIBUTE_VERTEX, i*4+2, vertex);
        tv_model_set_attribute(self->model, MODEL_ATTRIBUTE_TEXCO, i*4+2, texco);

        /* lower left corner */
        texco[0] -= uv_w;
        vertex[0] -= ch_w;
        tv_model_set_attribute(self->model, MODEL_ATTRIBUTE_VERTEX, i*4+3, vertex);
        tv_model_set_attribute(self->model, MODEL_ATTRIBUTE_TEXCO, i*4+3, texco);

        /* move to the next line? */
        if(vertex[0] < self->rect.w) {
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
