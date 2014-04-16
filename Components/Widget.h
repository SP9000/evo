/*****************************************************************************/
/* Widget.c                                                                  */
/* This file contains definitions for functions used to compose the GUI.     */
/* No code to actually render widgets should be put here. As with            */
/* all other rendered items, that belongs in draw.h/draw.c                   */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: June 5, 2013                                                     */
/*****************************************************************************/
#ifndef APP_WIDGET_H
#define APP_WIDGET_H
#include "..\evo.h"
/* TODO: maybe #ifdef Widget and make a separate version for when mouse input
 * is not needed (fewer or no default callbacks for input-events)
 */

typedef struct Widget {
	TvComponent base;

    /*************************************************************************/
    /*                            Data                                       */
    /*************************************************************************/
    /* the lower left corner and width/height of the widget */
    PUBLIC TvRect rect;
    /* the minimum dimensions of the widget */
    PUBLIC float minW;
    PUBLIC float minH;
    /* maximum dimensions of the widget or 0 for no limit */
    PUBLIC float maxW;
    PUBLIC float maxH;
    /* flags for various attributes of the widget TODO: needed? */
    PUBLIC uint32_t flags;
    /* the children of the widget - a list of widgets */
    struct WidgetList* children;
    /* the parent widget of this widget. NULL if this is the root widget */
    PUBLIC struct Widget* parent;
	/* for lists of widgets, this is this widget's next sibling */
	struct Widget* next;

    /*************************************************************************/
    /*                            Callbacks                                  */
    /*************************************************************************/
    /** 
     * Called when this widget becomes focused (ususally by being clicked).
     */
    PUBLIC void (*OnFocus)();
    /**
     * Called when the mouse is moved over this widget.
     * Even if the widget is not focused, this function is called on it.
     * The coordinates given are from 0-1 as with all widget functions.
     * @param 1 the relative x position of the mouse from the lower-left.
     * @param 2 the relative y position of the mouse from the lower-right.
     */
    PUBLIC void (*OnMoveOver)(float, float);
    /**
     * Called when the left mouse button is pressed on this widget.
     * The coordinates given are from 0-1 as with all widget functions.
     * @param 1 the relative x position of the mouse from the lower-left.
     * @param 2 the relative y position of the mouse from the lower-right.
     */
    PUBLIC void (*OnLClick)(float, float);
    /**
     * Called when the right mouse button is pressed on this widget.
     * The coordinates given are from 0-1 as with all widget functions.
     * @param 1 the relative x position of the mouse from the lower-left.
     * @param 2 the relative y position of the mouse from the lower-right.
     */
    PUBLIC void (*OnRClick)(float, float);
    /**
     * Called when the scroll-wheel is moved while this widget is focused.
     * @param 1 the amount the wheel was scrolled (up is positive).
     */
    PUBLIC void (*OnScroll)(int);
    /**
     * Called when a key is pressed down while this widget is focused.
     * @param 1 the key that was pressed.
     */
    PUBLIC void (*OnKeyDown)(int);
    /**
     * Called when a key is released while this widget is focused.
     * @param 1 the key that was released.
     */
    PUBLIC void (*OnKeyUp)(int);
}Widget;

#endif