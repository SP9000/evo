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

    CID_End
} STDComponentID;

/* forward declarations for component types */
typedef struct tagComponent_Collider Component_Collider;
typedef struct tagComponent_Transform Component_Transform;

/* don't build these components, only insert their definitions */
#ifdef BUILD
#define BUILDING
#undef BUILD
#endif

/* define the macro for NOT building, and include all standard components */
#include "definecomponent.h"
/* include the prototypes for all the components */
#include "Components/transform.h"
#include "Components/collider.h"

/* if we were building a component, continue */
#ifdef BUILDING
#undef BUILDING
#define BUILD
/* and redefine the macro so that building will work */
#include "definecomponent.h"
#endif


#ifdef __cplusplus
}
#endif
#endif
