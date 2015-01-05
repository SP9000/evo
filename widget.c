#include "widget.h"
#include "camera.h"

COMPONENT_NEW(tv_widget, tv_component)
	utarray_new(self->children, &ut_ptr_icd);
	self->model = NULL;
	self->material = NULL;
	self->pos.x = self->pos.y = self->pos.z = 0.0f;
END_COMPONENT_NEW(tv_widget)

COMPONENT_START(tv_widget)
	tv_widget_set_material((tv_widget*)self, (tv_material*)tv_component_get(self_component, tv_material_id()));
END_COMPONENT_START

COMPONENT_UPDATE(tv_widget)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(tv_widget)
	if(self->model) {
		CDESTROY(self->model);
	}
	if(self->material) {
		(self->material);
	}
	utarray_free(self->children);
END_COMPONENT_DESTROY

void tv_widget_add_child(tv_widget *widget, tv_widget *child)
{
	assert(widget != NULL);
	utarray_push_back(widget->children, &child);
	if(widget->material != NULL) {
		child->material = widget->material;
	}
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