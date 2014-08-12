#ifndef TV_MESSAGE_H
#define TV_MESSAGE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "tv_types.h"
#include "tv_array.h"
#include "tv_queue.h"

/**
 * An enumeration of the types of messages. User messages made be used
 * starting at TV_MESSAGE_USER_TYPES.
 */
typedef enum TV_message_type {
	TV_MESSAGE_NONE,
	TV_MESSAGE_COLLISION,

	TV_MESSAGE_USER_TYPES
}TV_message_type;

/**
 * Messages are packets of data that are used to communicate the details of a
 * signal's activation to the appropriate slot. The function that is activated
 * is responsible for casting the message packet to the correct type.
 */
typedef tvpointer TV_message; 
/* A function used to communicate signals to slots. */
typedef void (*TV_message_func)(TV_message *msg);

typedef struct TV_message_signal {
	/* the message listeners that are registered to this signal. */
	tv_array *listeners;
	/* if TRUE, this signal is emitted at the message-update stage. */
	tvbool emit;
}TV_message_signal;

typedef struct TV_message_slot {
	TV_message_func trigger;
}TV_message_slot;

/**
 * Connects the given signal to the given slot.
 * @param slot the slot that is to be connected to the given signal.
 * @param sig the signal that activates the slot it is connected to.
 * @param f the function that is called when a message of the given type is 
 *	sent to the handler.
 */
void tv_message_connect(TV_message_slot *slot, TV_message_signal *sig, TV_message_func f);

/**
 * Emit the given signal.
 * The specified message is emitted via the given signal.
 * @param handler the handler that is emitting the message.
 * @param message the message to send to all registered receivers.
 */
void tv_message_emit(TV_message_signal, TV_message *message);

#ifdef __cplusplus
}
#endif
#endif
