#include "Collider.h"

static void Start(Collider* self);

void Start(Collider* self)
{
	self->base.entity->aabb.w = 1.0f;
    self->base.entity->aabb.h = 1.0f;
    self->base.entity->aabb.d = 1.0f;
}

Collider* collider_new()
{
	return (Collider*)tv_alloc(sizeof(Collider));
}
