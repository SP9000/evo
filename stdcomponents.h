/******************************************************************************
 * stdcomponents.h
 * This file includes all the standard components for ease of inclusion by 
 * other files.
 * 
 * Bryce Wilson
 * Created June 25, 2013
 *****************************************************************************/
#ifndef STDCOMPONENTS_H
#define STDCOMPONENTS_H
#ifdef __cplusplus
extern "C" {
#endif

enum {
    CID_Transform = 1,
    CID_Collider,
    CID_Debug,
    CID_Model,
    CID_Material,

    CID_End
} STDComponentID;

/* forward declarations for component types */

#include <GL/glew.h>
#include "cJSON.h"
#include "glib.h"

/* include the prototypes for all the components */
#include "Components/transform.c"
#include "Components/collider.c"
#include "Components/material.c"
#include "Components/model.c"
#include "Components/widget.c"

#ifdef __cplusplus
}
#endif
#endif
