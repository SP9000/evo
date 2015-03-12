#ifndef MODEL_H
#define MODEL_H
#ifdef __cplusplus
extern "C" {
#endif


/* TODO: */
/** 
 * There's a lot of butchering of the convention to call attributes a collection
 * of properties.  Often throughout the code attributes are called properties.
 * Change this when you see it happen.
 */

/*****************************************************************************/
/* Model terminology:														 */
/*																			 */
/* Property: a primitive type representing the type of individual elements of*/
/*  a vertex attribute. Integer and Float are properties.				     */
/* Attribute: An attribute is an aggregate of one or more properties.        */
/*  Examples of attributes include Color and Normal.						 */
/* Vertex: A vertex is an aggregate of attributes.  Each attribute represents*/
/*  one element that composes each individual vertex.  A vertex might, for   */
/*  example have a POSITION, a COLOR, and a NORMAL attribute.				 */
/* Model: A model is a high-level structure, but in esscence, it is the		 */
/*  container for a group of vertices.										 */
/*																			 */
/* Notes:																	 */
/*  By convention the first attribute in a vertex should be the position.	 */
/*  This fact is depended on by some useful model functions. Not following   */
/*  this convention may lead to undesirable behavior when working with a	 */
/*  model. Furthermore, the position attribute is assumed to be of the format*/
/*  FLOAT,FLOAT,FLOAT.													     */
/*****************************************************************************/

#include "tv_types.h"
#include "tv_component.h"

/* comment to disable Assimp model loading */
#define TV_MODEL_USE_ASSIMP

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

/*****************************************************************************/
/* Model Vertex Types */
/**
 * The recognized per-vertex attributes a model may use 
 */
typedef enum tv_model_attribute_type {
    MODEL_ATTRIBUTE_NONE,
    MODEL_ATTRIBUTE_VERTEX,
    MODEL_ATTRIBUTE_COLOR,
    MODEL_ATTRIBUTE_NORMAL,
    MODEL_ATTRIBUTE_TEXCO,
	MODEL_ATTRIBUTE_INDEX
}tv_model_attribute_type;

/**
 * The handle used to render OpenGL vertex buffer objects.
 */
typedef GLuint tv_model_vbo_handle;

/**
 * a recognized primitive types of each property (component of a per-vertex 
 * attribute).
 */
typedef enum tv_model_property_type {
	TV_MODEL_PROPERTY_NONE,
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

/**
 * This structure gives information about the format of each individual per
 * vertex attribute.
 */
typedef struct tv_model_attribute {
	/* the type of the data in this property. */
	tv_model_property_type data_type;
	/* the # of values of the data type e.g. 3 for x, y, and z. */
	tvuint count;
	/* this properties' offset from the start of a vertex */
	tvuint offset;
}tv_model_attribute;

/**
 * This structure is used to tell tv_model_vertex_format what format the data
 * for the model will be represented in.
 */
typedef struct tv_model_vertex {
	/* the number of properties per vertex */
	tvuint count;
	/* the type of each per vertex property (TV_MODEL_PROPERTY_FLOAT, etc.) */
	tv_model_property_type types[TV_MODEL_MAX_PROPERTIES];
	/* the number of each corresponding type each property has. */
	tvuint sizes[TV_MODEL_MAX_PROPERTIES];
}tv_model_vertex;

/*****************************************************************************/
COMPONENT(tv_model, tv_component) 
	GLuint vao;
	GLuint primitive;
	GLuint num_vertices;
	GLuint num_indices;

	tv_array *vertices;	/**< the per vertex attributes this model uses */
	tv_array *indices;	/**< the (GLshort) indices for each face in the model. */
	tvuint vertex_size; /**< the size of each vertex (with all its attributes) for this model. */
	
	tv_model_vbo_handle vertex_vbo;	/**< the ID of the per-vertex attribute VBO. */
	tv_model_vbo_handle index_vbo;	/**< the ID of the index VBO. */

	tvuint num_properties;
	tv_model_attribute vertex_attributes[TV_MODEL_MAX_PROPERTIES];
	tv_array* submeshes;	/**< the submeshes of this mesh. */
ENDCOMPONENT(tv_model)

/**
 * Load a model from a file (the file-type is detected).
 * @param filename the name of the file to load the model from.
 */
void METHOD(tv_model, load, tvchar* filename);
/**
 * Loads a PLY model defined in the file of the given name.
 * @param file the file containing the data to load into the model.
 */
void METHOD(tv_model, load_ply, tvchar* file);
/**
 * Once all the vertices and indices are set for the given model's data, this
 * function allows the model to be rendered.
 * @param model the model to optimize for renderering.
 * @param optimize_vertices if true, vertices are optimized.
 * @param optimize_indices if true, indices are optimized.
 */
void METHOD(tv_model, optimize, tvbool optimize_vertices, tvbool optimize_indices);
/**
 * This function should be called after updates are made to a model's data and
 * after the model has already been optimized via tv_model_optimize.
 * @param optimize_vertices if true, vertices are optimized.
 * @param optimize_indices if true, indices are optimized.
 */
void METHOD(tv_model, reoptimize, tvbool optimize_vertices, tvbool optimize_indices);

/**
 * Set the format for the vertices in this model.
 * @param num_properties the number of properties in the format.
 * @param properties the properties information.
 */
void METHOD(tv_model, vertex_format, tvuint num_properties, tv_model_attribute *properties);
/**
 * Append the given property type to a model's vertex format.
 * Note that this does not alter any current vertex data, so you'll need to 
 * update the vertices and re-optimize the model before the changes take 
 * effect.
 * @param prop the information about the property to append.
 */
void METHOD(tv_model, append_property, tv_model_attribute *prop);
/**
 * Append a model to another model.
 * In order for this to work, the two given models must have identical 
 * attributes.
 * @param append the model to append to the given model.
 */
void METHOD(tv_model, append_model, tv_model* append);

/**
 * Add the given mesh to this one.  This is useful for rendering multiple
 * meshes in the same entity.
 */
void METHOD(tv_model, add_submesh, tv_model* mesh);

/** 
 * Append the given vertices to a model.
 * Vertices given must be in the same format as the model's vertices.
 * @param vertices the vertices to append.
 */
void METHOD(tv_model, append_vertices, tv_array* vertices);
void METHOD(tv_model, append_vertex, GLvoid* data);
void METHOD(tv_model, set_vertex, tvuint index, GLvoid *data);
void METHOD(tv_model, insert_vertex, tvuint index, GLvoid *data);

void METHOD(tv_model, set_index, tvuint index, tvuint new_index);
void METHOD(tv_model, append_indices1, tvuint i0);
void METHOD(tv_model, append_indices2, tvuint i0, tvuint i1);
void METHOD(tv_model, append_indices3, tvuint i0, tvuint i1, tvuint i2);
void METHOD(tv_model, append_indices4, tvuint i0, tvuint i1, tvuint i2, tvuint i3);
void METHOD(tv_model, append_indices, tvuint count, tvuint* indices);

tvpointer METHOD(tv_model, get_attribute, tvuint i, tv_model_attribute attribute);

/*****************************************************************************/
/** 
 * Get a reference to the array of a model's vertices. 
 * @return a reference to the array of the model's vertices.
 */
tv_array* METHOD(tv_model, get_vertices);
/** 
 * Set the model's vertices to the newly given array.
 * @param vertices an array of the vertices to set this model's to.
 */
void METHOD(tv_model, set_vertices, tv_array *vertices);
/** 
 * Get a reference to the array of a model's indices. 
 * @return a reference to the array of the model's indices.
 */
tv_array* METHOD(tv_model, get_indices);
/** 
 * Set the model's indices to the newly given array.
 * @param vertices an array of the indices to set this model's to.
 */
void METHOD(tv_model, set_indices, tv_array *indices);
/**
 * Get the handle to the model's vertex VBO.
 * @return the handle for this model's vertex VBO.
 */
tv_model_vbo_handle METHOD(tv_model, get_vertex_handle);
/**
 * Sets the vertex VBO handle for this model.
 * @param new_handle the handle to set this model's vertex VBO handle to.
 */
void METHOD(tv_model, set_vertex_handle, tvuint new_handle);
/**
 * Get the handle to the given model's index VBO.
 * @return the handle for this model's index VBO.
 */
tv_model_vbo_handle METHOD(tv_model, get_index_handle);
/**
 * Sets the index VBO handle for this model.
 * @param new_handle the handle to set this model's vertex VBO handle to.
 */
void METHOD(tv_model, set_index_handle, tvuint new_handle);
/**
 * Apply the given scale to this model's vertices.
 * @param scale the scale to apply to all the vertices.
 */
void METHOD(tv_model, apply_scale, tv_vector3 scale);
/**
 * Get the axis-aligned bounding box for this model.
 * @return the AABB of the model.
 */
TvAABB METHOD(tv_model, get_aabb);
/**
 * Free the resources of this model.
 */
void METHOD(tv_model, free);


/*****************************************************************************/
/* Non-method Functions */
/**
 * Get the size of the data (in bytes) that the given property data type uses.
 * @param data_type the type e.g. TV_MODEL_PROPERTY_FLOAT
 * @return the size of the data type requested.
 */
tvuint tv_model_get_property_size(tvuint data_type);

/*****************************************************************************/
/* Global data */
extern tv_model_vertex TV_MODEL_VERTEX_FORMAT_P;		/* position */
extern tv_model_vertex TV_MODEL_VERTEX_FORMAT_PN;		/* position + normal */
extern tv_model_vertex TV_MODEL_VERTEX_FORMAT_PC;		/* position + color */
extern tv_model_vertex TV_MODEL_VERTEX_FORMAT_PNC;		/* position + normal + color */

#ifdef __cplusplus
}
#endif
#endif
