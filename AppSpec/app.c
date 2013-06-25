#include "app.h"

extern void App_Start();
extern void App_Update();
extern void App_Quit();

static Model* m;
static Model* background;

void App_Start()
{
    float rectColors[] = {0.0f,0.0f,1.0f,1.0f,
          0.0f,0.0f,1.0f,1.0f,
          1.0f,0.0f,0.0f,1.0f,
          1.0f,0.0f,0.0f,1.0f};

    Material* mat = Material_Load("test.mat");
    GUILayout_SetMaterial(mat);

    m = Model_New();
    Model_LoadPLY(m, "TestAssets/monkey.ply");
    Model_SetMaterial(m, mat);
    Draw_OptimizeModel(m);

    Model* rectM = GenRect(0, 0, 0, 200, 100);
    Model_BufferAttribute(rectM, MODEL_ATTRIBUTE_COLOR, rectColors);
    Rect wRect = {0,0,.4,.4};
    Model_SetMaterial(rectM, mat);
    Widget* w = GUILayout_NewTextBox(rectM, &wRect, "HELLO ALL");
    GUILayout_SetRootWidget(w);
    
    background = GenRect(-10,-10,-4,20,20);
    Model_BufferAttribute(background, MODEL_ATTRIBUTE_COLOR, rectColors);
    Model_SetMaterial(background, mat);
    Draw_OptimizeModel(background);

    Scene_Add(background);
    Scene_Add(m);

    /* COLLISION TEST */
    Node n1 = {1.0f, 0.0f, 0.0f};
    Node n2 = {1.0f, 1.0f, 1.0f};
    Node n3 = {10.0f, 0.0f, 9.0f};
    Node n4 = {11.0f, 1.5f, 9.0f};
    AABB bb = {2.0f, 2.0f, 2.0f};
    Collider c1 = {&n1, &bb, NULL};
    Collider c2 = {&n2, &bb, NULL};
    Collider c3 = {&n3, &bb, NULL};
    Collider c4 = {&n4, &bb, NULL};
    Collision_AddCollider(&c1);
    Collision_AddCollider(&c2);
    Collision_AddCollider(&c3);
    Collision_AddCollider(&c4);
    Collision_Detect();
}

void App_Update()
{
    /* Render */
    Draw_StartFrame();
    Draw_Scene();

    /* Render GUI */
    Draw_GUI();
}

void App_Quit()
{
    Model_Free(m);
    //Model_Free(background);
}

