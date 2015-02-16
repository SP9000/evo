#include "app_entities.h"

ENTITY(test_ground, 0,-2.5f,0,1,1,1, 90,0,0)
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_model, model)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(TV_collider, collider)
	tv_vector2 size = {10, 10};
	tv_model_vertex vertex_format = {2, {TV_MODEL_PROPERTY_FLOAT, TV_MODEL_PROPERTY_FLOAT}, {4,4}};
	model = tv_modelgen_quad(size, vertex_format);
	tv_model_renderer_set_model(renderer, model);
	tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\cursor.mat");
	tv_collider_quad(collider, tv_vector2_new(10, 10));
	tv_entity_add_tag(e, APP_TAG_TERRAIN);
ENTITY_END
