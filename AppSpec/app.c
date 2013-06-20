#include "app.h"

extern void App_Start();
extern void App_Update();
extern void App_Quit();

static Model* m;
static Model* background;

void App_Start()
{
    Material* mat = Material_Load("test.mat");
    GUILayout_SetMaterial(mat);

    m = Model_New(0);
    Model_LoadPLY(m, "TestAssets/monkey.ply");
    Model_SetMaterial(m, mat);
    Draw_OptimizeModel(m);

    Color col = {1.0f,0.0f,0.0f,1.0f};
    Model* rectM = GenRect(0, 0, 0, 10, 10, col);
    Rect wRect = {0,0,.2,.4};
    Model_SetMaterial(rectM, mat);
    Widget* w = GUILayout_NewTextBox(rectM, &wRect, "HELLO ALL");
    GUILayout_SetRootWidget(w);

    Color bgCorner1 = {0.0f, 0.0f, 1.0f, 1.0f};
    Color bgCorner2 = {0.0f, 0.0f, 1.0f, 1.0f};
    Color bgCorner3 = {1.0f, 0.0f, 0.0f, 1.0f};
    Color bgCorner4 = {1.0f, 0.0f, 0.0f, 1.0f};
    background = GenRect(-4,-4,-20,8,8, bgCorner1);
    Model_SetAttribute(background, MODEL_ATTRIBUTE_COLOR, 0, bgCorner1);
    Model_SetAttribute(background, MODEL_ATTRIBUTE_COLOR, 1, bgCorner2);
    Model_SetAttribute(background, MODEL_ATTRIBUTE_COLOR, 2, bgCorner3);
    Model_SetAttribute(background, MODEL_ATTRIBUTE_COLOR, 3, bgCorner4);

    Model_SetMaterial(background, mat);
    Draw_OptimizeModel(background);

    Scene_Add(background);
    Scene_Add(m);
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
    Model_Free(background);
}
