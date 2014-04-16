#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "renderer.h"

HANDLER(tv_text_renderer, tv_renderer)
	const char *font;
	float font_size;
	TvRect rect;
	TvString *text;
	tv_model *model;
	TvTexture *font_texture;

	void (*set_text)(struct tv_text_renderer *self, const tvchar *text);
ENDHANDLER(tv_text_renderer)

#ifdef __cplusplus
}
#endif
#endif