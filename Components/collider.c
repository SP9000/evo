/*****************************************************************************/
/* The collider component.                                                   */
/* This component contains information that allows the entity it is attached */
/* to be checked for collision with other entities that have colliders.      */
/*****************************************************************************/
#ifndef COMPONENT_COLLIDER
#define COMPONENT_COLLIDER

#include "../component.h"

COMPONENT(Collider,
    struct Component_Transform* transform;
    AABB aabb;
)


#ifdef BUILD
    static void Start(Component_Collider* self)
    {
        self->transform = Component_GetAs(Transform);
        self->aabb.w = 1.0f;
        self->aabb.h = 1.0f;
        self->aabb.d = 1.0f;
    }

    static void Update(Component_Collider* c) 
    {

    }

    static void Collide(Entity* e)
    {
        puts("collider collision");
    }
#endif
#endif

