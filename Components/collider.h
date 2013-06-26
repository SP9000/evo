#ifndef COLLIDER_H
#define COLLIDER_H
#include "../component.h"

/**
 * The collider component.
 * This component contains information that allows the entity it is attached
 * to be checked for collision with other entities that have colliders.
 */
DEFINE_COMPONENT(Collider,
    ATTRIBUTE(Component_Transform* transform)
    ATTRIBUTE(AABB aabb)
)

#ifdef BUILD
void Start(Component_Collider* c)
{
    c->transform->x = 0.0f;
    c->transform->y = 0.0f;
    c->transform->z = 0.0f;

    c->aabb.w = 1.0f;
    c->aabb.h = 1.0f;
    c->aabb.d = 1.0f;
}

void Update(Component_Collider* c) 
{

}

#endif
#endif

