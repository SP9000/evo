#ifndef APPCOMPONENTS_H
#define APPCOMPONENTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* include all application's components */
#include "Components/playermotor.c"

/* define ID's for all components beginning where the standard ones left off */
enum {
    CID_PlayerMotor = CID_End,
    
} AppComponentID;

#ifdef __cplusplus
}
#endif

#endif
