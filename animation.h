#ifndef ANIMATION_H
#define ANIMATION_H
#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include "evo.h"

/* the maximum number of bones the root of a model may have */
#define TV_ANIMATION_MAX_BONES 128
/* the maximum # of bones that can be connected to another */
#define TV_ANIMATION_MAX_CHILDREN 10

typedef struct tv_animation_bone {
	tv_vector3 position;
	tv_vector4 rotation;
	tvuint num_children;
	tvuint children[TV_ANIMATION_MAX_CHILDREN];
}tv_animation_bone;

typedef struct tv_animation_keyframe {
	tvuint start_time;
	tvuint stop_time;

	tvuint bone;
	tv_vector3 offset_start;
	tv_vector3 offset_end;
	tv_vector4 rotation_start;
	tv_vector4 rotation_stop;
}tv_animation_keyframe;

typedef enum {
	TV_ANIMATION_NOT_PLAYING,
	TV_ANIMATION_PLAYING,
	TV_ANIMATION_LOOPING
}tv_animation_play_type;

/**
 * The animation component.
 */
COMPONENT(tv_animation, tv_component)
	/* the models for the animation - each model corresponds to 1 bone. */
	tv_array /*tv_model*/ *models;

	/* the "time" the animation is currently at (in terms of keyframe time). */
	tvuint t;
	/* where the animation's current playback is started. */
	tvuint play_start;
	/* where the animation's current playback is set to end. */
	tvuint play_stop;
	/* what the animation is currently doing (playing, looping, etc.) */
	tv_animation_play_type play_type;
	/* an array of keyframes - this defines how the models are animated. */
	tv_array /*tv_animation_keyframe*/ *keyframes;

	/* the number of bones in this animation. */
	tvuint num_bones;
	/* the number of bones that are attached to the root of the animation */
	tvuint num_root_bones;
	/* an array of arrays. this represents the root of the animation. each 
	 * array within this array represents one "path" from the root. 
	 * e.g. root->upper_arm->lower_arm->hand->fingers
	 * This chain is used to apply changes to all connected bones.
	 */
	tv_animation_bone bones[TV_ANIMATION_MAX_BONES];
ENDCOMPONENT(tv_animation)

/**
 * Creates a "joint" between the two specified bones.
 * A joint is given by a start and end bone.  In the following example A is the
 * start bone for the joint of A-B and B the end bone.  For the B-D joint, B is
 * the start bone and D the end, and so on...
 *   A
 *  / \
 * B   C
 * |   |\ 
 * D   E F
 * Essentially this function parents the end_bone to the start_bone.  
 * @param animation the animation to add the joint to.
 * @param start_bone the bone that leads to the end_bone.
 * @param the bone that the start_bone is attached to.
 */
void tv_animation_add_joint(tv_animation *animation, tvuint start_bone, tvuint end_bone);

/** 
 * Adds the specified model to the given animation component. 
 * The model must not be deallocated until it is removed from the animation.
 * @param animation the animation to add the model to.
 * @param model the model to add to the animation.
 * @return the index (or ID) that will be used to refer to this model.
 */
tvuint tv_animation_add_model(tv_animation *animation, tv_model *model);

/**
 * Play the given animation once.
 * Plays the animation given by the start offset.
 * @param animation the animation component to animate.
 * @param start the frame to begin the animation at.  
 * @param stop the frame to end the animation at.
 */
void tv_animation_play(tv_animation *animation, tvuint start, tvuint stop);

/**
 * Loads an armature defined in the given file into the given animation.
 * @param animation the animation to load into.
 * @param file the file to load from.
 */
void tv_animation_load_armature(tv_animation *animation, const tvchar *file);

#ifdef __cplusplus
}
#endif
#endif