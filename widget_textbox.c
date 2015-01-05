#include "widget_textbox.h"

#include "widget_frame.h"

COMPONENT_NEW(tv_widget_textbox, tv_widget)
	tv_widget_init((tv_widget*)self);
END_COMPONENT_NEW(tv_widget_textbox)

COMPONENT_START(tv_widget_textbox)
	tv_widget_set_material((tv_widget*)self, (tv_material*)tv_component_get(self_component, tv_material_id()));
END_COMPONENT_START

COMPONENT_DESTROY(tv_widget_textbox)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(tv_widget_textbox)
END_COMPONENT_UPDATE

void tv_widget_textbox_set_text(tv_widget_textbox *textbox, tvchar *text)
{
	tv_model* m = ((tv_widget*)textbox)->model;
	if(m) {
		/* delete the old text model (if there is one) */
		CDESTROY(m);
	}
	tv_widget_set_model((tv_widget*)textbox, tv_gui_model_text(text, 80, textbox->color));
}

void tv_widget_textbox_set_text_color(tv_widget_textbox *textbox, tv_vector4 color)
{
	textbox->color = color;
}