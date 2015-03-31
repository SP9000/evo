#ifndef INSTANCERENDERER_H
#define INSTANCERENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "renderer.h"

HANDLER(tv_instance_renderer, tv_renderer)
	tv_model *model;
	tvuint num_instances;
ENDHANDLER(tv_instance_renderer)

void tv_instance_renderer_set_model(tv_instance_renderer *self, tv_model *model);

#ifdef __cplusplus
}
#endif
#endif