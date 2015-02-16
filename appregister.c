#include "App/app.h"

void app_register()
{
	app_player_motor_register();
	app_grid_register();
	app_camera_controller_register();
	app_cursor_register();
	app_hud_register();

	app_unit_register();
	app_ability_register();
	app_ability_move_register();

	APP_mass_register();
	APP_velocity_register();
	APP_projectile_register();
	APP_laser_register();
	APP_gun_register();

	APP_spawner_register();
	APP_sine_spawner_register();
	APP_story_point_register();

	APP_sine_move_register();
}