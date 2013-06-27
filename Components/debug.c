#ifndef DEBUG_H
#define DEBUG_H

#include "../component.h"

/**
 * The debug component.
 * If this is attached to an object, it makes certain information visible
 * that may be useful for debugging.
 */
DEFINE_COMPONENT(Debug,
    ATTRIBUTE(struct Component_Transform* transform)
    ATTRIBUTE(struct Component_Collider* col)
    ATTRIBUTE(struct Model* colModel)
)

#ifdef BUILD
void Start(Component_Debug* c)
{
    c->transform = (Component_Transform*)Component_Get((Component*)c, CID_Transform);
    c->col = (Component_Collider*)Component_Get((Component*)c, CID_Collider);
    if(c->col) {
        AABB aabb = c->col->aabb;
        c->colModel = ModelGen_Box(c->transform->x, c->transform->y, c->transform->z,
                aabb.w, aabb.h, aabb.d);
    }
}

void Update(Component_Debug * c) 
{

}

void Collide(Entity* e)
{
    puts("debug collision");
}

#endif
#endif

