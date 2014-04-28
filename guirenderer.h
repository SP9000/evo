#ifndef GUIRENDERER_H
#define GUIRENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "renderer.h"

HANDLER(tv_gui_renderer, tv_renderer)
	const char *font;
	float font_size;
	tv_rect rect;
	tv_string *text;
	tv_model *model;
	TvTexture *font_texture;
ENDHANDLER(tv_gui_renderer)

#ifdef __cplusplus
}
#endif
#endif