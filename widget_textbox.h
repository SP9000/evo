#ifndef WIDGET_TEXTBOX_H
#define WIDGET_TEXTBOX_H
#ifdef __cplusplus
extern "C" {
#endif

#include "widget.h"

COMPONENT(tv_widget_textbox, tv_widget)
	tv_string *text;
	tv_rect rect;
	tv_vector4 color;
ENDCOMPONENT(tv_widget_textbox)

void tv_widget_textbox_set_text(tv_widget_textbox *textbox, tvchar *text);
void tv_widget_textbox_set_text_color(tv_widget_textbox *textbox, tv_vector4 color);
void tv_widget_textbox_set_rect(tv_widget_textbox *textbox, tv_rect *rect);

#ifdef __cplusplus
}
#endif
#endif