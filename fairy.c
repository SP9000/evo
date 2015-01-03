#include "app_entities.h"

ENTITY(fairy, 1.0f,0.0f,0.0f, 1,1,1, 0,0,0)
	tv_material *line_mat;
	tv_vector3 line_start = {3.0f, 2.0f, 0.0f};
	tv_vector3 line_end = {1.0f, 0.0f, 0.0f};
	tv_vector3 line_color = {1.0f, 0.0f, 0.0f};
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_model, model)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(app_player_motor, pm)
ADDCOMPONENT(tv_line_renderer, liner)
	tv_model_load_ply(model, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\TestAssets\\fairy.ply");
	tv_model_optimize(model, TRUE, TRUE);
	tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\test.mat");
	pm->speed = 0.5f;
	line_mat = tv_material_new();
	tv_material_load(line_mat, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\line.mat");
	tv_renderer_set_material((tv_renderer*)liner, line_mat);
	tv_line_renderer_set_line_width(liner, 6);
	tv_line_renderer_add_line(liner, line_start, line_end, line_color);
	tv_line_renderer_apply(liner);
	puts("FAIRY");
ENTITY_END
