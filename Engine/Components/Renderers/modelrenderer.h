#ifndef MODELRENDERER_H
#define MODELRENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "renderer.h"

/******************************************************************************
 * tv_model_renderer
 * TODO: rename to mesh
 * The mesh renderer renders one mesh using the materials of the renderer and 
 * the mesh of this handler.  Each material's index corresponds to the submesh
 * of this handler's mesh. 
 * For example:
 *   mesh -> materials[0]
 *   mesh.submesh[0] -> materials[1]
 *   mesh.submesh[1] -> materials[2]
 *  ....
******************************************************************************/
HANDLER(tv_model_renderer, tv_renderer)
	tv_model *model;
ENDHANDLER(tv_model_renderer)

void tv_model_renderer_set_model(tv_model_renderer *self, tv_model *model);

#ifdef __cplusplus
}
#endif
#endif