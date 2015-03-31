#include "renderer.h"

/******************************************************************************
 * render
 * The abstract render method that the render handlers implement.
 *****************************************************************************/
static void render(tv_component *renderer)
{
	tv_info("abstract render method");
}
/******************************************************************************
 * set_material
 *****************************************************************************/
void tv_renderer_set_materials(tv_renderer* renderer, tv_array* materials)
{
	renderer->materials = materials;
}
/* TODO: delete */
void tv_renderer_set_material(tv_renderer* renderer, tv_material* material)
{
	renderer->material = material;
}
/*******************************************************************************
********************************************************************************
*******************************************************************************/
HANDLER_NEW(tv_renderer, tv_component, render, TV_UPDATE_STAGE_MAIN_RENDER)
	utarray_new(self->materials, &ut_ptr_icd);
END_HANDLER_NEW(tv_renderer)

HANDLER_START(tv_renderer)
END_HANDLER_START

HANDLER_UPDATE(tv_renderer)
END_HANDLER_UPDATE


