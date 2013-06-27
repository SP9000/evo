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

    Model* rectM = ModelGen_Rect(0, 0, 0, 200, 100);
    Model_BufferAttribute(rectM, MODEL_ATTRIBUTE_COLOR, rectColors);
    Rect wRect = {0,0,.4,.4};
    Model_SetMaterial(rectM, mat);
    Widget* w = GUILayout_NewTextBox(rectM, &wRect, "HELLO ALL");
    GUILayout_SetRootWidget(w);
    
    background = ModelGen_Rect(-10,-10,-4,20,20);
    Model_BufferAttribute(background, MODEL_ATTRIBUTE_COLOR, rectColors);
    Model_SetMaterial(background, mat);
    Draw_OptimizeModel(background);

    Scene_Add(background);
    Scene_Add(m);

    Component_Transform e1_transform = {.x = 1.0f, .y = 1.0f, .z = 20.0f};
    Component_Transform e2_transform = {.x = 1.5f, .y = 1.0f, .z = 20.0f};
    Component_Collider testcol = {.aabb={1,1,1}};
    ENTITY(e1, e1_transform,
            Component_New_Collider(testcol)
          )
    ENTITY(e2, e2_transform,
            Component_New_Collider(testcol)
          )

    Collision_Detect();
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

