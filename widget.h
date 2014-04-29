#ifndef WIDGET_H
#define WIDGET_H
#ifdef __cplusplus
extern "C" {
#endif

#include "component.h"
#include "material.h"
#include "model.h"
#include "widgetmodels.h"

COMPONENT(tv_widget, tv_component)
	tvuint w, h;
	tvuint max_w, max_h;
	tvuint min_w, min_h;
	tvuint base_w, base_h;
	tv_rect children_rect;
	
	/* the geometry to render within this widget */
	tv_model *model;
	/* the material to use to render this widget */
	tv_material *material;

	/* the child widgets of this widget */
	tv_array *children;
ENDCOMPONENT(tv_widget)

void tv_widget_add_child(tv_widget *widget, tv_widget *child);
void tv_widget_set_model(tv_widget *widget, tv_model *model);
void tv_widget_set_material(tv_widget *widget, tv_material *material);

tvuint tv_widget_num_children(tv_widget *widget);
tv_widget **tv_widget_children(tv_widget *widget);

#ifdef __cplusplus
}
#endif
#endif