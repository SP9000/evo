#include "app_entities.h"

ENTITY(fairy2, 0.0f,0.0f,-5.0f, 1,1,1, 0,0,0)
//ADDCOMPONENT(tv_model_renderer, renderer)
//ADDCOMPONENT(tv_model, model)
ADDCOMPONENT(tv_animation_renderer, renderer)
ADDCOMPONENT(tv_animation, animation)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(app_player_motor, pm)
	//tv_entity *move = ability_move(e);
	tv_model *model = tv_model_new();
	tv_animation_bone bone = {{3,0,0},{0.0f,0.0f,45.0f,0},model,material, TV_ANIMATION_BONE_END, TV_ANIMATION_BONE_END};
	tv_animation_bone bone2 = {{3,0,0},{0,0,0,0},model,material, TV_ANIMATION_BONE_END, TV_ANIMATION_BONE_END};
	tv_animation_bone bone3 = {{6,0,0},{0,0,-90.0f,0},model,material, TV_ANIMATION_BONE_END, TV_ANIMATION_BONE_END};
	tv_animation_bone bone4 = {{3,0,0},{0,0,0,0},model,material, TV_ANIMATION_BONE_END, TV_ANIMATION_BONE_END};
	tvuint b1id, b2id;

	tv_model_load_ply(model, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\TestAssets\\fairy.ply");
	tv_model_optimize(model, TRUE, TRUE);
	tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\test.mat");
	tv_animation_set_root(animation, model, material);

	b1id = tv_animation_add_bone(animation, 0, bone);
	tv_animation_add_bone(animation, b1id, bone2);
	b2id = tv_animation_add_bone(animation, b1id, bone3);
	tv_animation_add_bone(animation, b2id, bone4);

	pm->speed = 0.5f;
	//tv_entity_add_child(e, move);
ENTITY_END
