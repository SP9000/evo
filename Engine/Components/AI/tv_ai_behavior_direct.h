#ifndef TV_AI_BEHAVIOR_DIRECT_H
#define TV_AI_BEHAVIOR_DIRECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ai_behavior.h"
COMPONENT(tv_ai_behavior_direct, tv_ai_behavior)
	/* the speed that the AI moves toward its target once acquired. */
	tvfloat speed;
	/* the range that the AI must be within before beginning movement */
	tvfloat aggro_range;
	/* the type of the target being sought */
	tv_ai_target_type target_type;
ENDCOMPONENT(tv_ai_behavior_direct)

void tv_ai_behavior_direct_set_speed(tv_ai_behavior_direct* self, tvfloat speed);
void tv_ai_behavior_direct_target_pos(tv_ai_behavior_direct* self, tv_vector3 pos);
void tv_ai_behavior_direct_target_node(tv_ai_behavior_direct* self, tv_vector3* node);
void tv_ai_behavior_direct_target_entity(tv_ai_behavior_direct* self, tv_entity* entity);

#ifdef __cplusplus
}
#endif
#endif
