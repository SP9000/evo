#include "app.h"

void App_Start()
{
    ModelGen_Charset();
    ModelGen_GUI();

    /* background */
    Entity* bg = Entity_New();
    Component_Transform* t1 = Component_Transform_New(
            (Vector3){10.0f,10.0f,-1.0f},   /* position */
            (Vector3){20.0f,20.0f,1.0f});   /* scale */
    Component_Model* mo1 = Component_Model_New("StdAssets/Models/quad.ply");
    Component_Material* m1 = Component_Material_New("test.mat");
    Component_ModelRenderer* mr1 = Component_ModelRenderer_New(RENDER_LAYER_MAIN);
    Entity_AddComponent(bg, (Component*)t1);
    Entity_AddComponent(bg, (Component*)mo1);
    Entity_AddComponent(bg, (Component*)m1);
    Entity_AddComponent(bg, (Component*)mr1);
    Entity_Start(bg);

    /* monkey */
    Entity* monkey = Entity_New();
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
    Entity* cam = Entity_New();
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
    Entity* gui_cam = Entity_New();
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

