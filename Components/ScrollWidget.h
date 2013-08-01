/*****************************************************************************/
/* ScrollWidget.c                                                            */
/* This component creates a widget with scrollbars that can be used to pan   */
/* around the content of the widget.                                         */ 
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: July 23, 2013                                                    */
/*****************************************************************************/
#ifndef APP_SCROLLWIDGET
#define APP_SCROLLWIDGET

#include "Widget.h"

typedef struct ScrollWidget {
	Widget base;
    /*************************************************************************/
    /*                            Data                                       */
    /*************************************************************************/
    PUBLIC float scrollbar_width;
    PUBLIC TvTexture scrollbar;
    PUBLIC TvTexture scrollbar_bg;
    TvRect virtual_rect;
}ScrollWidget;

void ScrollWidget_AddWidget(ScrollWidget* self, Widget* w, float x, float y);

#endif