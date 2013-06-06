/******************************************************************************
 * The Draw system contains all functions used for rendering within the 
 * TV Dinner Engine contains all utilities related to rendering to the display.
 * In theory, this code contains the most platform dependent code, though it is
 * standard OpenGL according to the 4.0 standard. The following subsystems are
 * included in the Draw system:
 *  Material - manages the material (shaders) loaded into the engine.
 *  GUI      - manages 2D drawing ontop of the 3D rendered game area.
 * 
 * Bryce Wilson
 * Created: April, 2013
 * Last Modified: May 4, 2013
 *****************************************************************************/
#ifndef _DRAW_H
#define _DRAW_H

#include <SDL/SDL.h>
#include <GL/glew.h>
#include "glib.h"
#include "model.h"
#include "matrix.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef float Vector2[2];
typedef float Vector3[3];

/*
typedef struct tagModelComponent {
    ModelComponent *attachedTo;
    ModelVertex vertices[];
}ModelComponent;


typedef struct tagModel {
    int numComponents;
    ModelComponent components[];
}Model;
*/

typedef struct Material {
    GLuint frag;
    GLuint vert;
    GLuint program;
}Material;

typedef struct tagRect {
    float x,y;
    float w,h;
}Rect;

typedef struct tagMap {
    guint32 width, height;
}Map;

typedef struct tagCamera {
    Vector3 pos;
    Vector2 rot;
    float fov;
    float aspect;
    float near;
    float far;
}Camera;


/**
 * Initialize the libraries needed for rendering. 
 * @return the success of the initialization - 0 on success, !0 on error.
 */
int DrawInit();

/**
 * Clean up all libraries and allocations of the rendering system.
 */
void DrawQuit();

/**
  * Clear the screen and do anything else necessary before beginning 
  * to render a frame.
  */
void DrawStartFrame();

/**
 * Draw GUI ontop of the currently rendered scene.
 */
void DrawGUI();

/**
 * Draw the given model.
 * Draws the model m, buffering its data to the GPU if this is the first time
 * that the model has been rendered.
 * @param m the model to draw.
 */
void DrawModel(Model *m);

/**
 * Move the camera by the given X,Y,Z amounts.
 * @param x the amount to move the camera in the X direction.
 * @param y the amount to move the camera in the Y direction.
 * @param z the amount to move the camera in the Z direction.
 */
void DrawMoveCamera(float x, float y, float z);

/*****************************GUI SUBSYSTEM***********************************/
/**
 * Draw the specified text in the specified bounds.
 * If the given text cannot fit, as much of it will be displayed as there is
 * room for.
 * @param box the area to draw the text inside.
 * @param text the text to display.
 */
Model* GUIDrawText(Rect *box, char *text, Model* m);


Material* GetMaterial(int id);
void AddMaterial(int id, Material *mat);

#ifdef __cplusplus
}
#endif

#endif
