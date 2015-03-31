#ifndef GUIRENDERER_H
#define GUIRENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "renderer.h"
#include "tv_gui.h"
#include "animation.h"

HANDLER(tv_gui_renderer, tv_renderer)
	const char *font;
	float font_size;
	tv_rect rect;
	tv_string *text;
	tv_animation *animation;
	TvTexture *font_texture;
	tv_widget *widget;
ENDHANDLER(tv_gui_renderer)

void tv_gui_renderer_set_base_widget(tv_gui_renderer *renderer, tv_widget *w);

#ifdef __cplusplus
}
#endif
#endif