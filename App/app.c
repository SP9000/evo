#include "app.h"

void App_Start()
{
    Component_Camera* cam_ptr;

    /*
    ENTITY(monkey,
            Component_New_Transform(NULL, 
                (Vector3){0.5f,0.5f,0.0f}, (Vector3){1.0f,1.0f,1.0f}),
            Component_New_Collider(NULL, (AABB){1,1,1}),
            Component_New_Model(NULL, "TestAssets/monkey.ply"),
            Component_New_Material(NULL, "test.mat"),
            Component_New_PlayerMotor(NULL, 1.0f, 1.0f, 0.05f)
          )
    ENTITY(monkey2,
            Component_New_Transform(NULL, 
                (Vector3){1.3f,0.6f,0.0f}, (Vector3){1.0f,1.0f,1.0f}),
            Component_New_Collider(NULL, (AABB){1,1,1}),
            Component_New_Model(NULL, "TestAssets/monkey.ply"),
            Component_New_Material(NULL, "test.mat")
          )
    ENTITY(background,
            Component_New_Transform(NULL, 
                (Vector3){50.0f,50.0f,-10.0f}, (Vector3){100.0f,100.0f,1.0f}),
            Component_New_Model(NULL, "StdAssets/Models/quad.ply"),
            Component_New_Material(NULL, "test.mat")
          )
    ENTITY(cam,
            Component_New_Transform(NULL,
                (Vector3){0.0f,0.0f,-7.0f}, (Vector3){1.0f,1.0f,1.0f}),
            Component_New_Camera(NULL)
          )
         */
    /* get/setup camera */
    /*
    cam_ptr= (Component_Camera*)Entity_GetComponent(cam, CID_Camera);
    cam_ptr->Perspective(cam_ptr, 60.0f, (float)screen->w/(float)screen->h,
            0.01f, 100.0f);
    */
    puts("Application initialized");
    fflush(stdout);
}

void App_Update()
{

}

void App_Quit()
{

}

