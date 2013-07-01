/*****************************************************************************/
/* The camera component - contains information to represent the window(s) to */
/* render within.                                                            */
/*                                                                           */
/* Bryce Wilson                                                              */
/* July 1, 2013                                                              */
/*****************************************************************************/
#ifndef COMPONENT_CAMERA
#define COMPONENT_CAMERA

#include "../component.h"

COMPONENT(Camera,
    struct Component_Transform* transform;
    float fov;
    float aspect;
    float nearZ;
    float farZ;

    Mat4x4 modelMat;
    Mat4x4 viewMat;
    Mat4x4 projectionMat;

    void (*Perspective)(struct Component_Camera* self,
        float fov, float aspect, float nearZ, float farZ);
    void (*Orthographic)(struct Component_Camera* self,
        float left, float right, float top, float bottom, float nearZ, float farZ);
)
#ifdef BUILD
    static void Start(Component_Camera* self) 
    {
        self->transform = Component_GetAs(Transform);
        Mat4x4LoadIdentity(self->modelMat);
        Mat4x4LoadIdentity(self->viewMat);
        Mat4x4LoadIdentity(self->projectionMat);
    }
    static void Update(Component_Camera* self) 
    {

    }
    static void Collide(Entity* e)
    {
        puts("camera collision");
    }
    void Perspective(Component_Camera* self, float fov, float aspect, 
            float nearZ, float farZ)
    {
        Mat4x4LoadIdentity(self->modelMat);
        Mat4x4LoadIdentity(self->viewMat);
        Mat4x4LoadIdentity(self->projectionMat);
        Mat4x4PerspMat(self->projectionMat, fov, aspect, nearZ, farZ);
    }
    static void Orthographic(Component_Camera* self,
        float left, float right, float top, float bottom, float nearZ, float farZ)
    {
        Mat4x4LoadIdentity(self->modelMat);
        Mat4x4LoadIdentity(self->viewMat);
        Mat4x4LoadIdentity(self->projectionMat);
        Mat4x4OrthoMat(self->projectionMat, 
                left, right, top, bottom, nearZ, farZ);
    }
#endif
BEGIN(Camera)
CTOR(
        self->Orthographic = Orthographic;
        self->Perspective = Perspective;
    )
END
#endif
