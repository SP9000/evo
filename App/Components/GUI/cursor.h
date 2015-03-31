#ifndef CURSOR_H
#define CURSOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "tags.h"

#include "Abilities/ability.h"
#include "GUI/hud.h"

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
 * @param unit the unit to add.
 */
void METHOD(app_cursor, select_unit,
	app_unit *unit);

/** 
 * Deselect all the curor's selected units.
 */
void METHOD(app_cursor, deselect_all);
			
/**
 * Set the HUD associated with the cursor.
 * @param hud the HUD to set.
 */
void METHOD(app_cursor, set_hud,
			 app_hud* hud); 

#ifdef __cplusplus
}
#endif
#endif