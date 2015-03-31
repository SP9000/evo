#include "app_prefabs.h"
#include "app_resources.h"

PREFAB(fairy_prefab)
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_model, model)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(app_player_motor, pm)
ADDCOMPONENT(TV_collider, collider)
	tv_model_load_ply(model, RSC_FAIRY);
	tv_model_optimize(model, TRUE, TRUE);
	tv_material_load(material, RSC_TEST_MAT);
	pm->speed = 0.5f;
	tv_collider_sphere(collider, 1.0f);
	puts("FAIRY");
ENTITY_END
