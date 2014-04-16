/*****************************************************************************/
/* Renderer.h                                                                */
/* This is the base component for all things that are to be rendered in the  */
/* engine. Included are some basic functions to add attributes.              */
/*                                                                           */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 19, 2013                                                    */
/*****************************************************************************/
#ifndef RENDERER_H
#define RENDERER_H

#include "All.h"

typedef struct Renderer;
typedef struct Renderer {
	TvComponent base;
	 /**
     * What layer this renderer should be displayed on. e.g. RENDER_LAYER_GUI
	 * or RENDER_LAYER_MAIN. 
     */
    PUBLIC int layer;

	/* the material property of the renderer (shaders and uniforms) */
	TvMaterial* material;

    /**
     * All functions which inherit Render should override this. 
     * This is where you put drawing code.
     */
	TvSceneRenderFunc Render;
}Renderer;

#endif