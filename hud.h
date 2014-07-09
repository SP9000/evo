#ifndef HUD_H
#define HUD_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"

COMPONENT(app_hud, tv_widget)
	tv_animation *animation;
	/* the indices of the thumbnail bones */
	tvuint thumbnails[16];
ENDCOMPONENT(app_hud)

/**
 * Set the thumbnail at the given index to the one given model.
 * @param hud the HUD to set the thumbnail in.
 * @param thumbnail the model to set the thumbnail to.
 * @param index the index of the ability to set.
 */
void app_hud_set_ability_thumbnail(app_hud *hud, tv_model *thumbnail, tvuint index);

/**
 * Get the dimensions of a single ability thumbnail in the HUD.
 * @return the dimensions of a thumbnail in the HUD.
 */
static tv_rect app_hud_thumbnail_dim() {tv_rect ret = {0.2f / 5.0f, 0.3f / 5.0f, 0.05f, 0.05f}; return ret;}
/**
 * @return the dimensions the HUD region.
 */
static tv_rect app_hud_thumbnail_region() {tv_rect ret = {0.85f, 0.75f, 0.1f, 0.1f}; return ret;}

#ifdef __cplusplus
}
#endif
#endif