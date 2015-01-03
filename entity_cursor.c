#include "app_entities.h"

ENTITY(cursor, 0.5,0.125,-10.5, 1,1,1, 0,0,0)
ADDCOMPONENT(tv_overlay_renderer, renderer)
ADDCOMPONENT(tv_material, material)
ADDCOMPONENT(app_cursor, curr)
tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\cursor.mat");
ENTITY_END
