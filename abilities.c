#include "abilities.h"

/*
ENTITY(move_, 0,0,0, 1,1,1, 1,1,1)
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_material, mat)
ADDCOMPONENT(tv_model, model)
ADDCOMPONENT(app_move, move)
	tv_model_load_ply(model, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\App\\AbilityAssets\\move.ply");
	tv_material_load(mat, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\line.mat");
	tv_model_optimize(model, TRUE, TRUE);
	model->primitive = GL_LINE_LOOP;
	move->speed = 0.5f;
	((app_ability*)move)->target.target.x = 22.0f;
	((app_ability*)move)->target.target.y = 0.0f;
	((app_ability*)move)->target.target.z = 0.0f;
ENTITY_END

tv_entity *ability_move(tv_entity *user) 
{
	tv_entity *m = move_();
	//app_ability_set_user((app_ability*)tv_entity_get_component(m, app_move_id()), (app_unit*)tv_entity_get_component(user, app_unit_id()));
	return m;
}
*/