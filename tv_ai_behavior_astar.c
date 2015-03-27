#include "ai_behavior_astar.h"

static void tv_ai_seek(tv_ai_behavior *self)
{
	tv_ai_behavior_astar *ai = (tv_ai_behavior_astar*)self;
}

COMPONENT_NEW(tv_ai_behavior_astar, tv_ai_behavior)
	tv_ai_behavior *base = (tv_ai_behavior*)self;
	base->seek  = tv_ai_seek;
END_COMPONENT_NEW(tv_ai_behavior_astar)

COMPONENT_START(tv_ai_behavior_astar)
END_COMPONENT_START

COMPONENT_DESTROY(tv_ai_behavior_astar)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(tv_ai_behavior_astar)
	tv_ai_behavior *base = (tv_ai_behavior*)self;
	tv_ai_seek(base);
END_COMPONENT_UPDATE
