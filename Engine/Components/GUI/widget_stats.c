#include "widget_stats.h"

COMPONENT_NEW(tv_widget_stats, tv_widget_textbox)
	tv_widget_textbox_init((tv_widget_textbox*)self);
END_COMPONENT_NEW(tv_widget_stats)

COMPONENT_START(tv_widget_stats)
	tv_widget_set_material((tv_widget*)self, (tv_material*)tv_component_get(self_component, tv_material_id()));
END_COMPONENT_START

COMPONENT_DESTROY(tv_widget_stats)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(tv_widget_stats)
	tvchar text[1024];
	sprintf(text, "FPS=%f\nPRIMITIVES DRAWN=%d", tv_stats_fps(), tv_stats_primitives_rendered());
	tv_widget_textbox_set_text((tv_widget_textbox*)self, text);
END_COMPONENT_UPDATE