#ifndef STORY_POINT_H
#define STORY_POINT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

COMPONENT(APP_story_point, tv_widget_textbox)
	tvchar *text;
ENDCOMPONENT(APP_story_point)

void APP_story_point_set_text(APP_story_point *sp, tvchar *text);

#ifdef __cplusplus
}
#endif
#endif