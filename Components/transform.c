#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../component.h"

/**
 * The transform component - common to all entities.
 * The transform component contains the data to represent an entity's physical
 * location in the scene.
 */
DEFINE_COMPONENT(Transform,
    ATTRIBUTE(float x)
    ATTRIBUTE(float y)
    ATTRIBUTE(float z)
)

#ifdef BUILD
void Start(Component_Transform* c)
{

}

void Update(Component_Transform* c) 
{

}

void Collide(Entity* e)
{
    puts("transform collision");
}

#endif
#endif

