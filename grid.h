#ifndef GRID_H
#define GRID_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

COMPONENT(app_grid, tv_component)
	tv_line_renderer *line_renderer;
	tv_material *line_material;
	tvuint width;
	tvuint height;
	tvfloat x_step;
	tvfloat y_step;
	tv_vector3 color;
ENDCOMPONENT(app_grid)

void app_grid_set_line_width(app_grid* grid, tvuint line_width);
void app_grid_set_width(app_grid* grid, tvuint w);
void app_grid_set_height(app_grid* grid, tvuint h);
void app_grid_set_x_step(app_grid* grid, tvuint x_step);
void app_grid_set_y_step(app_grid* grid, tvuint y_step);
void app_grid_set_color(app_grid* grid, tv_vector3 color);

#ifdef __cplusplus
}
#endif
#endif