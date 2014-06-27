#include "App/app.h"

void app_register()
{
	app_player_motor_register();
	app_grid_register();
	app_terrain_register();
	app_camera_controller_register();
	app_terrain_cursor_register();
	app_cursor_register();
	app_hud_register();
	app_unit_register();

	app_ability_register();
	app_move_register();
}