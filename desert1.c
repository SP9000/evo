#include "app_scenes.h"

void APP_SCENE_desert1()
{
	tv_entity *gride = grid();
	tv_entity *test_text = text();
	tv_entity *test_gui = gui();
	tv_entity *my_terrain = terrain();
	tv_entity *my_cursor = cursor();
	tv_entity *sp = prefab_moving_sphere();

	tv_entity *spawnpt = spawn_pt();
	tv_entity *plane = test_plane();
	tv_entity *f = fairy();
	tv_entity *f2 = fairy2();
	tv_entity *my_camera_controller = cc();
	tv_entity *ground = test_ground();

	tv_entity *doom1 = prefab_doomsphere();
	tv_entity *doom2 = prefab_doomsphere();

	tv_entity_setpos(doom1, tv_vector3_new(1.0f, 3.0f, 0.0f));
	tv_entity_setpos(doom2, tv_vector3_new(-2.0f, 3.0f, 0.0f));

	/* set the camera for the scene */
	main_cam->pos.x = 0.0f;
	main_cam->pos.z = -5.0f;
	main_cam->pos.y = 0.0f;
	main_cam->rot.x = 0.0f;
	main_cam->rot.y = 0.0f;
	main_cam->rot.z = 0.0f;
}
