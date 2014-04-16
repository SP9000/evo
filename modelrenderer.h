#ifndef MODELRENDERER_H
#define MODELRENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "renderer.h"

HANDLER(tv_model_renderer, tv_renderer)
	tv_model *model;
ENDHANDLER(tv_model_renderer)

void tv_model_renderer_set_model(tv_model_renderer *self, tv_model *model);


#ifdef __cplusplus
}
#endif
#endif