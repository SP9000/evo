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

#include "client_packet.h"
#include "collision.h"
#include "draw.h"
#include "input.h"
#include "modelgen.h"
#include "timer.h"

extern void App_Start();
extern void App_Update();
extern void App_Quit();

#ifdef __cplusplus
}
#endif
#endif
