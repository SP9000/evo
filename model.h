#ifndef MODEL_H
#define MODEL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "component.h"

/* If this is uncommented, integer types are normalized and stored as floats */
#define TV_MODEL_STORE_ATTRIBUTES_AS_FLOATS 1

/* the maximum # of properties e.g. position, texco, etc. a model may have. */
#define TV_MODEL_MAX_PROPERTIES 16

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

#define TV_BUFFER_OFFSET(i) ((char *)NULL + (i))

enum {
    MODEL_ATTRIBUTE_NONE,
    MODEL_ATTRIBUTE_VERTEX,
    MODEL_ATTRIBUTE_COLOR,
    MODEL_ATTRIBUTE_NORMAL,
    MODEL_ATTRIBUTE_TEXCO,
	MODEL_ATTRIBUTE_INDEX
};

typedef GLuint tv_model_vbo_handle;

typedef enum tv_model_property_type {
	TV_MODEL_PROPERTY_CHAR,
	TV_MODEL_PROPERTY_UCHAR,
	TV_MODEL_PROPERTY_SHORT,
	TV_MODEL_PROPERTY_USHORT,
	TV_MODEL_PROPERTY_INT,
	TV_MODEL_PROPERTY_UINT,
	TV_MODEL_PROPERTY_FLOAT,
	TV_MODEL_PROPERTY_DOUBLE,
	TV_MODEL_PROPERTY_LIST
}tv_model_property_type;

typedef struct tv_model_property {
	/* the type of the data in this property. */
	tvuint data_type;
	/* the # of values of the data type e.g. 3 for x, y, and z. */
	tvuint count;
	/* this properties' offset from the start of a vertex */
	tvuint offset;
}tv_model_property;

COMPONENT(tv_model, tv_component) 
	GLuint vao;
	GLuint primitive;
	GLuint num_vertices;
	GLuint num_indices;

	/* the per vertex attributes this model uses */
	tv_array *vertices;
	/* the indices for each face in the model. */
	tv_array /*GLshort*/ *indices;
	
	/* the size of each vertex (with all its attributes) for this model. */
	tvuint vertex_size;
	
	/* the ID of the per-vertex attribute VBO. */
	tv_model_vbo_handle vertex_vbo;
	/* the ID of the index VBO. */
	tv_model_vbo_handle index_vbo;

	tvuint num_properties;
	tv_model_property vertex_properties[TV_MODEL_MAX_PROPERTIES];
ENDCOMPONENT(tv_model)

int tv_model_init();

/**
 * Loads a PLY model defined in the file of the given name.
 * @param model the model to load the file into.
 * @param file the file containing the data to load into the model.
 */
void tv_model_load_ply(tv_model *model, tvchar* file);
/**
 * Once all the vertices and indices are set for the given model's data, this
 * function allows the model to be rendered.
 * @param model the model to optimize for renderering.
 */
void tv_model_optimize(tv_model* model);
/**
 * This function should be called after updates are made to a models data and
 * after the model has already been optimized via tv_model_optimize.
 * @param model the model to (re)optimize.
 */
void tv_model_reoptimize(tv_model* model);
/**
 * Get the size of the data (in bytes) that the given property data type uses.
 * @param data_type the type e.g. TV_MODEL_PROPERTY_FLOAT
 * @return the size of the data type requested.
 */
tvuint tv_model_get_property_size(tvuint data_type);
/**
 * Set the format for the vertices in this model.
 * @param model the model to set the vertex format of.
 * @param num_properties the number of properties in the format.
 * @param properties the properties information.
 */
void tv_model_vertex_format(tv_model* model, tvuint num_properties, tv_model_property *properties);
/**
 * Append the given property type to the given model's vertex format.
 * Note that this does not alter any current vertex data, so you'll need to 
 * update the vertices and re-optimize the model before the changes take 
 * effect.
 * @param model the model to append the property type to.
 * @param prop the information about the property to append.
 */
void tv_model_append_property(tv_model* model, tv_model_property *prop);


void tv_model_append_vertex(tv_model *model, GLvoid* data);
void tv_model_set_vertex(tv_model *model, tvuint index, GLvoid *data);
void tv_model_insert_vertex(tv_model *model, tvuint index, GLvoid *data);

void tv_model_set_index(tv_model *model, tvuint index, tvuint new_index);
void tv_model_append_indices1(tv_model* model, tvuint i0);
void tv_model_append_indices2(tv_model* model, tvuint i0, tvuint i1);
void tv_model_append_indices3(tv_model* model, tvuint i0, tvuint i1, tvuint i2);
void tv_model_append_indices4(tv_model* model, tvuint i0, tvuint i1, tvuint i2, tvuint i3);
void tv_model_append_indices(tv_model* model, tvuint count, tvuint* indices);

/*****************************************************************************/
/** 
 * Get a reference to the array of the given model's vertices. 
 * @param model the model to get the vertices of.
 * @return a reference to the array of the model's vertices.
 */
tv_array *tv_model_get_vertices(tv_model *model);
/** 
 * Set the model's vertices to the newly given array.
 * @param model the model to set the vertices of.
 * @param vertices an array of the vertices to set this model's to.
 */
void tv_model_set_vertices(tv_model *model, tv_array *vertices);
/** 
 * Get a reference to the array of the given model's indices. 
 * @param model the model to get the indices of.
 * @return a reference to the array of the model's indices.
 */
tv_array *tv_model_get_indices(tv_model *model);
/** 
 * Set the model's indices to the newly given array.
 * @param model the model to set the indices of.
 * @param vertices an array of the indices to set this model's to.
 */
void tv_model_set_indices(tv_model *model, tv_array *indices);

/**
 * Get the handle to the given model's vertex VBO.
 * @param model the model to retrieve the VBO handle from.
 * @return the handle for this model's vertex VBO.
 */
tv_model_vbo_handle tv_model_get_vertex_handle(tv_model *model);
/**
 * Sets the vertex VBO handle for this model.
 * @param model the model to get set the VBO handle to.
 * @param new_handle the handle to set this model's vertex VBO handle to.
 */
void tv_model_set_vertex_handle(tv_model *model, tvuint new_handle);

/**
 * Get the handle to the given model's index VBO.
 * @param model the model to retrieve the VBO handle from.
 * @return the handle for this model's index VBO.
 */
tv_model_vbo_handle tv_model_get_index_handle(tv_model *model);
/**
 * Sets the index VBO handle for this model.
 * @param model the model to get set the VBO handle to.
 * @param new_handle the handle to set this model's vertex VBO handle to.
 */
void tv_model_set_index_handle(tv_model *model, tvuint new_handle);

TvAABB tv_model_get_aabb(tv_model* model);
void tv_model_free(tv_model* model);

#ifdef __cplusplus
}
#endif
#endif
