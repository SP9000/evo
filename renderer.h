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

#include "component.h"
#include "entity.h"
#include "model.h"
#include "material.h"
#include "texture.h"
#include "draw.h"
#include "tv_vector.h"

HANDLER(tv_renderer, tv_component)
	/* the material (shaders) used to render this */
	tv_material *material;
	/* called to render the entity this renderer belongs to. */
	void (*render_func)(struct tv_component *self);
ENDHANDLER(tv_renderer);

void tv_renderer_set_material(tv_renderer* renderer, tv_material* material);

#ifdef __cplusplus
}
#endif
#endif