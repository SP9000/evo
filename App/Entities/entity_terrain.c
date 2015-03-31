#include "app_entities.h"
#include "app_resources.h"

ENTITY(terrain, 0.0f,0.0f,-10.0f, 1,1,1, 0,0,0)
ADDCOMPONENT(tv_model_renderer, renderer)
ADDCOMPONENT(tv_material, material)
tv_material_load(material, RSC_TEST_MAT);
ENTITY_END
