#include "app_entities.h"

ENTITY(terrain, 0.0f,0.0f,-10.0f, 1,1,1, 0,0,0)
ADDCOMPONENT(app_terrain, terr)
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_material, material)
tv_material_load(material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\test.mat");
ENTITY_END
