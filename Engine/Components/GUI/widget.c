#include "widget.h"
#include "camera.h"

COMPONENT_NEW(tv_widget, tv_component)
	utarray_new(self->children, &ut_ptr_icd);
	self->animation = NULL;
	self->material = NULL;
	self->pos = tv_vector3_new(0.0f, 0.0f, 0.0f);
	self->scale = tv_vector3_new(1.0f, 1.0f, 1.0f);
	self->rot = tv_vector4_new(0.0f, 0.0f, 0.0f,1.0f);
END_COMPONENT_NEW(tv_widget)

COMPONENT_START(tv_widget)
	tv_widget_set_material((tv_widget*)self, (tv_material*)tv_component_get(self_component, tv_material_id()));
END_COMPONENT_START

COMPONENT_UPDATE(tv_widget)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(tv_widget)
	if(self->animation) {
		DESTROY(self->animation);
		self->animation = NULL;
	}
	if(self->material) {
		(self->material);
	}
	utarray_free(self->children);
	self->children = NULL;
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
	/* kinda' a hack...actually this whole function is. user should ONLY use animation.
	 * models should never be used alone.  What I call a model should be called a mesh.
	 * animation should be enforced as only way to draw model in future */
	if(widget->animation == NULL) {
		widget->animation = tv_animation_new();
	}
	tv_animation_set_root(widget->animation, model, widget->material);
	//widget->model = model;
}

void tv_widget_set_animation(tv_widget* widget, tv_animation* animation)
{
	widget->animation = animation;
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