#include "hud.h"

typedef struct {
	tv_vector3 pos;
	tv_vector4 color;
}hud_vertex;

static tv_model_attribute vertex_attributes[] = {
	{TV_MODEL_PROPERTY_FLOAT, 3, 0},
	{TV_MODEL_PROPERTY_FLOAT, 4, 3*sizeof(TV_MODEL_PROPERTY_FLOAT)}
};

hud_vertex hud_vertices[] = {
	/* selected units bar outline */
	{0.0f, 0.87f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	{0.78f, 0.84f, 0.0f,	0.9f,0.0f,0.5f,1.0f},
	{0.78f, 1.0f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	{0.78f, 1.0f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	{0.0f, 1.0f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	{0.0f, 0.87f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	/* actions menu outline */
	{0.78f, 0.67f, 0.0f,	0.9f,0.0f,0.5f,1.0f},
	{1.0f, 0.67f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	{1.0f, 1.0f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	{1.0f, 1.0f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	{0.78f, 1.0f, 0.0f,		0.9f,0.0f,0.5f,1.0f},
	{0.78f, 0.67f, 0.0f,	0.9f,0.0f,0.5f,1.0f},

	/* selected units bar */
	{0.0f, 0.89f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{0.8f, 0.85f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{0.8f, 1.0f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{0.8f, 1.0f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{0.0f, 1.0f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{0.0f, 0.89f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	/* actions menu */
	{0.8f, 0.7f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{1.0f, 0.7f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{1.0f, 1.0f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{1.0f, 1.0f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{0.8f, 1.0f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
	{0.8f, 0.7f, 0.0f,		0.6f,0.0f,0.5f,1.0f},
};

tv_rect ability_rects[16] = {
	{0.8f, 0.7f, 0.05f, 0.05f},
	{0.85f, 0.7f, 0.05f, 0.05f},
	{0.9f, 0.7f, 0.05f, 0.05f},
	{0.95f, 0.7f, 0.05f, 0.05f},

	{0.8f, 0.75f, 0.05f, 0.05f},
	{0.85f, 0.75f, 0.05f, 0.05f},
	{0.9f, 0.75f, 0.05f, 0.05f},
	{0.95f, 0.75f, 0.05f, 0.05f},

	{0.8f, 0.8f, 0.05f, 0.05f},
	{0.85f, 0.8f, 0.05f, 0.05f},
	{0.9f, 0.8f, 0.05f, 0.05f},
	{0.95f, 0.8f, 0.05f, 0.05f},

	{0.8f, 0.85f, 0.05f, 0.05f},
	{0.85f, 0.85f, 0.05f, 0.05f},
	{0.9f, 0.85f, 0.05f, 0.05f},
	{0.95f, 0.85f, 0.05f, 0.05f},
};
	
COMPONENT_NEW(app_hud, tv_widget)
	tv_widget *w = (tv_widget*)self;
	hud_vertex v;
	tvuint i;
	tv_model *model = tv_model_new();
	tv_widget_init((tv_widget*)self);

	/* the main HUD background */
	tv_model_vertex_format(model, 2, vertex_attributes);
	for(i = 0; i < 24; ++i) {
		v = hud_vertices[i];
		tv_model_append_vertex(model, &v);
	}
	tv_animation_set_root(self->animation, model, w->material);
	/* the thumbnails */
	for(i = 0; i < 16; ++i) {
		app_hud_set_ability_thumbnail(self, NULL, i);
	}

	model->primitive = GL_TRIANGLES;
	tv_model_optimize(model, TRUE, FALSE);
	tv_widget_set_model(w, model);
END_COMPONENT_NEW(app_hud)

COMPONENT_START(app_hud)
	tv_widget_set_material((tv_widget*)self, (tv_material*)tv_component_get(self_component, tv_material_id()));
END_COMPONENT_START

COMPONENT_UPDATE(app_hud)
END_COMPONENT_UPDATE

void app_hud_set_ability_thumbnail(app_hud *hud, tv_model *thumbnail, tvuint index)
{
	tv_animation_bone bone;
	bone.model = thumbnail;
	bone.material = hud->base.material;
	bone.position.x = app_hud_thumbnail_region().x + ((tvfloat)(index % 4) * app_hud_thumbnail_dim().x);
	bone.position.y = app_hud_thumbnail_region().y + ((tvfloat)(index / 4) * app_hud_thumbnail_dim().y);
	bone.position.z = 1.0f;
	tv_animation_replace_bone(hud->animation, hud->thumbnails[index], bone);
}