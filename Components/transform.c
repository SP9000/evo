/*****************************************************************************/
/* The transform component - common to all entities.                         */
/* The transform component contains the data to represent an entity's        */
/* location in the scene.                                                    */
/*****************************************************************************/
#ifndef COMPONENT_TRANSFORM
#define COMPONENT_TRANSFORM

#include "../component.h"

/* Define the component's name and attributes */
COMPONENT(Transform,
    Vector3 pos;
    Vector3 scale;
)

/* Prototype the constructor for the component */
CTOR(Transform, Vector3 pos, Vector3 scale) 

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
    
    NEW(Transform, Vector3 pos, Vector3 scale) 
        self->pos = pos;
        self->scale = scale;
    END
#endif
#endif
