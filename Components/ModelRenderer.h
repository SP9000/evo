/*****************************************************************************/
/* ModelRenderer.c                                                           */
/* One of the more basic renderers.  This just renders a single model.       */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 19, 2013                                                    */
/*****************************************************************************/
#ifndef MODELRENDERER_H
#define MODELRENDERER_H
#include "All.h"

typedef struct ModelRenderer;
typedef struct ModelRenderer {
	Renderer base;
	/* the model attribute of the renderer (geometry and per-vertex attributes) */
	TvModel* model;
}ModelRenderer;

void ModelRenderer_Load(ModelRenderer* self, char* file);

#endif