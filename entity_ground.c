#include "app_entities.h"
#include "app_terrain.h"

ENTITY(test_ground, 0,-2.5f,0, 1,1,1, 90,0,0)
ADDCOMPONENT(terrain_ground_details, ground_deets)
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_model, model)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(TV_collider, collider)
	tv_model* grass_mesh = tv_model_new();

	tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\test.mat");

	/* load/set the grass blaade model */
	tv_model_load_ply(grass_mesh, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\StdAssets\\Models\\quad.ply");
	terrain_ground_details_set_grass_mesh(ground_deets, grass_mesh);

	tv_model_renderer_set_model(renderer, model);

	/* create the collider */
	tv_collider_quad(collider, tv_vector2_new(10, 10));

	/* tag this entity as part of the terrain */
	tv_entity_add_tag(e, APP_TAG_TERRAIN);
ENTITY_END
