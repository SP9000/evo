#include "tv_ai_behavior_direct.h"
#include "tv_entity.h"

static void tv_ai_seek(tv_ai_behavior *self)
{
}

COMPONENT_NEW(tv_ai_behavior_direct, tv_ai_behavior)
	tv_ai_behavior *base = (tv_ai_behavior*)self;
	self->speed = 1.0f;
	base->target.entity = NULL;
	base->seek  = tv_ai_seek;
	self->aggro_range = 8.0f;
END_COMPONENT_NEW(tv_ai_behavior_direct)

COMPONENT_START(tv_ai_behavior_direct)
END_COMPONENT_START

COMPONENT_DESTROY(tv_ai_behavior_direct)
END_COMPONENT_DESTROY

COMPONENT_UPDATE(tv_ai_behavior_direct)
	tv_vector3 from, to, dir;
	tv_ai_behavior *base = (tv_ai_behavior*)self;
	tv_ai_seek(base);
	
	from = ((tv_component*)self)->entity->transform.pos;
	switch(self->target_type) {
	case TV_AI_BEHAVIOR_TARGET_POS:
		to = base->target.pos;
		break;
	case TV_AI_BEHAVIOR_TARGET_NODE:
		to = *(base->target.node);
		break;
	case TV_AI_BEHAVIOR_TARGET_ENTITY:
		to = base->target.entity->transform.pos;
		break;
	}
	/* don't even look at the navgraph, just push on toward the destination */
	/* TODO: don't move through solid objects (requires some collider work) */
	tv_vector3_direction(from, to, &dir);
	tv_vector3_scale(&dir, self->speed * tv_time_delta);
	tv_vector3_add(((tv_component*)self)->entity->transform.pos, dir, &((tv_component*)self)->entity->transform.pos);
END_COMPONENT_UPDATE

void tv_ai_behavior_direct_set_speed(tv_ai_behavior_direct* self, tvfloat speed)
{
	self->speed = speed;
}
void tv_ai_behavior_direct_target_pos(tv_ai_behavior_direct* self, tv_vector3 pos)
{
	self->target_type = TV_AI_BEHAVIOR_TARGET_POS;
	((tv_ai_behavior*)self)->target.pos = pos;
}
void tv_ai_behavior_direct_target_node(tv_ai_behavior_direct* self, tv_vector3* node)
{
	self->target_type = TV_AI_BEHAVIOR_TARGET_NODE;
	((tv_ai_behavior*)self)->target.node = node;
}
void tv_ai_behavior_direct_target_entity(tv_ai_behavior_direct* self, tv_entity* entity)
{
	self->target_type = TV_AI_BEHAVIOR_TARGET_ENTITY;
	((tv_ai_behavior*)self)->target.entity = entity;
}
