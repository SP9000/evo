/******************************************************************************
 * client.h
 * The client contains the entry point to the engine and the code to interact
 * with the server.
 *
 * Bryce Wilson
 * created: Mar/April, 2013
 *****************************************************************************/
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
#include "draw.h"
#include "material.h"
#include "modelgen.h"
#include "AppSpec/app.h"

#ifdef __cplusplus
}
#endif
#endif
