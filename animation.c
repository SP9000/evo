#include "animation.h"

static UT_icd tv_animation_bone_icd = {sizeof(tv_animation_bone), NULL, NULL, NULL};
static UT_icd tv_animation_keyframe_icd = {sizeof(tv_animation_keyframe), NULL, NULL, NULL};
static UT_icd tv_model_icd = {sizeof(tv_model), NULL, NULL,NULL };

void tv_animation_translate_recursive_(tv_animation *animation, tvuint curr_bone, tv_vector3 offset, tv_vector4 rotation);

COMPONENT_NEW(tv_animation, tv_component)
	utarray_new(self->models, &tv_model_icd);
	utarray_new(self->keyframes, &tv_animation_keyframe_icd);
	self->num_bones = 0;
	self->num_root_bones = 0;
	self->t = 0;
END_COMPONENT_NEW(tv_animation)

COMPONENT_START(tv_animation)
END_COMPONENT_START

COMPONENT_UPDATE(tv_animation)
tv_animation_keyframe *keyframe_it;
tvuint i;

for(keyframe_it = (tv_animation_keyframe*)utarray_front(self->keyframes);
	keyframe_it != NULL;
	keyframe_it = (tv_animation_keyframe*)(self->keyframes, keyframe_it)) {
		/* if this keyframe is active, lerp it */
		if(self->t > keyframe_it->start_time && self->t < keyframe_it->stop_time) {
			self->bones[keyframe_it->bone].position = tv_vector3_lerp(
				keyframe_it->offset_start, 
				keyframe_it->offset_end, 
				keyframe_it->stop_time - keyframe_it->start_time);
			self->bones[keyframe_it->bone].rotation = tv_vector4_lerp(
				keyframe_it->rotation_start,
				keyframe_it->rotation_stop,
				keyframe_it->stop_time - keyframe_it->start_time);
		}
}
for(i = 0; i < self->num_root_bones; ++i) {
	tv_animation_translate_recursive_(self, i, tv_vector3_zero, tv_vector4_zero);
}
++self->t;
END_COMPONENT_UPDATE

void tv_animation_add_bone(tv_animation *animation, tv_animation_bone bone, tvuint parent_bone)
{
	animation->bones[animation->num_bones] = bone;
	/* add this bone to the list of the parent's child bones */
	animation->bones[parent_bone].children[animation->bones[parent_bone].num_children] = animation->num_bones;
	++animation->bones[parent_bone].num_children;

	/* if this bone is attached to the root of the animation, increment the
	 * count of root level bones */
	if(parent_bone == 0) {
		++animation->num_root_bones;
		assert(animation->num_root_bones < TV_ANIMATION_MAX_CHILDREN);
	}
	++animation->num_bones;
	assert(animation->num_bones < TV_ANIMATION_MAX_BONES);
}

void tv_animation_add_joint(tv_animation *animation, tvuint start_bone, tvuint end_bone)
{
	animation->bones[start_bone].children[animation->bones[start_bone].num_children] = end_bone;
	++animation->bones[start_bone].num_children;
}
tvuint tv_animation_add_model(tv_animation *animation, tv_model *model)
{
	utarray_push_back(animation->models, model);
	return utarray_eltidx(animation->models, model);
}

void tv_animation_play(tv_animation *animation, tvuint start, tvuint stop)
{
	animation->play_type = TV_ANIMATION_PLAYING;
	animation->play_start = start;
	animation->play_stop = stop;
	animation->t = start;
}

void tv_animation_loop(tv_animation *animation, tvuint start, tvuint stop)
{
	animation->play_type = TV_ANIMATION_LOOPING;
	animation->play_start = start;
	animation->play_stop = stop;
	animation->t = start;
}

void tv_animation_translate_recursive_(tv_animation *animation, tvuint curr_bone, tv_vector3 offset, tv_vector4 rotation)
{
	tvuint i;
	for(i = 0;  i < animation->bones[curr_bone].num_children; ++i) {
		/* accumulate the transformations to be applied to all bones that are part of this chain */
		animation->bones[curr_bone].rotation = tv_vector4_add(rotation, animation->bones[curr_bone].rotation);
		animation->bones[curr_bone].position = tv_vector3_add(offset, animation->bones[curr_bone].position);

		/* recursively transform all the bones that are attached (directly or indirectly) to this bone */
		tv_animation_translate_recursive_(animation, animation->bones[curr_bone].children[i], 
			animation->bones[curr_bone].position, 
			animation->bones[curr_bone].rotation);
	}
}

void tv_animation_load_armature(tv_animation *animation, const tvchar *file)
{
	FILE *fp = NULL;
	fopen(file, "r");
	
	if(fp == NULL) {
		fprintf(stderr, "Error: failed to open file %s\n", file);
	}
	if(animation == NULL) {
		animation = tv_animation_new();
	}
}