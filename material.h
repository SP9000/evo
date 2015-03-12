#ifndef MATERIAL_H
#define MATERIAL_H
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
Material
Materials have to main "sections". 
The first is the shader programs.
Each material has a number of "passes".  Each "pass" has one vertex, one
fragment, and (optionally) one geometry shader associated with it.
Passes are executed in the order they are added to the material unless 
explicity reordered. 
Each pass also contains a list of uniforms and/or uniform buffers required by 
their associated shaders.  
The list of uniforms is stored and maintained by the material. For all passes
to function properly, it is important that all the uniforms and uniform buffers 
that the pass requires are added to the material prior to rendering.
A typical material creation flow looks like this:
1. create material
2. load pass_0
	2a. load pass_0 uniforms
3. load pass_1
	3a. load pass_1 uniforms 
4. done
etc.
******************************************************************************/

/*****************************************************************************/
/* Includes */
#include "tv_component.h"
#include "tv_types.h"
#include "tv_alloc.h"
#include "cJSON.h"
#include "tv_util.h"
#include "tv_macros.h"
#include "tv_matrix.h"
#include "model.h"

/*****************************************************************************/
/* Constant Definitions */
#define TV_MATERIAL_BUFFER_BINDING_POINT 1
/* the maximum # of uniform blocks per material */
#define TV_MATERIAL_MAX_UBOS 8
/* the maximum # of passes that a material can have */
#define TV_MATERIAL_MAX_PASSES 4
/* the maximum # of attributes in each uniform block */
#define TV_MATERIAL_MAX_UBO_ATTRIBUTES 16

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

/*****************************************************************************/
/* Types */
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

/**
 * A structure containing information about an OpenGL uniform-buffer block.
 */
typedef struct {
	/* the handle of this uniform buffer */
	GLuint id;
	/* the offset of each attribute within this UBO */
	GLint offsets[TV_MATERIAL_MAX_UBO_ATTRIBUTES];
	/* the size of each attribute within this UBO */
	tvuint sizes[TV_MATERIAL_MAX_UBO_ATTRIBUTES];
	/* the buffer containing the attribute data */
	GLubyte *buffer;
	/* the size of the unifrom buffer */
	GLint size;
	/* the name of the uniform block. */
	tvchar name[31];
}TV_material_uniform_block;

/**
 * A hashable structure containing information about a shader program.
 */
typedef struct tagMaterialShader {
	tvchar* name;
	tvuint id;
	TvHashHandle hh;
}TvMaterialShader;

/**
 * A structure containing the individual shaders belonging to a shader program.
 */
typedef struct {
	tvuint vertex;
	tvuint fragment;
	tvuint geometry;
	tvuint tess_control;
	tvuint tess_eval;
}tv_material_program;

/**
 * The structure for a material pass.
 */
typedef struct {
	/* the name of the pass */
	tvchar* name;
	/* the shader program for the pass */
	GLuint program;
	/* the matrices for the pass TODO: use shared UBO */
	GLuint modelview_mat;
	GLuint projection_mat;

	/* the attributes (uniforms) of the pass */
	const tvchar** attributes;
	tvuint num_attributes;

	/* the (shared) uniform blocks for this pass */
	const tvchar ubo_names[TV_MATERIAL_MAX_UBOS][31];
	GLuint *ubos;
	tvuint num_ubos;

	/* if TRUE, this pass needs access to the scene's lighting information */
	tvbool lit;

	/* a hash handle so redundant passes are not loaded. */
	TvHashHandle hh;
}tv_material_pass;

/*****************************************************************************/
/* Component */
COMPONENT(tv_material, tv_component) 
	/* the name of the material */
	tvchar* name;

	tvuint num_passes;
	/* the passes run when rendering with this material. */
	tv_material_pass* passes[TV_MATERIAL_MAX_PASSES];

	/* a hash handle so redundant materials are not loaded. */
	TvHashHandle hh;
ENDCOMPONENT(tv_material)


/*****************************************************************************/
/* Methods */
/**
 * Loads a material from the given file.
 * @param file the filename of the file containg the material data.
 */
void METHOD(tv_material, load, const char* file);

/**
 * Compile a shader program.
 * @param vert_shader the vertex shader for the shader program.
 * @param frag_shader the fragment shader for the shader program.
 * @param geom_shader the geometry shader for the shader program.
 * @param num_attributes the number of attributes for the given shader.
 * @return the handle of the compiled shader program.
 */
