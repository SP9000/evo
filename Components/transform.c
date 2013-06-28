#ifndef COMPONENT_TRANSFORM
#define COMPONENT_TRANSFORM

#include "../component.h"

/**
 * The transform component - common to all entities.
 * The transform component contains the data to represent an entity's physical
 * location in the scene.
 */
DEFINE(Transform,
    ATTRIBUTES(
        float x;
        float y;
        float z;
    ),
    FUNCTIONS(
        static void Start(Component_Transform* this) 
        {

        }
        static void Update(Component_Transform* c) 
        {

        }
        static void Collide(Entity* e)
        {
            puts("transform collision");
        }
    ),
    INIT()
)

#endif

