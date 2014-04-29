#include "widget.h"
#include "camera.h"

COMPONENT_NEW(tv_widget, tv_component)
	utarray_new(self->children, &ut_ptr_icd);
END_COMPONENT_NEW(tv_widget)

COMPONENT_START(tv_widget)
END_COMPONENT_START

COMPONENT_UPDATE(tv_widget)
END_COMPONENT_UPDATE

void tv_widget_add_child(tv_widget *widget, tv_widget *child)
{
	assert(widget != NULL);
	utarray_push_back(widget->children, &child);
}

void tv_widget_set_model(tv_widget *widget, tv_model *model)
{
	assert(widget != NULL);
	widget->model = model;
}
void tv_widget_set_material(tv_widget *widget, tv_material *material)
{
	assert(widget != NULL);
	widget->material = material;
}

tvuint tv_widget_num_children(tv_widget *widget)
{
	if(widget) {
		return utarray_len(widget->children);
	}
	return 0;
}
tv_widget **tv_widget_children(tv_widget *widget)
{
	if(widget) {
		return (tv_widget**)(utarray_front(widget->children));
	}
	return NULL;
}