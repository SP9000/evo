#ifndef CURSOR_H
#define CURSOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "ability.h"
#include "hud.h"

COMPONENT(app_cursor, tv_component)
	tv_overlay_renderer *renderer;

	/* the HUD that this cursor's selections should affect. */
	app_hud *hud;
	/* the current model of the selection box */
	tv_model *model;
	/* the color of the selection box */
	tv_vector4 color;
	/* the current selection box */
	tv_rect rect;
	/* the current origin of the selection box */
	tv_vector2 start;

	/* the button that is used to create a selection box */
	tv_input_button select_button;
	/* the button used to move to the mouse position */
	tv_input_button move_button;

	/* the currently selected units */
	tv_array* selected_units;
	tv_array* selected_units_abilities;
ENDCOMPONENT(app_cursor)

/**
 * Add the given unit to the cursor's selection.
 * @param self the cursor to add the unit to the selection of.
 * @param unit the unit to add.
 */
void app_cursor_select_unit(app_cursor *self, app_unit *unit);

/** 
 * Deselect all the curor's selected units.
 * @param self the cursor to deselect all units from.
 */
void app_cursor_deselect_all(app_cursor *self);

#ifdef __cplusplus
}
#endif
#endif