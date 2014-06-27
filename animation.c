#include "animation.h"

static UT_icd tv_animation_bone_icd = {sizeof(tv_animation_bone), NULL, NULL, NULL};
static UT_icd tv_animation_keyframe_icd = {sizeof(tv_animation_keyframe), NULL, NULL, NULL};
static UT_icd tv_model_icd = {sizeof(tv_model), NULL, NULL,NULL };

COMPONENT_NEW(tv_animation, tv_component)
	tvuint i;

	utarray_new(self->keyframes, &tv_animation_keyframe_icd);
	self->num_bones = 0;
	self->num_root_bones = 0;
	self->num_sequences = 0;
	self->t = 0;
	/* initialize the bones array. */
	for(i = 0; i < TV_ANIMATION_MAX_BONES; ++i) {
		self->bones[i].model = NULL;
		self->bones[i].next_child = TV_ANIMATION_BONE_END;
		self->bones[i].next_sibling = TV_ANIMATION_BONE_END;
		self->bones[i].position.x = 0.0f;
		self->bones[i].position.y = 0.0f;
		self->bones[i].position.z = 0.0f;
		self->bones[i].rotation.x = 0.0f;
		self->bones[i].rotation.y = 0.0f;
		self->bones[i].rotation.z = 0.0f;
	}
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

}
++self->t;
END_COMPONENT_UPDATE

void tv_animation_set_root(tv_animation *animation, tv_model *model, tv_material *material)
{
	animation->bones[0].model = model;
	animation->bones[0].material = material;
	animation->bones[0].next_child = TV_ANIMATION_BONE_END;
	animation->bones[0].next_sibling = TV_ANIMATION_BONE_END;
	animation->bones[0].position.x = 0.0f;
	animation->bones[0].position.y = 0.0f;
	animation->bones[0].position.z = 0.0f;
	animation->bones[0].rotation.x = 0.0f;
	animation->bones[0].rotation.y = 0.0f;
	animation->bones[0].rotation.z = 0.0f;
}

tvuint tv_animation_add_bone(tv_animation *animation, tvuint parent, tv_animation_bone bone)
{
	tvuint i, ret;

	/* check that there is room for a new bone. */
	assert(animation->num_bones < TV_ANIMATION_MAX_BONES);

	/* if the parent has no child, then this bone's ID is the parent's + 1 */
	if(animation->bones[parent].next_child == TV_ANIMATION_BONE_END) {
		ret = 1; 
		animation->bones[parent].next_child = 1;
	}
	/* if there are children already, traverse siblings til a NULL is found */
	else {
		for(ret = animation->bones[parent].next_child; 
			ret != TV_ANIMATION_BONE_END;  
			ret = animation->bones[ret].next_child) ;
	}
	/* bump all bones that are affected by the change. */
	for(i = TV_ANIMATION_MAX_BONES-1; i > ret; --i) {
		if(animation->bones[i].next_child > i) {
			++animation->bones[i].next_child;
		}
		if(animation->bones[i].next_sibling > i) {
			++animation->bones[i].next_sibling;
		}
		animation->bones[i+1] = animation->bones[i];
	}
	/* set the bone. */
	animation->bones[ret] = bone;
	animation->bones[ret].next_child = TV_ANIMATION_BONE_END;
	animation->bones[ret].next_sibling = TV_ANIMATION_BONE_END;
	return ret;	
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

void tv_animation_play_sequence(tv_animation *animation, tvuint sequence_id)
{
	animation->play_start = animation->sequences[sequence_id].start;
	animation->play_stop = animation->sequences[sequence_id].stop;
}

tvuint tv_animation_register_sequence(tv_animation *animation, tvchar *name, tvuint start, tvuint stop)
{
	strncpy(animation->sequences[animation->num_sequences].name, name, 31);
	animation->sequences[animation->num_sequences].start = start;
	animation->sequences[animation->num_sequences].stop = stop;
	assert((animation->num_sequences + 1) < TV_ANIMATION_MAX_SEQUENCES);
	return animation->num_sequences++;
}

tvuint tv_animation_get_sequence(tv_animation *animation, tvchar *name)
{
	tvuint i;
	for(i = 0; i < animation->num_sequences; ++i) {
		if(strncmp(animation->sequences[i].name, name, 31) == 0) {
			return i;
		}
	}
	fprintf(stderr, "Error: requested sequence name %s does not exist in the animation.\n", name);
	return 0;
}

void tv_animation_replace_bone(tv_animation *animation, tvuint loc, tv_animation_bone bone)
{
	tvuint child = animation->bones[loc].next_child;
	tvuint sibling = animation->bones[loc].next_sibling;
	bone.next_child = child;
	bone.next_sibling = sibling;
	animation->bones[loc] = bone;
}
