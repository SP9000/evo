#include "app_prefabs.h"
#include "app_resources.h"

PREFAB(prefab_doomsphere)
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_model, model)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(TV_collider, collider)
ADDCOMPONENT(tv_ai_behavior_direct, ai)
ADDCOMPONENT(app_unit, unit)
	tv_model_load_ply(model, RSC_SPHERE);
	tv_model_optimize(model, TRUE, TRUE);
	tv_material_load(material, RSC_TEST_MAT);
	tv_collider_sphere(collider, 1.0f);

	/* the doomsphere crawls along */
	tv_ai_behavior_direct_set_speed(ai, 0.2f);

	tv_ai_behavior_direct_target_pos(ai, tv_vector3_new(10.0f, 1.0f, -10.0f));

	/* the doomsphere is loyal to the doomsphere */
	app_unit_set_allied_allegiances(unit, APP_UNIT_ALLEGIANCE_DOOMSPHERE);
	/* the doomsphere is NOT friendly with the player */
	app_unit_set_enemy_allegiances(unit, APP_UNIT_ALLEGIANCE_PLAYER);
ENTITY_END
