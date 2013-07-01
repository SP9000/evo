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

/* I'm well goddamn aware we're redefining DEFINE */
#undef DEFINE
#undef BEGIN
#undef END
#undef SET
#undef CTOR

#ifdef BUILD
#define COMPONENT(X, ATTRS) \
    typedef struct Component_##X { \
        Component base; \
        ATTRS \
    }Component_##X;

#define BEGIN(X, ...) \
    Component* Component_New_##X(Component_##X *init, ##__VA_ARGS__) { \
        Component_##X *self; \
        self = (Component_##X *)malloc(sizeof(Component_##X)); \
        *self = *init; \
        self -> base.start = Start; \
        self -> base.update = Update; \
        self -> base.collide = Collide; \
        self -> base.id = CID_##X ; \

#define CTOR(X, ...) X, ##__VA_ARGS__
#define END return (Component*)self; }

#else
#define COMPONENT(X, ATTRS) \
    typedef struct Component_##X { \
        Component base; \
        ATTRS \
    }Component_##X;
#define BEGIN(X, ...) \
    Component* Component_New_##X(Component_##X *init, __VA_ARGS__);
#define SET(X, Y) 
#define CTOR(X, ...) 
#define END 

#endif
