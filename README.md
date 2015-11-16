# evo
Low-dependency ECS-based game engine.

evo is an ultra low dependency game engine written very nearly from the ground up.  The purpose of its creation was (is?) largely academic, but it is a capable engine in its own right.

Driving philosophies behind the creation of this engine are portability, speed (especially in the worst-case scenario), and general-purposeness.

As an entity-component based engine, all objects that exist in the game are so-called "entities" composed of zero to many "components".

Components are defined with a series of macros as well.

COMPONENT_NEW(copmponent, parent)
This macro generates the code to create and initialize a new component.  Inheritance is supported, and the parent's initialization is done prior to the execution of the code defined beyond this macro. 
Note that the inheritance chain may be many levels deep (though that is usually not a good idea).

COMPONENT_START(copmponent)
This macro defines a start funtion for the component, which is executed once when the component is "start"ed.  The purpose of the start function is to get references to other components of interest within this components entity.  By the time this function is executed, all components in the entity are guaranteed to have been allocated and initialized.

The function to retrieve references to these sibling components is tv_entity_get_component.
```
tv_component* tv_entity_get_component(tv_entity* e, tvuint id);
```
id is a unique identifier used for various details of the engine's lightweight RTTI.  Since there can be only one component of each type attached to an entity, there will be only one component assigned this id attached to the entity.

COMPONENT_UPDATE(copmponent)
This macro defines an update function.
Updates happen once per frame.  This is where components work their magic.

COMPONENT_DESTROY(copmponent)
This macro destroys the component.  In general, you do not need to write any code here, but if you allocated any memory, this is where you'll need to free it.


TODO: document the vast remainder of the engine.


