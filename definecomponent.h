/*****************************************************************************/
/* definecomponent.h                                                         */
/* This file contains the macros needed to create new types of components.   */
/* There are no guards on this file for good reason. When building a         */
/* component the macros must be redefined to get just the prototypes of the  */
/* other components. DON'T ADD GUARDS!                                       */
/* NO I WILL NOT RECONSIDER THIS INCREASINGLY CONVOLUTED HACK OF A SYSTEM!   */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created June 25, 2013                                                     */
/*****************************************************************************/
#include <stdlib.h>
#include "types.h"
#include "component.h"

#include "p99/p99_args.h"
#include "p99/p99_for.h"

/**
 * these variables are used when parsing the component file to determine what
 * to expose to the JSON initializer. Default is PRIVATE
 */
#define PRIVATE 
#define PUBLIC 

/* I'm well goddamn aware we're redefining COMPONENT */
#undef COMPONENT

#ifdef BUILD
#define COMPONENT(X, ATTRS) \
    typedef struct Component_##X { \
        Component base; \
        ATTRS \
    }Component_##X; \
    static void Update(Component_##X *self); \
    static void Start(Component_##X *self); \
    static void Collide(Entity* other); \
    Component* Component_New_##X(Component_##X *init) { \
        Component_##X *self; \
        self = (Component_##X *)malloc(sizeof(Component_##X)); \
        if(init != NULL) { \
            *self = *init; \
        } \
        self -> base.start = Start; \
        self -> base.update = Update; \
        self -> base.collide = Collide; \
        self -> base.id = CID_##X ; \
        return (Component*)self; }

#else
#define COMPONENT(X, ATTRS) \
    typedef struct Component_##X { \
        Component base; \
        ATTRS \
    }Component_##X;
#endif
