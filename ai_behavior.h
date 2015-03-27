#ifndef AI_BEHAVIOR_H
#define AI_BEHAVIOR_H

#ifdef __cplusplus
extern "C" {
#endif
#include "tv_component.h"
#include "tv_types.h"
#include "tv_alloc.h"

#define AI_BEHAVIOR_MAX_SEEK_TAGS 8

/**
 * A structure for holding the target of an artificial intelligence.
 */
typedef union {
	tv_vector3 pos;		/**< a position that is not changing */
	tv_vector3* node;	/**< a reference to a position (for moving positions) */
	tv_entity* entity;	/**< a reference to an entity */
}tv_ai_behavior_target;

/**
 * An enumeration of possible types that the AI is seeking.
 * This may or may not be used by an AI-behavior. Some AI-behaviors may 
 * implicitly choose the type of target they are seeking.
 */
typedef enum {
	TV_AI_BEHAVIOR_TARGET_POS,
	TV_AI_BEHAVIOR_TARGET_NODE,
	TV_AI_BEHAVIOR_TARGET_ENTITY
}tv_ai_target_type;

/** 
 * A structure representing a navigation point in the AI graph.
 */
typedef struct {
	tv_vector3 position;
}tv_ai_navpoint;

/**
 * One component contatinig the necessary information for an AI behavior to find 
 * its destination.
 * Derive this component to add the necessary information for the AI behavior 
 * routines that use it.
 * It is the job of the navgraph to find the next waypoint to reach a given
 * target destination.  AI-behaviors may or may not use the navgraph, and they
 * may or may not follow the waypoints it provides directly.
 */
COMPONENT(tv_ai_navgraph, tv_component) 
ENDCOMPONENT(tv_ai_navgraph)

/* gets the next closest navpoint from the source to dest position. */
tv_ai_navpoint get_next_navpoint(tv_vector3 src, tv_vector3 dest);


/**
 * A component for navigating a scene using the A* AI-behavior.
 */
COMPONENT(tv_ai_a_star_navgraph, tv_ai_navgraph)
ENDCOMPONENT(tv_ai_a_star_navgraph)

/**
 * The AI behavior component. 
 * Derive this component to implement any AI behaviors desired.
 */
COMPONENT(tv_ai_behavior, tv_component)
	/* the target of the AI */
	tv_ai_behavior_target target;
	/* if true, this AI behavior is actively seeking its target */
	tvbool seeking;
	/* the AI seek function */
	void (*seek)(struct tv_ai_behavior*);
	/* the representation of the area that this AI behavior is traversing */
	tv_ai_navgraph* graph;
	/* the distance closer a target must be before it becomes the new target.
	 * use TV_INF to never change targets or 0.0f to always seek the closest.
	 */
	tvfloat change_target_range;

	/* the seek tags of the AI-behavior */
	tvuint tags_to_seek[AI_BEHAVIOR_MAX_SEEK_TAGS];
	/* the number of these tags */
	tvuint num_seek_tags;
ENDCOMPONENT(tv_ai_behavior)

/**
 * A simple A* based AI-behavior suitable for many AI applications.
 * Derive this component to implement any AI behaviors desired.
 */
COMPONENT(tv_ai_behavior_a_star, tv_ai_behavior)
ENDCOMPONENT(tv_ai_behavior_a_star)

/**
 * Tells the given AI that it's new target is the given target.
 * @param self the AI to set the target of.
 * @param target the target that the AI is now trying to find.
 */
void tv_ai_behavior_set_target(tv_ai_behavior* self, tv_ai_behavior_target target);

/**
 * Commands the AI to begin seeking its target.
 * @param self the AI to begin searching for its target.
 */
void tv_ai_behavior_seek(tv_ai_behavior* self);

/**
 * Tells the given AI to stop searching for its target.
 * @param self the AI to cease the search with.
 */
void tv_ai_behavior_stop(tv_ai_behavior* self);

/** 
 * Sets the seek function of the given AI behavior.
 * @param self the AI behavior to set the seek function of.
 * @param func the new seek function.
 */
void tv_ai_behavior_set_seek_func(tv_ai_behavior* self, void (*func)(tv_ai_behavior*));

/**
 * A default seek function based on the popular A* algorithm.
 */
void ai_seek_a_star();

/**
 * Adds a type of tag that the given AI behavior is seeking.
 * Entities with tags in the "seek-list" become targets when they are closer to
 * this AI-behavior's position by "change_target_range".
 * @param self the AI-behavior to add the tag to.
 * @param tag the tag to begin seeking.
 */
void tv_ai_behavior_add_seek_tag(tv_ai_behavior* self, tvuint tag);

#ifdef __cplusplus
}
#endif
#endif
