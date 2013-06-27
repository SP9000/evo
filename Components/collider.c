#ifndef COMPONENT_COLLIDER
#define COMPONENT_COLLIDER

#include "../component.h"
#include <stdio.h>


/**
 * The collider component.
 * This component contains information that allows the entity it is attached
 * to be checked for collision with other entities that have colliders.
 */
DEFINE_COMPONENT(Collider,
    ATTRIBUTE(struct Component_Transform* transform)
    ATTRIBUTE(AABB aabb)
)

#ifdef BUILD
void Start(Component_Collider* c)
{
    c->transform = (Component_Transform*)Component_Get((Component*)c, CID_Transform);
    c->aabb.w = 1.0f;
    c->aabb.h = 1.0f;
    c->aabb.d = 1.0f;
}

void Update(Component_Collider* c) 
{

}

void Collide(Entity* e)
{
    puts("collider collision");
}

#endif
#endif

