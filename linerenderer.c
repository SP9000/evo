#include "linerenderer.h"

typedef struct my_vertex {
	tv_vector4 pos;
	tv_vector4 col;
}my_vertex;
static tv_model_attribute vertex_properties[] = {
	{TV_MODEL_PROPERTY_FLOAT, 4, 0},
	{TV_MODEL_PROPERTY_FLOAT, 4, 0}
};
void tv_line_renderer_add_line(tv_line_renderer *self, tv_vector3 start, tv_vector3 end, tv_vector3 color)
{
	my_vertex v1 = {start.x, start.y, start.z, 1.0f, color.x, color.y, color.z, 1.0f};
	my_vertex v2 = {end.x, end.y, end.z, 1.0f, color.x, color.y, color.z, 1.0f};

	/* if this renderer doesn't have a mode for whatever reason, make a new one */
	if(self->model == NULL) {
		self->model = tv_model_new();
		tv_model_vertex_format(self->model, 2, vertex_properties);
	}
	
	/* append the vertices and indices to the line renderer's model. */
	tv_model_append_vertex(self->model, &v1);
	tv_model_append_vertex(self->model, &v2);
}

void tv_line_renderer_apply(tv_line_renderer* self)
{
	/* optimize the model to buffer all changes to the vertices and indices */
	if(self->model) {
		tv_model_optimize(self->model, TRUE, TRUE);
	}
}

void tv_line_renderer_set_line_width(tv_line_renderer* self, tvuint line_width)
{
	self->line_width = line_width;
}
void tv_line_renderer_clear(tv_line_renderer* self)
{
	tv_model_free(self->model);
	self->model = tv_model_new();
	tv_model_vertex_format(self->model, 2, vertex_properties);
}

HANDLER_NEW(tv_line_renderer, tv_renderer, render, TV_UPDATE_STAGE_MAIN_RENDER)
	tv_component *c = (tv_component*)self;
	self->model = NULL;
	self->base.material = NULL;
	self->base.render_func = render;
END_HANDLER_NEW(tv_line_renderer)

HANDLER_START(tv_line_renderer)
	if(self->base.material == NULL) {
		self->base.material = (tv_material*)tv_component_get((tv_component*)self, tv_material_id());
	}
	if(self->model == NULL) {
		self->model = tv_model_new();
		tv_model_vertex_format(self->model, 2, vertex_properties);
	}
END_HANDLER_START

HANDLER_UPDATE(tv_line_renderer)
END_HANDLER_UPDATE

static void render(tv_component* self)
{
	tv_line_renderer *renderer = (tv_line_renderer*)self;
	if(renderer->model == NULL) {
		return;
	}
	tv_mat4x4_push(main_cam->modelview_mat);
	tv_mat4x4_scale(&main_cam->modelview_mat, self->entity->transform.scale.x, 
		self->entity->transform.scale.y, 
		self->entity->transform.scale.z);
	tv_mat4x4_translate(&main_cam->modelview_mat, self->entity->transform.pos.x + main_cam->pos.x,
		self->entity->transform.pos.y + main_cam->pos.y, self->entity->transform.pos.z + main_cam->pos.z);
	tv_mat4x4_rotate(&main_cam->modelview_mat, -self->entity->transform.rot.x, 1.0f, 0.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat, -self->entity->transform.rot.y, 0.0f, 1.0f, 0.0f);
	tv_mat4x4_rotate(&main_cam->modelview_mat, -self->entity->transform.rot.z, 0.0f, 0.0f, 1.0f);

	//TODO: ?
	//tv_mat4x4_translate(&main_cam->modelview_mat, -self->entity->transform.pos.x - main_cam->pos.x,
	//	-self->entity->transform.pos.y - main_cam->pos.y, -self->entity->transform.pos.z - main_cam->pos.z);

    /* Bind the models' vertex attribute object. */
    glBindVertexArray(renderer->model->vao);
    /* use the model's material's shader */
    glUseProgram(renderer->base.material->program);

    /* set matrices */
	glUniformMatrix4fv(renderer->base.material->modelview_mat, 1, GL_FALSE, 
            tv_mat4x4_to_array(&main_cam->modelview_mat));
	glUniformMatrix4fv(renderer->base.material->projection_mat, 1, GL_FALSE, 
			tv_mat4x4_to_array(&main_cam->projection_mat));

	glLineWidth((GLfloat)renderer->line_width);
    glBindVertexArray(renderer->model->vao);
	tv_draw_arrays(GL_LINES, 0, utarray_len(renderer->model->vertices));
    glBindVertexArray(0);

	main_cam->modelview_mat = tv_mat4x4_pop();
}
