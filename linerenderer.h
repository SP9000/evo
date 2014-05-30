#ifndef LINERENDERER_H
#define LINERENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "renderer.h"

HANDLER(tv_line_renderer, tv_renderer)
	tv_model *model;
	tvuint line_width;
ENDHANDLER(tv_line_renderer)

void tv_line_renderer_apply(tv_line_renderer* self);
void tv_line_renderer_add_line(tv_line_renderer* self, tv_vector3 start, tv_vector3 end, tv_vector3 color);
void tv_line_renderer_set_line_width(tv_line_renderer* self, tvuint line_width);
void tv_line_renderer_clear(tv_line_renderer* self);

#ifdef __cplusplus
}
#endif
#endif