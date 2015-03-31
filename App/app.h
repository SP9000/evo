#ifndef _APP_H
#define _APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "evo.h"
#include "tv_client.h"
#include "entities.h"
#include "tags.h"

/*****************************************************************************/
/* components */
#include "Misc/playermotor.h"

#include "Misc/grid.h"
#include "Misc/camera_controller.h"
#include "GUI/cursor.h"
#include "GUI/hud.h"

#include "Abilities/ability.h"
#include "Abilities/app_ability_move.h"
#include "Abilities/abilities.h"

#include "Misc/projectile.h"
#include "Misc/laser.h"
#include "Misc/gun.h"
#include "Misc/mass.h"
#include "Misc/velocity.h"
#include "Misc/spawner.h"
#include "Misc/sinespawner.h"
#include "Misc/story_point.h"
#include "Misc/app_sine_move.h"

#include "Terrain/app_terrain.h"


/*****************************************************************************/
void app_register();

#ifdef __cplusplus
}
#endif
#endif
