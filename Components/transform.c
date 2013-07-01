/*****************************************************************************/
/* The transform component - common to all entities.                         */
/* The transform component contains the data to represent an entity's        */
/* location in the scene.                                                    */
/*****************************************************************************/
#ifndef COMPONENT_TRANSFORM
#define COMPONENT_TRANSFORM

#include "../component.h"

COMPONENT(Transform,
    float x;
    float y;
    float z;
)
#ifdef BUILD
    static void Start(Component_Transform* self) 
    {

    }
    static void Update(Component_Transform* self) 
    {

    }
    static void Collide(Entity* e)
    {
        puts("transform collision");
    }
#endif
BEGIN(Transform, float x, float y, float z)
CTOR(
    self->x = x;
    self->y = y;
    self->z = z;
)
END
#endif
