#include "app_entities.h"
#include "app_resources.h"

ENTITY(test_gun, 0,0,0, 1,1,1, 0,0,0)
ADDCOMPONENT(APP_gun, laser_gun)
ADDCOMPONENT(APP_laser, laser)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(tv_model, model)
ADDCOMPONENT(tv_animation, animation);
laser->base.range = 100.0f;
laser->base.speed = 100.0f;
laser->base.animation = animation;
tv_model_load_ply(model, "..\\TestAssets\\fairy.ply");
tv_model_optimize(model, TRUE, TRUE);
tv_material_load(material, RSC_TEST_MAT);
tv_animation_set_root(laser->base.animation, model, material);
laser_gun->projectile = (APP_projectile*)laser;
ENTITY_END
