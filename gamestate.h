#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>
#include "model.h"

typedef struct tagGamestate {
    GHashTable *entities;
}Gamestate;

Gamestate* GamestateNew();
void Gamestate* GamestateFree(Gamestate *gs)

void GamestateAddEntity(Gamestate *gs, Entity *e, char *name);

#ifdef __cplusplus
}
#endif

#endif
