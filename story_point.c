#include "story_point.h"

COMPONENT_NEW(APP_story_point, tv_widget_textbox)
	tv_widget_textbox_init((tv_widget_textbox*)self);
END_COMPONENT_NEW(APP_story_point)

COMPONENT_START(APP_story_point)
	tv_widget_set_material((tv_widget*)self, (tv_material*)tv_component_get(self_component, tv_material_id()));
END_COMPONENT_START

COMPONENT_UPDATE(APP_story_point)
	tvchar text[1024];
	tv_vector4 c = {1.0f, 0.0f, 0.0f, 1.0f};
	tv_widget_textbox_set_text_color((tv_widget_textbox*)self, c);
	sprintf(text, "AND FINALLY, THE END ARRIVED...");
	tv_widget_textbox_set_text((tv_widget_textbox*)self, text);
END_COMPONENT_UPDATE