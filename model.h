#ifndef MODEL_H
#define MODEL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "component.h"

#define MODEL_ATTRIBUTE_VERTEX_NUM_ELEMENTS 3
#define MODEL_ATTRIBUTE_COLOR_NUM_ELEMENTS 4
#define MODEL_ATTRIBUTE_NORMAL_NUM_ELEMENTS 3
#define MODEL_ATTRIBUTE_TEXCO_NUM_ELEMENTS 2
#define MODEL_ATTRIBUTE_INDEX_NUM_ELEMENTS 1

#define MODEL_ATTRIBUTE_VERTEX_SIZE (sizeof(float)*3)
#define MODEL_ATTRIBUTE_COLOR_SIZE  (sizeof(float)*4)
#define MODEL_ATTRIBUTE_NORMAL_SIZE (sizeof(float)*3)
#define MODEL_ATTRIBUTE_TEXCO_SIZE  (sizeof(float)*2)
#define MODEL_ATTRIBUTE_INDEX_SIZE  (sizeof(GLshort)*1)

enum {
    MODEL_ATTRIBUTE_NONE,
    MODEL_ATTRIBUTE_VERTEX,
    MODEL_ATTRIBUTE_COLOR,
    MODEL_ATTRIBUTE_NORMAL,
    MODEL_ATTRIBUTE_TEXCO,
	MODEL_ATTRIBUTE_INDEX
};

COMPONENT(tv_model, tv_component) 
	GLuint vao;
	GLuint primitive;
	GLuint num_vertices;
	GLuint num_indices;
	tvuint num_attributes;

	TvArray /*TvArray<GLvoid>*/ *attributes;
	TvArray /*tvuint*/ *attribute_sizes;
	TvArray /*tvuint*/ *attribute_types;
	TvArray /*GLshort*/ *indices;
	GLuint *vbo_ids;

	const tvchar *name;
	TvHashHandle hh;
ENDCOMPONENT(tv_model)

int tv_model_init();

/**
 * Loads a PLY model defined in the file of the given name.
 * @param model the model to load the file into.
 * @param file the file containing the data to load into the model.
 */
void tv_model_load_ply(tv_model *model, tvchar* file);

/**
 * Allocates room for an attribute of the given type in the given model.
 * No actual data is put in the newly added attributes buffer, this is done by
 * using the add_attribute, insert_attribute, etc. functions.
 * @param model the model to add the given attribute to.
 * @param the attribute to add to the model.
 */
void tv_model_add_attribute(tv_model* model, tvuint attribute);
/**
 * Inserts the given attribute data into the attribute given by the ID.
 * @param model the model to insert the attribute data into.
 * @param attribute_id the ID of the attribute type e.g. TV_MODEL_COLOR
 * @param vertex the vertex to insert the attribute data at.
 * @param data the data to insert at the given position.
 */
void tv_model_insert_attribute(tv_model *model, tvuint attribute_id, GLfloat *data);
/**
 * Appends  the given attribute data into the attribute given by the ID.
 * @param model the model to append the attribute data into.
 * @param attribute_id the ID of the attribute type e.g. TV_MODEL_COLOR
 * @param data the data to append to the attribute.
 */
void tv_model_append_attribute(tv_model *model, tvuint attribute_id, GLfloat *data);
/**
 * Replaces the given attribute data with the given attribute data.
 * @param model the model to set the attribute data.
 * @param attribute_id the ID of the attribute type e.g. TV_MODEL_COLOR
 * @param vertex the vertex to set the attribute data at.
 * @param data the data to set at the given position.
 */
void tv_model_set_attribute(tv_model *model, tvuint attribute_id, tvuint vertex, GLfloat *data);

/**
 * Buffers the specified attribute data into the given model.
 * @param model the model to buffer the attribute data to.
 * @param attribute the ID of the attribute that is the target of the buffering
 * @param buffer the buffer to copy to the given attribute.
 */
void tv_model_buffer_attribute(tv_model* model, tvuint attribute, TvArray* buffer);

/**
 * Get the number of elements that the specified attribute has per property.
 * @param attribute_id the attribute to retrieve the number of elements per
 *	property (e.g. TV_ATTRIBUTE_VERTEX returns 3: x, y, and Z).
 */
tvuint tv_model_get_attribute_num_elements(tvuint attribute_id);

tvuint tv_model_get_attribute_size(tvuint attribute_id);

TvArray* tv_model_get_attribute(tv_model* model, tvuint attribute);
GLvoid *tv_model_get_attribute_idx(tv_model *model, tvuint index);
tvuint tv_model_get_attribute_size_idx(tv_model *model, tvuint index);

TvAABB tv_model_get_aabb(tv_model* model);
void tv_model_optimize(tv_model* model);
void tv_model_free(tv_model* model);

#ifdef __cplusplus
}
#endif
#endif
