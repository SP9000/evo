#include "tv_message.h"

void tv_message_register(TV_message_listener *listener, TV_message_type msg_type, TV_message_func f)
{
	TV_message msg = {msg_type};
	//utarray_push_back(handler->registered_receivers, &msg);
}

void tv_message_emit(TV_message_listener *handler, TV_message *message)
{
	
}