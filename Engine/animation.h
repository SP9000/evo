#ifndef ANIMATION_H
#define ANIMATION_H
#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include "model.h"
#include "material.h"
#include "cJSON.h"
#include "tv_tree.h"
#include "tv_stack.h"

/* the maximum number of bones the root of a model may have */
#define TV_ANIMATION_MAX_BONES 128
/* the maximum # of bones that can be connected to another */
#define TV_ANIMATION_MAX_CHILDREN 10
/* the maximum # of animation sequences - WALK, ATTACK, CAST etc. */
#define TV_ANIMATION_MAX_SEQUENCES 32
/* this specifies that a bone has no child (or sibling) */
#define TV_ANIMATION_BONE_END UINT_MAX


typedef struct tv_animation_bone {
	tv_vector3 position;
	tv_vector4 rotation;
	tv_model *model;
	tv_material *material;

	/* used for iterating through the bones in an animation */
	tvuint prev_sibling, next_sibling;
	tvuint parent, next_child;

	/* the name of the bone */
	tvchar name[31];
	TvHashHandle hh;
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

typedef struct tv_animation_sequence {
	tvuint start;
	tvuint stop;
	tvchar name[31];
}tv_animation_sequence;

typedef enum {
	TV_ANIMATION_NOT_PLAYING,
	TV_ANIMATION_PLAYING,
	TV_ANIMATION_LOOPING
}tv_animation_play_type;

typedef struct tv_animation_bone_key {
	tvuint id;
	tvchar name[31];
	TvHashHandle hh;
}tv_animation_bone_key;

/**
 * The animation component.
 */
COMPONENT(tv_animation, tv_component)
	/* the materials for each model. */
	tv_array /*tv_material*/ *materials;

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

	/* the number of registered sequences */
	tvuint num_sequences;
	/* the animation sequences. */
	tv_animation_sequence sequences[TV_ANIMATION_MAX_SEQUENCES];

	/* a table for looking up bone's IDs based upon their names */
	tv_animation_bone_key *bone_name_table;
	/* a queue containing free bone ID's */
	TV_stack *free_bone_ids;
	/* the next unused bone ID (assuming free bone stack is empty) */
	tvuint next_free_bone_id;

	/* an array of arrays. this represents the root of the animation. each 
	 * array within this array represents one "path" from the root. A value
	 * at the start of each level of the tree tells how many nodes are in that
	 * level of the tree.
	 *    ROOT
	 *     |       
	 * r_upper_arm -- l_upper_arm  -- r_upper_leg
	 *     |               |               |
	 * r_lower_arm    l_lower_arm     r_lower_leg
	 *     |               |               |
	 * r_hand           l_hand          r_foot
	 *     |            ...             ...
	 * r_finger0  -- r_finger1 -- r_finger2 -- rfinger3 -- rfinger4
	 * stored as:
	 * [ROOT, (r_upper_arm,l_upper_arm), (r_lower_arm,NULL), (r_hand,NULL), 
	 * (NULL,r_finger0),(NULL,r_finger1),(NULL,r_finger2),(NULL,r_finger3),
	 * (NULL,r_finger4),(l_lower_arm,r_upper_arm),(l_hand,NULL)
	 * This chain is used to apply changes to all connected bones.
	 */
	tv_animation_bone bones[TV_ANIMATION_MAX_BONES];
ENDCOMPONENT(tv_animation)

/**
 * Sets the root to the given model.
 * This is a convienience function for simple animations that don't require any
 * bones (only a simple model).
 * @param animation the animation to set the root of.
 * @param model the model to make the root of the animation.
 * @param material the material for the given model.
 */
void tv_animation_set_root(tv_animation *animation, tv_model *model, tv_material *material);
/**
 * Adds a bone to the given parent bone.
 * @param animation the animation to add the bone to.
 * @param parent the bone that leads to the bone to add (0 is root).
 * @param the index that can be used to reference the added bone.
 */
tvuint tv_animation_add_bone(tv_animation *animation, tvuint parent, tv_animation_bone bone);
/**
 * Adds a location for a bone and returns its ID.
 * @param animation the animation to reserve a bone within.
 * @param parent the parent bone of the bone to reserve.
 * @return the ID of a new reserved bone.
 */
tvuint tv_animation_add_empty(tv_animation *animation, tvuint parent, tvchar *name);
/**
 * Replace the specified bone with the given bone.
 * @param animation the animation to replace the bone of.
 * @param loc the location (or ID) of the bone to replace.
 * @param bone the bone to replace the old bone with.
 */
void tv_animation_replace_bone(tv_animation *animation, tvuint loc, tv_animation_bone bone);
/**
 * Remove the given bone from the given animation.
 * @param animation the animation to remove the bone from.
 * @param bone_id the ID of the bone to remove.
 */
void tv_animation_remove_bone(tv_animation *animation, tvuint bone_id);

/**
 * Get the ID of the bone by its name
 * @param animation the animation to get the bone of.
 * @param the name of the bone to retrieve.
 * @return returns TV_ANIMATION_BONE_END if no bone is found, else the bone.
 */
tvuint tv_animation_get_bone(tv_animation *animation, tvchar *name);

/** 
 * Plays the given sequence within the animation.
 * @param animation the animation to play the sequence within.
 * @param sequence_id the ID of the sequence to play.
 */
void tv_animation_play_sequence(tv_animation *animation, tvuint sequence_id);
/** 
 * Registers the given sequence within the animation.
 * @param animation the animation to register the sequence to.
 * @param name the name of the sequence to register.
 * @param start the keyframe to begin the sequence at.
 * @param stop the keyframe to end the sequence at.
 * @return the ID of the sequence registered.
 */
tvuint tv_animation_register_sequence(tv_animation *animation, tvchar *name, tvuint start, tvuint stop);

/**
 * Retrieve the ID for the desired sequence by its name.
 * @param animation the animation containing the desired sequence.
 * @param name the name of the sequence requested.
 * @return the ID of the sequence.
 */
tvuint tv_animation_get_sequence(tv_animation *animation, tvchar *name);

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