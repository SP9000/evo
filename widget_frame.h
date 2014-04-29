#ifndef WIDGET_TEXTBOX_H
#define WIDGET_TEXTBOX_H
#ifdef __cplusplus
extern "C" {
#endif

#include "widget.h"

COMPONENT(tv_widget_frame, tv_widget)
	tv_vector4 color;
ENDCOMPONENT(tv_widget_frame)

void tv_widget_frame_set_color(tv_widget_frame *frame, tv_vector4 color);

#ifdef __cplusplus
}
#endif
#endif