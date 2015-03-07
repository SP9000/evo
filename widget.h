#ifndef WIDGET_H
#define WIDGET_H
#ifdef __cplusplus
extern "C" {
#endif

#include "tv_component.h"
#include "material.h"
#include "animation.h"
#include "widgetmodels.h"

COMPONENT(tv_widget, tv_component)
	/* the position of the widget; note that the transform is ONLY used to
	 * position the top level widget - children of the root widget 
	 * have no reliance on an entity of any kind. 
	 */
	tv_vector3 pos;
	/* The scale for the widget (applied to all sub-widgets and animation) */
	tv_vector3 scale;
	/* rotation, same thing */
	tv_vector4 rot;

	tvuint w, h;
	tvuint max_w, max_h;
	tvuint min_w, min_h;
	tvuint base_w, base_h;
	tv_rect children_rect;
	
	/* the geometry to render within this widget */
	tv_animation* animation;
	/* the material to use to render this widget */
	tv_material *material;

	/* the child widgets of this widget */
	tv_array *children;
ENDCOMPONENT(tv_widget)

void tv_widget_add_child(tv_widget *widget, tv_widget *child);
void tv_widget_set_model(tv_widget *widget, tv_model *model);
void tv_widget_set_animation(tv_widget* widget, tv_animation* animation);
void tv_widget_set_material(tv_widget *widget, tv_material *material);

tvuint tv_widget_num_children(tv_widget *widget);
tv_widget **tv_widget_children(tv_widget *widget);

static void tv_widget_set_position(tv_widget *widget, tv_vector3 pos) {widget->pos = pos;}

#ifdef __cplusplus
}
#endif
#endif