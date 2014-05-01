#ifndef MATERIAL_H
#define MATERIAL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "component.h"
#include "types.h"
#include "tv_alloc.h"
#include "cJSON.h"
#include "util.h"

#define TV_MATERIAL_BUFFER_BINDING_POINT 1

#if 0
typedef enum {
	TV_MATERIAL_EFFECT_INTERFACE_TYPE_FLOAT,
	TV_MATERIAL_EFFECT_INTERFACE_TYPE_VEC2,
	TV_MATERIAL_EFFECT_INTERFACE_TYPE_VEC3,
	TV_MATERIAL_EFFECT_INTERFACE_TYPE_VEC4,
	TV_MATERIAL_EFFECT_INTERFACE_TYPE_MAT4X1,
	TV_MATERIAL_EFFECT_INTERFACE_TYPE_MAT4X4
}tv_material_effect_interface_element_type;

typedef enum {
	TV_MATERIAL_EFFECT_INTERFACE_IO_IN,
	TV_MATERIAL_EFFECT_INTERFACE_IO_OUT,
	TV_MATERIAL_EFFECT_INTERFACE_IO_INOUT,
	TV_MATERIAL_EFFECT_INTERFACE_IO_UNIFORM,
	TV_MATERIAL_EFFECT_INTERFACE_IO_GLOBAL_UNIFORM
}tv_material_effect_interface_element_io_dir;

typedef struct tv_material_effect_interface_element {
	tv_material_effect_interface_element_type type;
	tv_material_effect_interface_element_io_dir io_dir;
	tvchar name[32];
}tv_material_effect_interface_element;

/** 
 * The material effect interface defines the way that effects can connect to
 * each other. 
 */
typedef struct tag_tv_material_effect_interface {
	tv_array /*tv_material_effect_interface_element*/ *in_params;
	tv_array /*tv_material_effect_interface_element*/ *out_params;
	tv_array /*tv_material_effect_interface_element*/ *uniform_params;
	tv_array /*tv_material_effect_interface_element*/ *ubo_params;
	tvchar name[32];
} tv_material_effect_interface;

typedef struct tv_material_effect {
	tv_material_effect_interface effect_interface;
	tvchar name[32];
}tv_material_effect;
#endif

COMPONENT(tv_material, tv_component) 
	tvchar* name;
	GLuint program;

	GLuint model_mat;
	GLuint view_mat;
	GLuint projection_mat;

	/* the location of the uniform buffer this material uses. */
	GLuint buffer;
	/* an array of effects in the order they are used. */
	/* tv_array *effects; */

	/* if TRUE, this model needs access to the scene's lighting information */
	tvbool lit;

	TvHashHandle hh;
ENDCOMPONENT(tv_material)

typedef struct tagMaterialShader {
	tvchar* name;
	tvuint id;
	TvHashHandle hh;
}TvMaterialShader;

GLuint tv_material_compile_shader(GLchar* shader, GLuint type);
GLuint tv_material_compile_program(GLuint vert_shader, GLuint frag_shader,
								   GLuint geom_shader, tvchar **attributes, 
								   tvuint num_attributes);
tvint tv_material_get_uniform(tv_material *material, tvchar *name);

void tv_material_get_uniforms(GLuint program, GLuint* model, GLuint* view, GLuint* projection);


/**
 * Loads a material from the given file.
 * @param material the material to load with the data supplied by the file.
 * @param file the filename of the file containg the material data.
 */
void tv_material_load(tv_material *material, const char* file);

#if 0
/** 
 * Adds a pass to the given material.
 * The pass is inserted at the given location in the material's pass array.
 * @param material the material to add the pass to.
 * @param index the index to insert the pass at.
 * @param pass the material that will be used to render this pass.
 */
void tv_material_add_pass(tv_material *material, tvint index, tv_material *pass);
/** 
 * Appends a pass to the given material.
 * @param material the material to add the pass to.
 * @param pass the material that will be used to render this pass.
 */
void tv_material_append_pass(tv_material *material, tv_material *pass);
/** 
 * Prepends a pass to the given material.
 * @param material the material to add the pass to.
 * @param pass the material that will be used to render this pass.
 */
void tv_material_prepend_pass(tv_material *material, tv_material *pass);
/**
 * Removes the given pass' material from the given material.
 * @param material the material to remove the pass from.
 * @param pass the pass to remove.
 */
void tv_material_remove_pass(tv_material *material, tv_material *pass);

/** 
 * Sets a parameter for the given material.
 * This is used to set a uniform variable for the given material's shader
 * program.
 * @param material the material to set the parameter of.
 * @param param_name the name of the parameter to set.
 * @param data_size the size of the data to set.
 * @param data the data to set the parameter to.
 */
void tv_material_set_param(tv_material *material, const tvchar *param_name, size_t data_size, tvpointer data);

/**
 * Loads a material effect from the file given by the filename.
 * @param filename the name of the file containing the effect information.
 * @return a reference to the newly created/loaded effect.
 */
tv_material_effect* tv_material_effect_load(tvchar *filename);
/** 
 * Connects effect2 to effect1 using their defined interfaces.
 * If the interfaces of effect1 and effect2 do not match up, the effects are
 * not joined (obviously...)
 * @param material the material containing the effects to join.
 * @param effect1 the effect to connect to effect2 (via OUT parameters).
 * @param effect2 the effect that receives effect1's input (via IN parameters).
 */
void tv_material_effect_connect(tv_material *material, tv_material_effect *effect1, tv_material_effect *effect2);

/**
 * Optimize the material after all desired altercations have been made to it.
 * This function should be called after all the effects and passes that are desired
 * have been added to it.  Once this function is called no existing individual
 * effects may be added or removed from the material without re-optimizing.
 * @param material the material to optimize.
 */
void tv_material_optimize(tv_material *material);
#endif

#ifdef __cplusplus
}
#endif
#endif