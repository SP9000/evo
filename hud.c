#include "hud.h"

typedef struct {
	tv_vector3 pos;
	tv_vector4 color;
}hud_vertex;

static tv_model_attribute vertex_attributes[] = {
	{TV_MODEL_PROPERTY_FLOAT, 3, 0},
	{TV_MODEL_PROPERTY_FLOAT, 4, 3*sizeof(TV_MODEL_PROPERTY_FLOAT)}
};

static hud_vertex hud_vertices[] = {
	/* selected units bar outline */
	{0.0f, 0.825f, 0.0f,		0.9f,0.0f,1.0f,1.0f},
	{0.8f, 0.825f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	{0.8f, 1.0f, 0.0f,			0.9f,0.0f,0.5f,1.0f},
	{0.8f, 1.0f, 0.0f,			0.9f,0.0f,1.0f,1.0f},
	{0.0f, 1.0f, 0.0f,			0.9f,0.0f,1.0f,1.0f},
	{0.0f, 0.825f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	/* actions menu outline */
	{0.775f, 0.675f, 0.0f,		0.8f,0.0f,0.3f,1.0f},
	{1.0f, 0.675f, 0.0f,		0.9f,0.0f,0.3f,1.0f},
	{1.0f, 1.0f, 0.0f,			0.9f,0.0f,0.3f,1.0f},
	{1.0f, 1.0f, 0.0f,			0.9f,0.0f,0.3f,1.0f},
	{0.775f, 1.0f, 0.0f,		0.8f,0.0f,0.3f,1.0f},
	{0.775f, 0.675f, 0.0f,		0.8f,0.0f,0.3f,1.0f},
	/* selected units bar */
	{0.0f, 0.85f, 0.0f,		0.5f,0.0f,0.5f,1.0f},
	{0.8f, 0.85f, 0.0f,		0.5f,0.0f,0.5f,1.0f},
	{0.8f, 1.0f, 0.0f,		0.5f,0.0f,0.5f,1.0f},
	{0.8f, 1.0f, 0.0f,		0.5f,0.0f,0.5f,1.0f},
	{0.0f, 1.0f, 0.0f,		0.5f,0.0f,0.5f,1.0f},
	{0.0f, 0.85f, 0.0f,		0.5f,0.0f,0.5f,1.0f},
	/* actions menu */
	{0.8f, 0.7f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{1.0f, 0.7f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{1.0f, 1.0f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{1.0f, 1.0f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{0.8f, 1.0f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{0.8f, 0.7f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
};

static hud_vertex ability_rects[16] = {
	{0.8f, 0.75f, -0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.85f, 0.75f, -0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.85f, 0.7f, -0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.8f, 0.7f, -0.05f,	1.0f,0.0f,1.0f,1.0f},

	{0.855f, 0.75f, -0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.905f, 0.75f, -0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.905f, 0.7f, -0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.855f, 0.7f, -0.05f,	1.0f,0.0f,1.0f,1.0f},

	{0.8f, 0.8f, 0.05f,		1.0f,0.0f,1.0f,1.0f},
	{0.85f, 0.8f, 0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.9f, 0.8f, 0.05f,		1.0f,0.0f,1.0f,1.0f},
	{0.95f, 0.8f, 0.05f,	1.0f,0.0f,1.0f,1.0f},

	{0.8f, 0.85f, 0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.85f, 0.85f, 0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.9f, 0.85f, 0.05f,	1.0f,0.0f,1.0f,1.0f},
	{0.95f, 0.85f, 0.05f,	1.0f,0.0f,1.0f,1.0f},
};
	
COMPONENT_NEW(app_hud, tv_widget)
	tvuint i;
	tv_widget *w = (tv_widget*)self;
	tv_widget_init((tv_widget*)self);
	for(i = 0; i < APP_HUD_UNIT_SLOTS; ++i) {
		self->unit_widgets[i] = NULL;
	}
END_COMPONENT_NEW(app_hud)

COMPONENT_START(app_hud)
	tv_widget *w = (tv_widget*)self;
	hud_vertex v;
	tvuint i, j;
	tv_widget *c;
	tv_model *ability_thumb_models[4];
	tv_widget *ability_thumb_widgets[4];
	tv_model *model = tv_model_new();

	/* set the material for the widget */
	tv_widget_set_material((tv_widget*)self, (tv_material*)tv_component_get(self_component, tv_material_id()));

	/* create the main HUD background widget */
	tv_model_vertex_format(model, 2, vertex_attributes);
	for(i = 0; i < 24; ++i) {
		v = hud_vertices[i];
		tv_model_append_vertex(model, &v);
	}
	/* create the model for the ability thumbnails */
	for(i = 0; i < 4; i++) {
		ability_thumb_models[i] = tv_model_new();
		ability_thumb_widgets[i] = tv_widget_new();
		tv_model_vertex_format(ability_thumb_models[i], 2, vertex_attributes);
		for(j = 0; j < 4; ++j) {
			v = ability_rects[i*4 + j];
			tv_model_append_vertex(ability_thumb_models[i], &v);
		}
		ability_thumb_models[i]->primitive = GL_QUADS;
		tv_model_optimize(ability_thumb_models[i], TRUE, FALSE);
		tv_widget_set_model(ability_thumb_widgets[i], ability_thumb_models[i]);
		tv_widget_add_child(w, ability_thumb_widgets[i]);
	}

	/* add the widgets for the units */
	for(i = 0; i < APP_HUD_UNIT_SLOTS; ++i) {
		self->unit_widgets[i] = tv_widget_new();
		self->unit_widgets[i]->scale = tv_vector3_new(0.05f, 0.05f, 0.05f);
		self->unit_widgets[i]->pos = tv_vector3_new(i*0.2f + 0.2f, 0.85f, 0.0f);
		tv_widget_add_child(w, self->unit_widgets[i]);
	}

	model->primitive = GL_TRIANGLES;
	tv_model_optimize(model, TRUE, FALSE);
	tv_widget_set_model(w, model);
END_COMPONENT_START

COMPONENT_DESTROY(app_hud)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(app_hud)
END_COMPONENT_UPDATE

void app_hud_set_unit(app_hud* hud, app_unit* unit, tvuint slot)
{
	tv_widget_set_animation(hud->unit_widgets[slot], unit->animation);
}
void app_hud_add_unit(app_hud* hud, app_unit* unit)
{
	tvuint i; 
	/* make sure unit is not already selected */
	for(i = 0; i < APP_HUD_UNIT_SLOTS; ++i) {
		if(hud->units[i] == unit) {
			return;
		}
	}
	/* add to next available slot (if there is one) */
	for(i = 0; i < APP_HUD_UNIT_SLOTS; ++i) {
		if(hud->unit_widgets[i]->animation == NULL) {
			app_hud_set_unit(hud, unit, i);
			hud->units[i] = unit;
			break;
		}
	}
}
