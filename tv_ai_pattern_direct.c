#include "tv_ai_pattern_direct.h"

static void tv_ai_seek(tv_ai_pattern *self)
{
	tv_ai_pattern_direct *ai = (tv_ai_pattern_direct*)self;
	tv_vector3 dpos;
	tv_vector3_direction(((tv_component*)ai)->transform->pos, ((tv_component*)self->target)->transform->pos, &dpos);

	tv_vector3_scale(&dpos, ai->speed);
	tv_vector3_add(((tv_component*)ai)->transform->pos, dpos, &((tv_component*)ai)->transform->pos);
}

COMPONENT_NEW(tv_ai_pattern_direct, tv_ai_pattern)
	tv_ai_pattern *base = (tv_ai_pattern*)self;
	self->speed = 1.0f;
END_COMPONENT_NEW(tv_ai_pattern_direct)

COMPONENT_START(tv_ai_pattern_direct)
END_COMPONENT_START

COMPONENT_DESTROY(tv_ai_pattern_direct)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(tv_ai_pattern_direct)
	tv_ai_pattern *base = (tv_ai_pattern*)self;
	tv_ai_seek(base);
END_COMPONENT_UPDATE
