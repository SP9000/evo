#include "animation.h"

/* an ICD to store boenes */
static UT_icd tv_animation_bone_icd = {sizeof(tv_animation_bone), NULL, NULL, NULL};
/* an ICD to store animation keyframes */
static UT_icd tv_animation_keyframe_icd = {sizeof(tv_animation_keyframe), NULL, NULL, NULL};
/* an ICD to store models */
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
	self->bone_name_table = NULL;
	tv_stack_new(self->free_bone_ids, &ut_int_icd);
	self->next_free_bone_id = 1;
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

/*****************************************************************************/
/* bone setting/adding/removing */
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
	strncpy(animation->bones[0].name, "root", 4);
}
tvuint tv_animation_add_bone(tv_animation *animation, tvuint parent, tv_animation_bone bone)
{
	tvuint i, ret;
	tvuint sibling_id;
	tv_animation_bone_key *key;

	/* check that there is room for a new bone. */
	assert(animation->num_bones < TV_ANIMATION_MAX_BONES);
	/* get an ID for this bone */
	if(!tv_stack_empty(animation->free_bone_ids)) {
		tv_stack_pop(animation->free_bone_ids, &ret);
	}
	else {
		ret = animation->next_free_bone_id++;
	}
	/* if the parent has no child, then this bone's ID is the parent's + 1 */
	if(animation->bones[parent].next_child == TV_ANIMATION_BONE_END) {	
		animation->bones[parent].next_child = ret;
	}
	/* if there are children already, traverse siblings til an end is found */
	else {
		for(sibling_id = animation->bones[parent].next_child; 
			animation->bones[sibling_id].next_sibling != TV_ANIMATION_BONE_END;  
			sibling_id = animation->bones[sibling_id].next_sibling) ;
		animation->bones[sibling_id].next_sibling = ret;
	}
	/* set the bone. */
	animation->bones[ret] = bone;
	animation->bones[ret].parent = parent;
	animation->bones[ret].next_sibling = TV_ANIMATION_BONE_END;
	animation->bones[ret].next_child = TV_ANIMATION_BONE_END;

	/* add the bone to the name lookup table */
	key = (tv_animation_bone_key*)malloc(sizeof(tv_animation_bone_key));
	strncpy(key->name, bone.name, 31);
	key->id = ret;
	HASH_ADD_STR(animation->bone_name_table, name, key);

	++animation->num_bones;
	return ret;	
}
tvuint tv_animation_add_empty(tv_animation *animation, tvuint parent, tvchar *name)
{
	tv_animation_bone bone;
	tv_animation_bone_key *key;

	bone.material = NULL;
	bone.model = NULL;
	bone.next_child = TV_ANIMATION_BONE_END;
	bone.next_sibling = TV_ANIMATION_BONE_END;
	bone.position.x = 0;
	bone.position.y = 0;
	bone.position.z = 0;
	bone.rotation.x = 0;
	bone.rotation.y = 0;
	bone.rotation.z = 0;
	strncpy(bone.name, name, 31);
	return tv_animation_add_bone(animation, parent, bone);
}
void tv_animation_replace_bone(tv_animation *animation, tvuint loc, tv_animation_bone bone)
{
	tvuint child = animation->bones[loc].next_child;
	tvuint sibling = animation->bones[loc].next_sibling;
	
	assert(loc < TV_ANIMATION_MAX_BONES);
	/* TODO: delete old bone from table */

	bone.next_child = child;
	bone.next_sibling = sibling;
	bone.parent = animation->bones[loc].parent;
	animation->bones[loc] = bone;
}
tvuint tv_animation_get_bone(tv_animation *animation, tvchar *name)
{
	tv_animation_bone_key *lup;
	HASH_FIND_STR(animation->bone_name_table, name, lup);
	return lup->id;
}
void tv_animation_remove_bone(tv_animation *animation, tvuint bone_id)
{
	/* 9   1   8                  9   1   8
	   |   |   |                  |   |   |
	   2 - 3 - 4       =>         2 - 5 - 4
	   |   |   |    remove 3      |       | 
	   6   5   7                  6       7 */
	/* make sure the bone we want to remove actually exists */
	if(animation->bones[bone_id].next_child) { 
		/* connect this bone's two siblings */
		animation->bones[animation->bones[bone_id].prev_sibling].next_sibling = animation->bones[bone_id].next_sibling;
		animation->bones[animation->bones[bone_id].next_sibling].prev_sibling = animation->bones[bone_id].prev_sibling;
		/* connect this bone's child and parent */
		animation->bones[animation->bones[bone_id].parent].next_child = animation->bones[bone_id].next_child;
		animation->bones[animation->bones[bone_id].next_child].parent = animation->bones[bone_id].parent;
	}
}

/*****************************************************************************/
/* keyframe animation/sequencing */
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

/*****************************************************************************/
/* Armature loading */
void tv_animation_load_armature_r_(tv_animation *animation, tvuint parent, cJSON *node)
{
	tvuint bone_id;
	/* foreach sibling bone */
	for(bone_id = 0; node != NULL; node = node->next) {
		/* if there is are children, recursively add them */
		if(node->child != NULL) {
			tvuint new_parent = tv_animation_add_empty(animation, parent, node->string);
			tv_animation_load_armature_r_(animation, new_parent, node);
		}	
	}
}
void tv_animation_load_armature(tv_animation *animation, const tvchar *filename)
{
	cJSON *root;
	cJSON *node;
	tvchar *text;
	tvuint bone_id;

	UtilReadFile(filename, &text);
	root = cJSON_Parse(text);

	/* check that the file could be parsed */
	if(!root) {
		tv_warning("JSON parse error before: [%s]\n", cJSON_GetErrorPtr());
		return;
	}
	root = root->child;
	/* ensure we're dealing with an armature */
	if(strncmp(root->string, "armature", 8) != 0) {
		tv_warning("expected JSON object named \"armature\" not found in armature %s", filename);
		return;
	}
	tv_animation_load_armature_r_(animation, 0, node);
}
