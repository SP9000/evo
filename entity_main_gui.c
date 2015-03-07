#include "app_entities.h"

ENTITY(gui, 0,0,-1.0f, 1,1,1, 0,0,0)
	tv_rect rect = {0.0f,0.0f,0.1f,0.1f};
	tv_vector4 text_color = {1.0f, 1.0f, 1.0f, 0.8f};
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(tv_gui_renderer, renderer)
ADDCOMPONENT(tv_widget, base_widget)
ADDCOMPONENT(tv_widget_stats, stats)
ADDCOMPONENT(app_hud, hud)
ADDCOMPONENT(APP_story_point, sp)
ADDCOMPONENT(tv_widget_frame, frame)
tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\line.mat");
tv_widget_textbox_set_text_color((tv_widget_textbox*)stats, text_color);
tv_gui_renderer_set_base_widget(renderer, base_widget);
tv_widget_add_child(base_widget, (tv_widget*)hud);
tv_widget_add_child(base_widget, (tv_widget*)stats);
tv_widget_add_child(base_widget, (tv_widget*)sp);
ENTITY_END
