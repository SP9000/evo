#include "widgetmodels.h"

typedef struct {
	tv_vector3 pos;
	tv_vector4 color;
}gui_vertex;

tv_model *tv_gui_model_text(tvchar *text, tv_rect bounds)
{

}

tv_model *tv_gui_model_border(tvfloat w, tvfloat h, tv_vector4 color)
{
	tvuint i;
	gui_vertex vertex;
	tv_model_property vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 3, 0},
		{TV_MODEL_PROPERTY_FLOAT, 3, 3*sizeof(TV_MODEL_PROPERTY_FLOAT)}
	};
	tv_model *model = tv_model_new();

    vertex.pos.x = 0.0f;
    vertex.pos.y = 0.0f;
    vertex.pos.z = 0.0f;
	vertex.color = color;

	tv_model_vertex_format(model, 2, vertex_properties);

	tv_model_append_vertex(model, &vertex);
	vertex.pos.x += w;
	tv_model_append_vertex(model, &vertex);
	vertex.pos.y += h;
	tv_model_append_vertex(model, &vertex);
	vertex.pos.x -= w;
	tv_model_append_vertex(model, &vertex);

	model->primitive = GL_LINE_LOOP;
	tv_model_optimize(model);

	return model;
}
