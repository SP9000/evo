#include "widget_slider.h"

COMPONENT_NEW(tv_widget_slider, tv_widget)
	tv_widget_init((tv_widget*)self);
END_COMPONENT_NEW(tv_widget_slider)

COMPONENT_START(tv_widget_slider)
	tv_widget_set_material((tv_widget*)self, (tv_material*)tv_component_get(self_component, tv_material_id()));

	/* get the slider models */
	self->slide = tv_modelgen_quad(tv_vector2_new(20, 100), TV_MODEL_VERTEX_FORMAT_PC);
	self->slide = tv_modelgen_quad(tv_vector2_new(300, 20), TV_MODEL_VERTEX_FORMAT_PC);
END_COMPONENT_START

COMPONENT_DESTROY(tv_widget_slider)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(tv_widget_slider)
END_COMPONENT_UPDATE

void tv_widget_slider_set_value(tv_widget_slider* slider, tvfloat val)
{
	slider->value = val;
}
