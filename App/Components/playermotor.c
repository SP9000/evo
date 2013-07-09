/*****************************************************************************/
/* The player motor component - attach this to a component to have it respond*/
/* to user input to move an entity.                                          */
/*                                                                           */
/* Bryce Wilson                                                              */
/* July 8, 2013                                                              */
/*****************************************************************************/
#ifndef COMPONENT_PLAYERMOTOR
#define COMPONENT_PLAYERMOTOR

#include <SDL/SDL.h>
#include "../../input.h"
#include "../../component.h"

COMPONENT(PlayerMotor,
    struct Component_Transform* transform;
    float fallSpeed;
    float jumpHeight;
    float speed;
)

CTOR(PlayerMotor, float fallSpeed, float jumpHeight, float speed)

#ifdef BUILD
    static void Start(Component_PlayerMotor* self) 
    {
        self->transform = Component_GetAs(Transform);
    }
    static void Update(Component_PlayerMotor* self) 
    {   
        if(Input_KeyDown(SDLK_RIGHT)) {
            self->transform->pos.x -= self->speed * time_delta;
        }
        if(Input_KeyDown(SDLK_LEFT)) {
            self->transform->pos.x += self->speed * time_delta;
        }
    }
    static void Collide(Entity* e)
    {

    }
    NEW(PlayerMotor, float fallSpeed, float jumpHeight, float speed)
        self->fallSpeed = fallSpeed;
        self->speed = speed;
        self->jumpHeight = jumpHeight;
    END
#endif
#endif
