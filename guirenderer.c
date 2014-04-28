#include "guirenderer.h"

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

HANDLER_NEW(tv_gui_renderer, tv_renderer, render, TV_UPDATE_STAGE_GUI_RENDER)
	utstring_new(self->text);
	self->font_size = 0.025f;
	self->font_texture = tv_texture_new();
	self->base.render_func = render;
	self->base.material = NULL;
END_HANDLER_NEW(tv_gui_renderer)

HANDLER_START(tv_gui_renderer)
    if(self->font != NULL) {
        self->font_texture = tv_texture_load_bmp((char*)self->font);
    }
	if(self->base.material == NULL) {
		self->base.material = (tv_material*)tv_component_get((tv_component*)self, tv_material_id());
	}
END_HANDLER_START

HANDLER_UPDATE(tv_gui_renderer)
END_HANDLER_UPDATE

static void render(tv_component *self)
{
	tv_gui_renderer *renderer = (tv_gui_renderer*)self;
	tv_vector3 pos = self->entity->transform.pos;
	tv_vector3 scale = self->entity->transform.scale;

	GLint loc;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	tv_mat4x4_push(tv_camera_gui->view_mat);
	tv_mat4x4_load_identity(tv_camera_gui->view_mat);
	tv_mat4x4_translate(tv_camera_gui->view_mat, -pos.x, -pos.y, -1.0f); //pos.z);
	tv_camera_gui->view_mat[0] *= scale.x;
    tv_camera_gui->view_mat[5] *= scale.y;

    /* use the model's material's shader */
    glUseProgram(renderer->base.material->program);

    /* set matrices */
	glUniformMatrix4fv(renderer->base.material->model_mat, 1, GL_FALSE, 
		tv_camera_gui->model_mat);
    glUniformMatrix4fv(renderer->base.material->view_mat, 1, GL_FALSE, 
		tv_camera_gui->view_mat);
	glUniformMatrix4fv(renderer->base.material->projection_mat, 1, GL_FALSE, 
        tv_camera_gui->projection_mat);

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