#include "ai_behavior.h"

/*******************************************************************************
********************************************************************************
*******************************************************************************/
COMPONENT_NEW(tv_ai_navgraph, tv_component)
END_COMPONENT_NEW(tv_ai_navgraph)

COMPONENT_DESTROY(tv_ai_navgraph)
END_COMPONENT_DESTROY

COMPONENT_START(tv_ai_navgraph)
END_COMPONENT_START

COMPONENT_UPDATE(tv_ai_navgraph)
END_COMPONENT_UPDATE
/*******************************************************************************
********************************************************************************
*******************************************************************************/
COMPONENT_NEW(tv_ai_behavior, tv_component)
	self->seeking = FALSE;
	self->seek = NULL;
	self->num_seek_tags = 0;
END_COMPONENT_NEW(tv_ai_behavior)

COMPONENT_DESTROY(tv_ai_behavior)
END_COMPONENT_DESTROY

COMPONENT_START(tv_ai_behavior)
END_COMPONENT_START

COMPONENT_UPDATE(tv_ai_behavior)
	/* if the behavior is currently seeking and a seek function has been provided, 
	 * have the AI behavior seek its target */
	if(self->seeking && self->seek) {
		self->seek(self);
	}
END_COMPONENT_UPDATE
/******************************************************************************/
void tv_ai_behavior_set_target(tv_ai_behavior* self, tv_ai_behavior_target target)
{
	self->target = target;
}
void tv_ai_behavior_seek(tv_ai_behavior* self)
{
	self->seeking = TRUE;
}
void tv_ai_behavior_stop(tv_ai_behavior* self)
{
	self->seeking = FALSE;
}
void tv_ai_behavior_set_seek_func(tv_ai_behavior* self, void (*func)(tv_ai_behavior*))
{
	self->seek = func;
}
/*******************************************************************************
********************************************************************************
*******************************************************************************/
/******************************************************************************/
/* TODO: implement */
static void ai_seek_a_star(tv_ai_behavior* self)
{

}

COMPONENT_NEW(tv_ai_behavior_a_star, tv_ai_behavior)
	tv_ai_behavior_set_seek_func((tv_ai_behavior*)self, ai_seek_a_star);
END_COMPONENT_NEW(tv_ai_behavior_a_star)

COMPONENT_START(tv_ai_behavior_a_star)
END_COMPONENT_START

COMPONENT_UPDATE(tv_ai_behavior_a_star)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(tv_ai_behavior_a_star)
END_COMPONENT_DESTROY
