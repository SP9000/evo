#include "app.h"
///////////////////////////////////////////////////////////////////////////////
ENTITY(grid, 0,0,0.0f, 1,1,1, 10,0,0)
	tv_vector3 color = {1.0f, 0.0f, 0.0f};
ADDCOMPONENT(app_grid, my_grid) 
ADDCOMPONENT(tv_line_renderer, renderer)
	app_grid_set_width(my_grid, 5);
	app_grid_set_color(my_grid, color);
ENTITY_END
///////////////////////////////////////////////////////////////////////////////
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
ENTITY_END
///////////////////////////////////////////////////////////////////////////////
ENTITY(fairy2, 0.0f,0.0f,-5.0f, 1,1,1, 0,0,0)
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_model, model)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(app_player_motor, pm)
	tv_model_load_ply(model, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\TestAssets\\fairy.ply");
	tv_model_optimize(model, TRUE, TRUE);
	tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\test.mat");
	pm->speed = 0.5f;
ENTITY_END
///////////////////////////////////////////////////////////////////////////////
ENTITY(text, 0,0,0, 1,1,1, 0,0,0)
	tv_rect rect = {0.0f,0.0f,0.1f,0.1f};
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(tv_gui_renderer, text_r)
tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\tex.mat");
text_r->rect = rect;
text_r->font = "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\TestAssets\\font.bmp";
tv_text_renderer_set_text((tv_text_renderer*)text_r, "HELLO WORLD!");
ENTITY_END
///////////////////////////////////////////////////////////////////////////////
ENTITY(terrain, 0.0f,0.0f,-10.0f, 1,1,1, 0,0,0)
ADDCOMPONENT(app_terrain, terr)
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_material, material)
tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\test.mat");
ENTITY_END
///////////////////////////////////////////////////////////////////////////////
ENTITY(cc, 0,0,0, 1,1,1, 0,0,0)
ADDCOMPONENT(app_camera_controller, c)
app_camera_controller_set_pan_speed(c, 5.25f);
ENTITY_END
///////////////////////////////////////////////////////////////////////////////
ENTITY(gui, 0,0,-1.0f, 1,1,1, 0,0,0)
	tv_rect rect = {0.0f,0.0f,0.1f,0.1f};
	tv_vector4 text_color = {1.0f, 1.0f, 1.0f, 0.8f};
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(tv_gui_renderer, renderer)
ADDCOMPONENT(tv_widget, base_widget)
ADDCOMPONENT(tv_widget_stats, stats)
ADDCOMPONENT(app_hud, hud)
//ADDCOMPONENT(tv_widget_frame, frame)
tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\line.mat");
tv_widget_textbox_set_text_color((tv_widget_textbox*)stats, text_color);
tv_widget_add_child(base_widget, (tv_widget*)hud);
tv_widget_add_child(base_widget, (tv_widget*)stats);
ENTITY_END
///////////////////////////////////////////////////////////////////////////////
ENTITY(cursor, 0.5,0.125,-10.5, 1,1,1, 0,0,0)
ADDCOMPONENT(tv_overlay_renderer, renderer)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(app_cursor, curr)
tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\cursor.mat");
ENTITY_END

void app_update() 
{
	tv_vector3 v = fairy2()->transform.pos;
	tv_vector4 v2 = {1.0, v.x, v.y, 1.0f};
}

void test()
{
	tv_entity *gride = grid();
	//tv_entity *f = fairy();
	tv_entity *f2 = fairy2();
	tv_entity *test_text = text();
	tv_entity *test_gui = gui();
	tv_entity *my_terrain = terrain();
	tv_entity *my_camera_controller = cc();
	tv_entity *my_cursor = cursor();
	main_cam->pos.x = 0.0f;
	main_cam->pos.z = -5.0f;
	main_cam->pos.y = 0.0f;
	main_cam->rot.x = 0.0f;
	main_cam->rot.y = 0.0f;
	main_cam->rot.z = 0.0f;
}

int main(int argc, char** argv)
{
	puts("invoked"); fflush(stdout);
	tv_client_init();
	tv_client_set_update_func(app_update);
	app_register();
	
	test();

	tv_client_start();
	return 0;
}

