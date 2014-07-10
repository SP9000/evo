#ifndef OVERLAYRENDERER_H
#define OVERLAYRENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "renderer.h"
#include "tv_gui.h"

HANDLER(tv_overlay_renderer, tv_renderer)
	tv_model *model;
ENDHANDLER(tv_overlay_renderer)

void tv_overlay_renderer_set_model(tv_overlay_renderer *self, tv_model *model);

#ifdef __cplusplus
}
#endif
#endif