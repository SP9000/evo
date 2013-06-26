/******************************************************************************
 * definecomponent.h
 * This file contains the macros needed to create new types of components.
 * There are no guards on this file for good reason. When building a component
 * the macros must be redefined to get just the prototypes of the other
 * components. DON'T ADD GUARDS!
 * NO I WILL NOT RECONSIDER THIS INCREASINGLY CONVOLUTED HACK OF A SYSTEM! 
 *
 * Bryce Wilson
 * Created June 25, 2013
 *****************************************************************************/
#include <stdlib.h>
#include "types.h"
#include "component.h"

#define ATTRIBUTE(X) X;
#define COMPONENT(X) X
#define COMPONENT_SET(X, Y) X, (Y)Y

/** 
 * A macro that defines basic functions for every component.
 * This macro defines the following:
 *  a Component_Start_XX() function - called to initialize the component.
 *  a Component_Update_XX() function - called per frame to update the component.
 *  a Component_New_XX() function that allocates and returns the
 *      component XX.
 *  Note that because Start() and Update() are static, you should make sure
 *  components are only included in one compilation unit to avoid redundant 
 *  code generation. 
 *  To define members of a comonent, use the ATTRIBUTE macro. 
 *  Here is an example of this very unintuitive process:
 *  DEFINE_COMPONENT(
 *      Transform,
 *      ATTRIBUTE(Vector3 position)
 *      ATTRIBUTE(Vector3 rotation)
 *      ATTRIBUTE(Vector3 scale)
 *  )
 * If _BUILD_COMPONENTS is not defined, this macro will just generate the 
 * prototypes for the types and functions.
 */
#ifdef BUILD
#define DEFINE_COMPONENT(X, attributes) \
    static void Start(); \
    static void Update(); \
    typedef struct tagComponent_##X { \
        Component base; \
        attributes \
    }Component_##X; \
    Component* Component_New_##X() { \
        Component* c; \
        c = (Component*)malloc(sizeof(Component_##X)); \
        c->start = Start; \
        c->update = Update; \
        c->id = CID_##X; \
        return c; \
    }
#else
#define DEFINE_COMPONENT(X, attributes) \
    typedef struct tagComponent_##X { \
        Component base; \
        attributes \
    }Component_##X; \
    Component* Component_New_##X(); 
#endif

