#ifndef GRID_H
#define GRID_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

/* the maximum number of bones the root of a model may have */
#define TV_ANIMATION_MAX_BONES 128
/* the maximum # of bones that can be connected to another */
#define TV_ANIMATION_MAX_CHILDREN 10

typedef struct tv_animation_bone {
	tv_vector3 position;
	tv_vector4 rotation;
	tvuint num_children;
	tvuint children[TV_ANIMATION_MAX_CHILDREN];
}tv_animation_bone;

typedef struct tv_animation_keyframe {
	tvuint start_time;
	tvuint stop_time;

	tvuint bone;
	tv_vector3 offset_start;
	tv_vector3 offset_end;
	tv_vector4 rotation_start;
	tv_vector4 rotation_stop;
}tv_animation_keyframe;

typedef enum {
	TV_ANIMATION_NOT_PLAYING,
	TV_ANIMATION_PLAYING,
	TV_ANIMATION_LOOPING
}tv_animation_play_type;

/**
 * The animation component.
 */
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