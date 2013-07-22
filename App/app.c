#include "app.h"

void App_Start()
{
    int i;

    /*
    Component_Model* font[26];
    for(i = 0; i < 26; ++i) {
        char dir[] = "StdAssets/Models/Charset/";
        char file[31];
        char path[64];
        strcpy(file, " .ply");
        file[0] = 'A' + i;
        strcpy(path, dir);
        strcat(path, file);

        font[i] = Component_Model_New();
        font[i]->file = path;
        font[i]->Start(font[i]);
        font[i]->file = NULL;
    }
    */

    /* background */
    Entity* bg = Entity_New();
    Component_Transform* t1 = Component_Transform_New();
        t1->pos.x = 50; t1->pos.y = 50; t1->pos.z = -10;
        t1->scale.x = 100.0f; t1->scale.y = 100.0f; t1->scale.z = 1.0f;
    Component_Model* mo1 = Component_Model_New();
        mo1->file = "StdAssets/Models/quad.ply";
    Component_Material* m1 = Component_Material_New();
        m1->file = "test.mat";
    Entity_AddComponent(bg, (Component*)t1);
    Entity_AddComponent(bg, (Component*)mo1);
    Entity_AddComponent(bg, (Component*)m1);
    Entity_AddComponent(bg, Component_ModelRenderer_New());
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
    Entity_AddComponent(monkey, (Component*)t2);
    Entity_AddComponent(monkey, (Component*)mo2);
    Entity_AddComponent(monkey, (Component*)m2);
    Entity_AddComponent(monkey, (Component*)pm);
    Entity_AddComponent(monkey, Component_ModelRenderer_New());
    Entity_Start(monkey);

    /* camera */
    Entity* cam = Entity_New();
    Component_Transform* t = Component_Transform_New();
    t->pos.x = 0; t->pos.y = 0; t->pos.z = -7;
    t->scale.x = 1; t->scale.y = 1; t->scale.z = 1;
    Component_Camera* c = Component_Camera_New();
    Entity_AddComponent(cam, (Component*)t);
    Entity_AddComponent(cam, (Component*)c);
    Entity_Start(cam);
    c->Perspective(c, 60.0f, (float)screen->w/(float)screen->h,
            0.01f, 100.0f);
    Draw_SetCamera(c);

    ModelGen_Charset();
    ModelGen_GUI();
    return;

    /*
    Entity* a = Entity_New();
    Component_Transform* t3 = Component_Transform_New();
        t3->pos.x = 0; t3->pos.y = 0; t3->pos.z = 3;
        t3->scale.x = 1; t3->scale.y = 1; t3->scale.z = 1;
    Component_Text* text = Component_Text_New();
        text->text = "HELLO WORLD";
        text->font = font;
    Component_Material* m3 = Component_Material_New();
        m3->file = "test.mat";
        m3->Start(m3);
        m3->file = NULL;
    Entity_AddComponent(a, (Component*)t3);
    Entity_AddComponent(a, (Component*)text);
    Entity_AddComponent(a, (Component*)m3);
    Entity_Start(a);
    */
}

void App_Update()
{

}

void App_Quit()
{

}

