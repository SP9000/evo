#include "app_entities.h"
ENTITY(grid, 0,0,0.0f, 1,1,1, 10,0,0)
	tv_vector3 color = {1.0f, 0.0f, 0.0f};
ADDCOMPONENT(app_grid, my_grid) 
ADDCOMPONENT(tv_line_renderer, renderer)
	app_grid_set_width(my_grid, 5);
	app_grid_set_color(my_grid, color);
ENTITY_END