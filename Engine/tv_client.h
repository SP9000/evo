/*****************************************************************************/
/* client.h                                                                  */
/* The client contains the entry point to the engine and the code to interact*/
/* with the server.                                                          */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: Mar/April, 2013                                                  */
/*****************************************************************************/
#ifndef _CLIENT_H
#define _CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <enet/enet.h>

#include "tv_client_packet.h"
#include "tv_collision.h"
#include "tv_draw.h"
#include "tv_input.h"
#include "tv_modelgen.h"
#include "animation.h"
#include "renderer.h"
#include "linerenderer.h"
#include "modelrenderer.h"
#include "animationrenderer.h"
#include "textrenderer.h"
#include "guirenderer.h"
#include "overlayrenderer.h"
#include "tv_time.h"
#include "tv_gui.h"
#include "tv_stats.h"
#include "ai_behavior.h"
#include "tv_ai_behavior_direct.h"

/**
 * Set a function to be called every frame.
 * @param update_func the function to call each frame.
 */
void tv_client_set_update_func(void (*update_func)());

/**
 * Initialize the client.
 */
void tv_client_init();

/**
 * Call this after calling tv_client_init and doing whatever other setup you
 * need to.
 */
void tv_client_start();

#ifdef __cplusplus
}
#endif
#endif
