#include "..\app_scenes.h"

void app_update() 
{
	tv_vector3 v = fairy2()->transform.pos;
	tv_vector4 v2 = {1.0, v.x, v.y, 1.0f};
}

void test()
{
	APP_SCENE_desert1();
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