GLuint METHOD(tv_material, compile_program, GLuint vert_shader, GLuint frag_shader,
								   GLuint geom_shader, tvchar **attributes, 
								   tvuint num_attributes);
/**
 * Retrieve the handle of a uniform of the given name from the material.
 * @param name the name of the uniform.
 * @return the handle of the uniform.
 */
tvint METHOD(tv_material, get_uniform, tvchar *name);

/**
 * Retrieve the handle of a uniform block in the material.
 * @param name the name of the uniform block.
 * @return the handle of the uniform block.
 */
tvint METHOD(tv_material, get_uniform_block, const tvchar *name);

/**
 * Set a uniform buffer attribute for the material.
 * @param material the material to set the attribute of.
 * @param ubo the name of the UBO data to buffer.
 * @param data the attribute data for the uniform block.
 * @param size the size of the data to buffer.
 */
void METHOD(tv_material, set_ubo_attribute, tv_material *material, const tvchar* block, void *data, tvuint size);

/** 
 * Adds a pass to the material.
 * @param material the material to add the pass to.
 * @param pass the material that will be used to render this pass.
 */
void METHOD(tv_material, add_pass, tv_material_pass *pass);

/**
 * Sets the OpenGL rendering up for rendering with the specified pass.
 * This function does the glUseProgram and the binding/updating of any uniforms
 * associated with the pass.
 * @param pass_number the index of the pass to begin using.
 */
void METHOD(tv_material, use_pass, tvuint pass);
void METHOD(tv_material, use_pass_gui, tvuint pass_idx);

/** 
 * Render the currently bound vertex attribute array with the given pass.
 * @param pass_index the index of the pass to render with.
 * @param model the model to render.
 */
void METHOD(tv_material, do_pass, tvuint pass_index, tv_model* model);
void METHOD(tv_material, do_pass_instanced, tvuint pass_index, tv_model* model, tvuint count);
void METHOD(tv_material, do_pass_gui, tvuint pass_index, tv_model* model);

#if 0
/** 
 * Appends a pass to the material.
 * @param material the material to add the pass to.
 * @param pass the material that will be used to render this pass.
 */
void METHOD(tv_material, append_pass, tv_material *pass);
/** 
 * Prepends a pass to the material.
 * @param material the material to add the pass to.
 * @param pass the material that will be used to render this pass.
 */
void METHOD(tv_material, prepend_pass, tv_material *pass);
/**
 * Removes the given pass from the material.
 * @param material the material to remove the pass from.
 * @param pass the pass to remove.
 */
void METHOD(tv_material, remove_pass, tv_material *pass);

/** 
 * Sets a parameter for the material.
 * This is used to set a uniform variable for the material's shader
 * program.
 * @param param_name the name of the parameter to set.
 * @param data_size the size of the data to set.
 * @param data the data to set the parameter to.
 */
void METHOD(tv_material, set_param, const tvchar *param_name, size_t data_size, tvpointer data);

/**
 * Loads a material effect from the file given by the filename.
 * @param filename the name of the file containing the effect information.
 * @return a reference to the newly created/loaded effect.
 */
tv_material_effect* METHOD(tv_material, effect_load, tvchar *filename);
/** 
 * Connects effect2 to effect1 using their defined interfaces.
 * If the interfaces of effect1 and effect2 do not match up, the effects are
 * not joined (obviously...)
 * @param material the material containing the effects to join.
 * @param effect1 the effect to connect to effect2 (via OUT parameters).
 * @param effect2 the effect that receives effect1's input (via IN parameters).
 */
void METHOD(tv_material, effect_connect, tv_material_effect *effect1, tv_material_effect *effect2);

/**
 * Optimize the material after all desired altercations have been made to it.
 * This function should be called after all the effects and passes that are desired
 * have been added to it.  Once this function is called no existing individual
 * effects may be added or removed from the material without re-optimizing.
 * @param material the material to optimize.
 */
void METHOD(tv_material_optimize);
#endif

/*****************************************************************************/
/* Non-method functions */
/**
 * Get the camera uniforms associated with a given OpneGL program.
 * @param program the program to get the camera unifroms of.
 * @param modelview the modelview matrix uniform for the given program.
 * @param projection the projection matrix uniform for the given program.
 */
void tv_material_get_uniforms(GLuint program, GLuint* modelview, GLuint* projection);

#ifdef __cplusplus
}
#endif
#endif