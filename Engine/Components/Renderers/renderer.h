/*****************************************************************************/
/* Renderer.h                                                                */
/* Defines the basic renderer of which other renderers are derived.			 */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: April 7, 2014                                                    */
/*****************************************************************************/
#ifndef RENDERER_H
#define RENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tv_component.h"
#include "tv_entity.h"
#include "model.h"
#include "material.h"
#include "tv_texture.h"
#include "tv_draw.h"
#include "tv_vector.h"

HANDLER(tv_renderer, tv_component)
	tv_material *material;	/**< a reference to the first material used by this renderer  TODO: delete */
	tv_array* materials;	/**< the materials used by this renderer to render */
	void (*render_func)(struct tv_component *self);	/**< called to render the entity this renderer belongs to. */
ENDHANDLER(tv_renderer);

void tv_renderer_set_material(tv_renderer* renderer, tv_material* material);
void tv_renderer_set_materials(tv_renderer* renderer, tv_array* materials);

#ifdef __cplusplus
}
#endif
#endif