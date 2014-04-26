#ifndef TV_MESSAGE_H
#define TV_MESSAGE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef enum {
	TV_MESSAGE_NONE,
	TV_MESSAGE_COLLISION	//TODO: delete? collision probably not sent via message
}tv_message_type;

typedef tvpointer tv_message;

#ifdef __cplusplus
}
#endif
#endif