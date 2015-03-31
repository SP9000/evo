#ifndef WIDGET_SLIDER_H
#define WIDGET_SLIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "widget.h"
#include "tv_modelgen.h"

COMPONENT(tv_widget_slider, tv_widget)
	/* the width of the slider relative to the parent widget's size */
	tvfloat width;
	/* the range that this slider goes to/from */
	tv_vector2 range;
	/* the current value of the slider */
	tvfloat value;

	/* the model for the slide */
	tv_model* slide;
	/* the model for the slider handle */
	tv_model* handle;
ENDCOMPONENT(tv_widget_slider)

void tv_widget_slider_set_value(tv_widget_slider* slider, tvfloat val);

#ifdef __cplusplus
}
#endif
#endif