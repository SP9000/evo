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

HANDLER_NEW(tv_text_renderer, tv_renderer, render, TV_UPDATE_STAGE_MAIN_RENDER)
	utstring_new(self->text);
	self->font_size = 0.025f;
	self->font_texture = tv_texture_new();
	self->base.render_func = render;
	self->base.material = NULL;
END_HANDLER_NEW(tv_text_renderer)

HANDLER_START(tv_text_renderer)
    if(self->font != NULL) {
        self->font_texture = tv_texture_load_bmp((char*)self->font);
    }
	if(self->base.material == NULL) {
		self->base.material = (tv_material*)tv_component_get((tv_component*)self, tv_material_id());
	}
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
	glDrawElements(renderer->model->primitive, (GLsizei)utarray_len(renderer->model->indices),
		GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    tv_mat4x4_pop(main_cam->view_mat);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void tv_text_renderer_set_text(tv_text_renderer *self, const tvchar *text)
{
	tvuint i;
	tv_textrenderer_vertex vertex;
	tv_model_property vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 3, 0},
		{TV_MODEL_PROPERTY_FLOAT, 2, 0}
	};

    tvfloat uv_w = 1.0f / 16.0f;
    tvfloat uv_h = 1.0f / 16.0f;
    tvfloat ch_w = self->font_size;
    tvfloat ch_h = self->font_size;
 
	utstring_clear(self->text);
	utstring_printf(self->text, text);

    self->model = tv_model_new();

    vertex.pos.x = 0.0f;
    vertex.pos.y = 0.0f;
    vertex.pos.z = -1.0f;

	tv_model_vertex_format(self->model, 2, vertex_properties);
    for(i = 0; i < utstring_len(self->text); ++i) {
        /* upper left */
        vertex.tex.u = ((tvfloat)(utstring_body(self->text)[i] % 16)) * uv_w;
        vertex.tex.v = ((tvfloat)(utstring_body(self->text)[i] / 16)) * uv_h;
		tv_model_append_vertex(self->model, &vertex);

        /* upper right corner */
        vertex.tex.u += uv_w;
        vertex.pos.x += ch_w;
		tv_model_append_vertex(self->model, &vertex);

        /* lower right corner */
        vertex.tex.v += uv_h;
        vertex.pos.y += ch_h;
		tv_model_append_vertex(self->model, &vertex);

        /* lower left corner */
        vertex.tex.u -= uv_w;
        vertex.pos.x -= ch_w;
		tv_model_append_vertex(self->model, &vertex);

        /* move to the next line? */
        if(vertex.pos.x < self->rect.w) {
            vertex.pos.y -= ch_h;
            vertex.pos.x += ch_w;
        }
        else {
            vertex.pos.x = 0.0f;
        }
		tv_model_append_indices4(self->model, i*4, i*4+1, i*4+2, i*4+3);
    }
    self->model->primitive = GL_QUADS;
	tv_model_optimize(self->model);
}
