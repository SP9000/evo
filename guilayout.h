/**
 * This file contains definitions for functions used to compose the GUI.
 * This includes high level objects such as widgets, text boxes, etc.
 * No code to actually render said objects should be put here. As with
 * all other rendered items, that belongs in draw.h/draw.c 
 * Bryce Wilson
 * 6/5/2013
 */
#ifndef _GUILAYOUT_H
#define _GUILAYOUT_H

#ifndef __cplusplus
extern "C" {
#endif

/**
 * The widget structure.
 * Widgets are physically similar to models, but carry a little extra
 * information to assist in prettily laying out the GUI. When setting the
 * coordinates of the model within the widget, use a scale from 0 to 1 to allow
 * for flexible sizing on monitors of all resolutions.
 */
typedef struct tagWidget {
    /* the minimum dimensions of the widget */
    float minW, minH;
    /* maximum dimensions of the widget or 0 for no limit */
    float maxW, maxH;
    /* the contents of the widget (what's rendered) */
    Model* contents;
}Widget;

/**
 * Initialize the GUILayout system.
 */
void GUILayoutInit();
void GUILayoutAddWidget();



#ifndef __cplusplus
}
#endif
#endif
