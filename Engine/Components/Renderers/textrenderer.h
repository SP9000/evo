#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "renderer.h"

HANDLER(tv_text_renderer, tv_renderer)
	const char *font;
	float font_size;
	tv_rect rect;
	tv_string *text;
	tv_model *model;
	TvTexture *font_texture;
ENDHANDLER(tv_text_renderer)

void tv_text_renderer_set_text(tv_text_renderer *self, const tvchar *text);

#ifdef __cplusplus
}
#endif
#endif