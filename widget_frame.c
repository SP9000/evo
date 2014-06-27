#include "widget_frame.h"

COMPONENT_NEW(tv_widget_frame, tv_widget)
	tv_widget_init((tv_widget*)self);
END_COMPONENT_NEW(tv_widget_frame)

COMPONENT_START(tv_widget_frame)
	self->color.w = 1;
	self->color.y = 0;
	self->color.y = 0;
	self->color.z = 1;
	((tv_widget*)self)->model = tv_gui_model_border(0.3f, 0.1f, self->color);
	tv_widget_set_material((tv_widget*)self, (tv_material*)tv_component_get(self_component, tv_material_id()));
END_COMPONENT_START

COMPONENT_UPDATE(tv_widget_frame)
END_COMPONENT_UPDATE
