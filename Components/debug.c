#ifndef COMPONENT_DEBUG
#define COMPONENT_DEBUG

#include "../component.h"

/**
 * The debug component.
 * If this is attached to an object, it makes certain information visible
 * that may be useful for debugging.
 */
DEFINE(Debug,
    ATTRIBUTES(
        struct Component_Transform* transform;
        struct Component_Collider* col;
        struct Model* colModel;
    ),
    FUNCTIONS( 
        static void Start(Component_Debug* c)
        {
            c->transform = (Component_Transform*)Component_Get((Component*)c, CID_Transform);
            c->col = (Component_Collider*)Component_Get((Component*)c, CID_Collider);
            if(c->col) {
                AABB aabb = c->col->aabb;
                c->colModel = ModelGen_Box(c->transform->x, c->transform->y, c->transform->z,
                        aabb.w, aabb.h, aabb.d);
            }
        }

        static void Update(Component_Debug * c) 
        {

        }

        static void Collide(Entity* e)
        {
            puts("debug collision");
        }
    ),
    INIT()
)

#endif

