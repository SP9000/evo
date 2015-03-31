#include "app_entities.h"
#include "app_resources.h"

ENTITY(test_plane, 0,0,0, 1,1,1, 0,0,0)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(tv_animation, animation)
	tv_model *model = tv_model_new();
	tv_model_load_ply(model, RSC_FAIRY);
	tv_model_optimize(model, TRUE, TRUE);
	tv_material_load(material, RSC_TEST_MAT);
	tv_animation_set_root(animation, model, material);
ENTITY_END
