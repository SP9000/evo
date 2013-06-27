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

/* I'm well goddamn aware we're redefining DEFINE_COMPONENT */
#undef DEFINE_COMPONENT

/** 
 * DEFINE_COMPONENT(X, Y)
 * This macro performs different functions based on the definition of BUILD.
 *
 * If BUILD is not defined, this macro generates:
 *  a typedef for this component of the name <Component_X>
 *  a Component_New_XX() function that allocates and returns the
 *      component XX.
 *
 * If BUILD is defined, this macro also produces the actual initialization
 * code for the component and additionally declares:
 *  a static void Start() function - called to initialize the component.
 *  a static void Update() function - called per frame to update the component.
 *
 *  To define members of a component, use the ATTRIBUTE macro. Make sure to 
 *  prefix other components with "struct"
 *  Here is an example of this very unintuitive process:
 *  DEFINE_COMPONENT(
 *      ATTRIBUTE(struct Component_Transform)
 *      ATTRIBUTE(Vector3 position)
 *      ATTRIBUTE(Vector3 rotation)
 *      ATTRIBUTE(Vector3 scale)
 *  )
 */
#ifdef BUILD
#define DEFINE_COMPONENT(X, attributes) \
    typedef struct Component_##X { \
        Component base; \
        attributes \
    }Component_##X; \
    static void Start(Component_##X *c); \
    static void Update(Component_##X *c); \
    static void Collide(Entity* e); \
    Component* Component_New_##X(Component_##X init) { \
        Component_##X *c; \
        c = (Component_##X *)malloc(sizeof(Component_##X)); \
        *c = init; \
        c->base.start = Start; \
        c->base.update = Update; \
        c->base.collide = Collide; \
        c->base.id = CID_##X; \
        return (Component*)c; \
    }
#else
#define DEFINE_COMPONENT(X, attributes) \
    typedef struct Component_##X { \
        Component base; \
        attributes \
    }Component_##X; \
    Component* Component_New_##X(Component_##X); 
#endif

