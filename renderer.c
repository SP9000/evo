#include "renderer.h"

HANDLER_NEW(tv_renderer, tv_component, render, 1)
END_HANDLER_NEW(tv_renderer)

HANDLER_START(tv_renderer)
END_HANDLER_START

HANDLER_UPDATE(tv_renderer)
END_HANDLER_UPDATE

void render()
{
}

void tv_renderer_set_material(tv_renderer* renderer, tv_material* material)
{
	renderer->material = material;
}