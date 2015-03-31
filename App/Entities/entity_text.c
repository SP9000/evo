#include "app_entities.h"
#include "app_resources.h"

ENTITY(text, 0,0,0, 1,1,1, 0,0,0)
	tv_rect rect = {0.0f,0.0f,0.1f,0.1f};
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(tv_gui_renderer, text_r)
tv_material_load(material, RSC_TEX_MAT);
text_r->rect = rect;
text_r->font = "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\TestAssets\\font.bmp";
tv_text_renderer_set_text((tv_text_renderer*)text_r, "HELLO WORLD!");
ENTITY_END
