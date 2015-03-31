#ifndef HUD_H
#define HUD_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "Abilities/ability.h"

#define APP_HUD_UNIT_SLOTS 6

COMPONENT(app_hud, tv_widget)
	tv_animation *animation;
	/* the indices of the thumbnail bones */
	tvuint thumbnails[16];
	/* widgets of the player's units */
	tv_widget* unit_widgets[APP_HUD_UNIT_SLOTS];
	/* the units corresponding to the thumbnail widgets */
	app_unit* units[APP_HUD_UNIT_SLOTS];
ENDCOMPONENT(app_hud)

/**
 * Set the thumbnail at the given index to the one given model.
 * @param hud the HUD to set the thumbnail in.
 * @param thumbnail the model to set the thumbnail to.
 * @param index the index of the ability to set.
 */
void app_hud_set_ability_thumbnail(app_hud *hud, tv_model *thumbnail, tvuint index);

/**
 * Adds a model to the next available spot in the HUD.
 * @param hud the HUD to add the model to.
 * @param model the model to append.
void app_hud_set_unit(app_hud* hud, app_unit* unit, tvuint slot)
 */
void app_hud_add_model(app_hud* hud, tv_model* model);
void app_hud_add_unit(app_hud* hud, app_unit* unit);

/**
 * Sets the model for the given unit slot.
 * @param hud the HUD to set the model of.
 * @param model the model to replace the contents of the given slot with.
 * @param the unit slot to replace.
 */
void app_hud_set_model(app_hud* hud, tv_model* model, tvuint slot); 
void app_hud_set_unit(app_hud* hud, app_unit* unit, tvuint slot);

/**
 * Get the dimensions of a single ability thumbnail in the HUD.
 * @return the dimensions of a thumbnail in the HUD.
 */
static tv_rect app_hud_thumbnail_dim() {tv_rect ret = {0.2f / 5.0f, 0.3f / 5.0f, 0.05f, 0.05f}; return ret;}

#ifdef __cplusplus
}
#endif
#endif