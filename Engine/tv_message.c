#include "tv_message.h"

void tv_message_connect(TV_message_slot *slot, TV_message_signal *sig, TV_message_func f)
{
	slot->trigger = f;
	utarray_push_back(sig->listeners, &slot);
}

void tv_message_emit(TV_message_signal *signal, TV_message *message)
{
	
}