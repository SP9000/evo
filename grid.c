#include "grid.h"

void app_grid_set_line_width(app_grid* grid, tvuint line_width)
{
	tv_line_renderer_set_line_width(grid->line_renderer, line_width);
}

void app_grid_set_color(app_grid* grid, tv_vector3 color) 
{
	grid->color = color;
}
void app_grid_set_width(app_grid* grid, tvuint w)
{
	grid->width = w;
}
void app_grid_set_height(app_grid* grid, tvuint h)
{
	grid->height = h;
}
void app_grid_set_x_step(app_grid* grid, tvuint x_step)
{
	grid->x_step = x_step;
}
void app_grid_set_y_step(app_grid* grid, tvuint y_step)
{
	grid->y_step = y_step;
}

COMPONENT_NEW(app_grid, tv_component)
	self->x_step = 1.0f;
	self->y_step = 1.0f;
	self->width = 10;
	self->height = 10;
END_COMPONENT_NEW(app_grid)

COMPONENT_START(app_grid)
	tvuint i, j;
	tv_vector3 start = {0.0f, 0.0f, 0.0f};
	tv_vector3 end = {self->width * self->x_step, 0.0f, 0.0f};
	
	self->line_renderer = (tv_line_renderer*)tv_component_get(self_component, tv_line_renderer_id());
	self->line_material = tv_material_new();
	tv_material_load(self->line_material, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\line.mat");
	tv_renderer_set_material((tv_renderer*)self->line_renderer, self->line_material);
	tv_line_renderer_set_line_width(self->line_renderer, 4);	
	
	/* generate the horizontal grid lines */
	for(i = 0; i < self->height+1; ++i) {
		tv_line_renderer_add_line(self->line_renderer, start, end, self->color);
		start.y += self->y_step;
		end.y += self->y_step;
	}
	/* generate the vertical grid lines */
	start.y = 0.0f;
	end.x = 0.0f;
	end.y -= self->y_step;
	for(i = 0; i < self->width+1; ++i) {
		tv_line_renderer_add_line(self->line_renderer, start, end, self->color);
		start.x += self->x_step;
		end.x += self->x_step;
	}
	tv_line_renderer_apply(self->line_renderer);
END_COMPONENT_START

COMPONENT_UPDATE(app_grid)
END_COMPONENT_UPDATE