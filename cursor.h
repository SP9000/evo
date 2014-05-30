#ifndef CURSOR_H
#define CURSOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

COMPONENT(app_cursor, tv_component)
	tv_vector4 color;
	tv_overlay_renderer *renderer;
	tv_model *model;
	tv_rect rect;
	tv_vector2 start;

	tv_input_button select_button;
ENDCOMPONENT(app_cursor)

#ifdef __cplusplus
}
#endif
#endif