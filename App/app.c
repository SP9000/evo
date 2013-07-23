#include "app.h"

void App_Start()
{
    int i;

    /* background */
    Entity* bg = Entity_New();
    Component_Transform* t1 = Component_Transform_New();
        t1->pos.x = 10; t1->pos.y = 10; t1->pos.z = -1;
        t1->scale.x = 20.0f; t1->scale.y = 20.0f; t1->scale.z = 1.0f;
    Component_Model* mo1 = Component_Model_New();
        mo1->file = "StdAssets/Models/quad.ply";
    Component_Material* m1 = Component_Material_New();
        m1->file = "test.mat";
    Component_ModelRenderer* mr1 = Component_ModelRenderer_New();
        mr1->layer = RENDER_LAYER_MAIN;
    Entity_AddComponent(bg, (Component*)t1);
    Entity_AddComponent(bg, (Component*)mo1);
    Entity_AddComponent(bg, (Component*)m1);
    Entity_AddComponent(bg, (Component*)mr1);
    Entity_Start(bg);

    /* monkey */
    Entity* monkey = Entity_New();
    Component_Transform* t2 = Component_Transform_New();
        t2->pos.x = 0; t2->pos.y = 0; t2->pos.z = 1;
        t2->scale.x = 1; t2->scale.y = 1; t2->scale.z = 1;
    Component_Model* mo2 = Component_Model_New();
        mo2->file = "TestAssets/monkey.ply";
    Component_Material* m2 = Component_Material_New();
        m2->file = "test.mat";
    Component_PlayerMotor* pm = Component_PlayerMotor_New();
        pm->fallSpeed = 0; pm->jumpHeight = 0; pm->speed = 0.1f;
    Component_ModelRenderer* mr2 = Component_ModelRenderer_New();
        mr2->layer = RENDER_LAYER_MAIN;
    Entity_AddComponent(monkey, (Component*)t2);
    Entity_AddComponent(monkey, (Component*)mo2);
    Entity_AddComponent(monkey, (Component*)m2);
    Entity_AddComponent(monkey, (Component*)pm);
    Entity_AddComponent(monkey, (Component*)mr2);
    Entity_Start(monkey);

    /* scene camera */
    Entity* cam = Entity_New();
    Component_Transform* t = Component_Transform_New();
        t->pos.x = 0; t->pos.y = 0; t->pos.z = -7;
        t->scale.x = 1; t->scale.y = 1; t->scale.z = 1;
    Component_Camera* c = Component_Camera_New();
        c->render_layers = 1<<RENDER_LAYER_MAIN;
    Entity_AddComponent(cam, (Component*)t);
    Entity_AddComponent(cam, (Component*)c);
    Entity_Start(cam);
    c->Perspective(c, 60.0f, (float)screen->w/(float)screen->h,
            0.01f, 100.0f);

    /* GUI camera */
    Entity* gui_cam = Entity_New();
    Component_Transform* t4 = Component_Transform_New();
        t4->pos.x = 0; t->pos.y = 0; t->pos.z = -7;
        t4->scale.x = 1; t->scale.y = 1; t->scale.z = 1;
    Component_Camera* c2 = Component_Camera_New();
        c2->render_layers = 1<<RENDER_LAYER_GUI;
    Entity_AddComponent(gui_cam, (Component*)t4);
    Entity_AddComponent(gui_cam, (Component*)c2);
    Entity_Start(gui_cam);
    c2->Orthographic(c2, 0.0f, 1.0f, 0.0f, 1.0f, 0.01f, 100.0f);

    ModelGen_Charset();
    ModelGen_GUI();

    Entity* a = Entity_New();
    Component_Transform* t3 = Component_Transform_New();
        t3->pos.x = 0; t3->pos.y = 0; t3->pos.z = 0;
        t3->scale.x = 1; t3->scale.y = 1; t3->scale.z = 1;
    Component_Material* m3 = Component_Material_New();
        m3->file = "tex.mat";
        m3->Start(m3);
        m3->file = NULL;
    Component_TextRenderer* tr1 = Component_TextRenderer_New();
        tr1->font = "TestAssets/font.bmp";
        tr1->init_text = "HELLO WORLD";
        tr1->layer = RENDER_LAYER_GUI;
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

