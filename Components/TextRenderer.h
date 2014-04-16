/*****************************************************************************/
/* TextRenderer.h                                                            */
/* This component is designed for rendering text, but really can be used to  */
/* render any sequence of models.                                            */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 19, 2013                                                    */
/*****************************************************************************/

/*  TODO: could make this actually do text..make another component for more  */
/* general stuff.                                                            */

#include "All.h"
typedef struct TextRenderer;
typedef struct TextRenderer {
	Renderer base;
    TvRect* rect;
    
	PUBLIC char* font;
    PUBLIC float font_size;
    PUBLIC char* init_text;

    TvString* text;
    TvModel* model;
    TvTexture* font_texture;
}TextRenderer;
void TextRenederer_SetText(TextRenderer* self, char* text);


