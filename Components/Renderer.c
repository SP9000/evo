#include "Renderer.h"
static void Start(Renderer* self);
static void Update(Renderer* self);

void Start(Renderer* self)
{
	tv_scene_register_renderer(self->Render, self->layer, self->base.id);
	if(self->material->name != NULL) {
		tv_material_load(self->material->name);
	}
}
