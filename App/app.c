#include "app.h"


void app_update() 
{

}

void test()
{
	/*
	TvEntity *test_e;	
	tv_vector3 transform1_pos = {10.0f,10.0f,-7.0f};
	tv_vector3 transform1_scale = {20.0f,20.0f,1.0f};
	tv_model* model1 = tv_model_new();
	tv_material *mat1 = tv_material_new();
	tv_model_renderer *renderer = tv_model_renderer_new();
	app_player_motor *pm = app_player_motor_new();
	pm->speed = 0.5f;

	tv_model_load_ply(model1, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\TestAssets\\monkey.ply");
	tv_model_optimize(model1);
	tv_material_load(mat1, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\test.mat");
	tv_model_renderer_set_model(renderer, model1);
	
	test_e = tv_entity_new();
	tv_entity_add_component(test_e, (tv_component*)model1);
	tv_entity_add_component(test_e, (tv_component*)mat1);
	tv_entity_add_component(test_e, (tv_component*)renderer);
	tv_entity_add_component(test_e, (tv_component*)pm);
	*/

	TvEntity *test_e;
	TvRect trect = {0.0f,0.0f,0.1f,0.1f};
	
	tv_vector3 transform1_pos = {10.0f,10.0f,-7.0f};
	tv_vector3 transform1_scale = {20.0f,20.0f,1.0f};
	tv_material *mat1 = tv_material_new();
	tv_text_renderer *tr = tv_text_renderer_new();
	app_player_motor *pm = app_player_motor_new();
	pm->speed = 0.05f;

	tv_material_load(mat1, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\tex.mat");
	tr->font = "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\TestAssets\\font.bmp";
	tr->rect = trect;
	tv_text_renderer_set_text(tr, "HELLO WORLD!");
	tv_camera_orthographic(main_cam, 0.0f, 1.0f, 0.0f, 1.0f, 0.01f, 100.0f);

	test_e = tv_entity_new();
	tv_entity_add_component(test_e, (tv_component*)mat1);
	tv_entity_add_component(test_e, (tv_component*)tr);
	tv_entity_add_component(test_e, (tv_component*)pm);

	tv_entity_start(test_e);
}

int main(int argc, char** argv)
{
	puts("invoked"); fflush(stdout);
	tv_client_init();
	tv_client_set_update_func(app_update);

	app_player_motor_register();

	test();
	tv_client_start();
	return 0;
}

#if 0
void App_Start()
{

	Entity* bg;
	Entity* monkey;
	Entity* a;
	Entity* cam;
	Entity* gui_cam;

	Component_Transform* transform1;
	Vector3 transform1_pos = {10.0f,10.0f,-1.0f};
	Vector3 transform1_scale = {20.0f,20.0f,1.0f};
	Component_Model* model1;
	Component_Material* material1;
	Component_ModelRenderer* modelrenderer1;

	Component_Transform* t2;

	Component_Transform* t3;

	ModelGen_Charset();
    ModelGen_GUI();

    /* background */
    bg = Entity_New();
    transform1 = Component_Transform_New(transform1_pos, transform1_scale);
    model1 = Component_Model_New("StdAssets/Models/quad.ply");
    material1 = Component_Material_New("test.mat");
    modelrenderer1 = Component_ModelRenderer_New(RENDER_LAYER_MAIN);
    Entity_AddComponent(bg, (Component*)transform1);
    Entity_AddComponent(bg, (Component*)model1);
    Entity_AddComponent(bg, (Component*)material1);
    Entity_AddComponent(bg, (Component*)modelrenderer1);
    Entity_Start(bg);
}

    /* monkey */
    monkey = Entity_New();
    Component_Transform* t2 = Component_Transform_New(
            (Vector3){0.0f,0.0f,1.0f},   /* position */
            (Vector3){1.0f,1.0f,1.0f});  /* scale */
    Component_Model* mo2 = Component_Model_New("TestAssets/monkey.ply");
    Component_Material* m2 = Component_Material_New("test.mat");
    Component_PlayerMotor* pm = Component_PlayerMotor_New(0,0,.1f);
    Component_ModelRenderer* mr2 = Component_ModelRenderer_New(RENDER_LAYER_MAIN);
    Entity_AddComponent(monkey, (Component*)t2);
    Entity_AddComponent(monkey, (Component*)mo2);
    Entity_AddComponent(monkey, (Component*)m2);
    Entity_AddComponent(monkey, (Component*)pm);
    Entity_AddComponent(monkey, (Component*)mr2);
    Entity_Start(monkey);

    /* scene camera */
    cam = Entity_New();
    Entity_AddComponent(cam, Component_Transform_New(
        (Vector3){0.0f,0.0f,-7.0f},   /* position */
        (Vector3){1.0f,1.0f,1.0f}));  /* scale */
    Entity_AddComponent(cam, Component_Camera_New(
            0,0,0,0,
            1<<RENDER_LAYER_MAIN,
            0));
    Entity_Start(cam);
    Entity_Get(cam, Camera)->Perspective(Entity_Get(cam, Camera), 60.0f, (float)screen->w/(float)screen->h,
            0.01f, 100.0f);

    /* GUI camera */
    gui_cam = Entity_New();
    Component_Transform* t4 = Component_Transform_New(
        (Vector3){0.0f,0.0f,-7.0f},   /* position */
        (Vector3){1.0f,1.0f,1.0f});  /* scale */
    Component_Camera* c2 = Component_Camera_New(
            0,0,0,0,
            1<<RENDER_LAYER_GUI,
            0);
    Entity_AddComponent(gui_cam, (Component*)t4);
    Entity_AddComponent(gui_cam, (Component*)c2);
    Entity_Start(gui_cam);
    c2->Orthographic(c2, 0.0f, 1.0f, 0.0f, 1.0f, 0.01f, 100.0f);

    /* lil' text */
    Rect trect = {0.0f,0.0f,0.1f,0.1f};
    Entity* a = Entity_New();
    Component_Transform* t3 = Component_Transform_New(
        (Vector3){0.0f,0.0f,0.0f},   /* position */
        (Vector3){1.0f,1.0f,1.0f});  /* scale */
    Component_Material* m3 = Component_Material_New("tex.mat");
    Component_TextRenderer* tr1 = Component_TextRenderer_New(
            RENDER_LAYER_GUI,
            &trect,
            "TestAssets/font.bmp",0,
            "HELLOWORLD");
    Entity_AddComponent(a, (Component*)t3);
    Entity_AddComponent(a, (Component*)m3);
    Entity_AddComponent(a, (Component*)tr1);
    Entity_Start(a);
}

void App_Update()
{

}

void App_Quit()
{

}

#endif