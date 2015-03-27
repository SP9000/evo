#include "model.h"
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
* Mesh Format:
*   Vertices
*     num_attributes
*     attributes: [pos][color][normal]
*        properties: [fff][ffff][fff]
*   Indices
******************************************************************************/
/*****************************************************************************/
/* includes */
#ifdef TV_MODEL_USE_ASSIMP
#include <assimp\cimport.h>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#endif

/*****************************************************************************/
/* global data */
/******************************************************************************
* vertex formats 
* These are common formats that may be used with tv_model_vertex_format to 
* easily produce models with these formats programmatically. 
******************************************************************************/
tv_model_vertex TV_MODEL_VERTEX_FORMAT_P = {1, {TV_MODEL_PROPERTY_FLOAT}, {3}};
tv_model_vertex TV_MODEL_VERTEX_FORMAT_PN = {2, {TV_MODEL_PROPERTY_FLOAT, TV_MODEL_PROPERTY_FLOAT}, {3, 3}};
tv_model_vertex TV_MODEL_VERTEX_FORMAT_PC = {2, {TV_MODEL_PROPERTY_FLOAT, TV_MODEL_PROPERTY_FLOAT}, {3, 4}};
tv_model_vertex TV_MODEL_VERTEX_FORMAT_PNC = {3, {TV_MODEL_PROPERTY_FLOAT, TV_MODEL_PROPERTY_FLOAT, TV_MODEL_PROPERTY_FLOAT}, {3, 3, 4}};

/*****************************************************************************/
/* local types */
/******************************************************************************
* loaded_model
* A structure for holding the names and references of loaded models 
******************************************************************************/
typedef struct loaded_model {
	tv_model *model;  /**< a reference to an instance of the loaded model */
	tvchar name[32];  /**< the name of the model */
	TvHashHandle hh;  /**< a hash handle for making tables with this struct */
} loaded_model;

/*****************************************************************************/
/* .PLY loading helper types */
/******************************************************************************
* property_group_type
* The types of common groups of properties.  These directly correspond to 
* model attributes.
******************************************************************************/
typedef enum {
	GROUP_POSITION,
	GROUP_NORMAL,
	GROUP_COLOR,
	GROUP_TEXCO,
	GROUP_NONE
}property_group_type;
/******************************************************************************
* ply_property
* A .PLY property defines one component of a .PLY element.
* In a .PLY property a property defines two things- its name, and its type.
* The type (ushort, float, etc.) is used to determine how to store and 
* handle the data.  The name is general but some special names may be 
* recognized by the engine.
*******************************************************************************/
typedef struct ply_property {
	tv_model_property_type type;  /**< the type of the property e.g. "float" */
	property_group_type group;    /**< the group this property's belongs to */
	tvuint size;                  /**< the size of the property in bytes */
	tvuint type_list[32];   /**< if TV_MODEL_PROPERTY_LIST, the list of types */
	tvchar name[32];        /**< the name of the property */
}ply_property;
static UT_icd ply_property_icd = {sizeof(ply_property), 0, 0, 0};
/*******************************************************************************
* ply_element
* A .PLY element defines a per-vertex attribute composed of a number of PLY 
* properties.
* Some elements are required the engine.  One such example is "vertex", 
* required by all meshes, which has 3 float properties.  
* The elements that a mesh has determine which material interfaces may be used
* to render the mesh.  In general you should try to produce models renderable
* with the standard material interface.  
*******************************************************************************/
typedef struct ply_element {
	tvuint count;         /**< the number of this element in the file */
	tv_array *properties;  /**< array of properties associated with this element */
	tvchar name[32];      /**< the name of the element */
}ply_element;
static UT_icd ply_element_icd = {sizeof(ply_element), 0, 0, 0};
/******************************************************************************
* ply
* A structure representing the .ply file to be parsed into a mesh.
******************************************************************************/
typedef struct {
	FILE* fp;			/**< a file pointer for the PLY file */
	tvchar* line_buffer;  /**< a buffer containing the contents of the current line */
}ply_file;
/******************************************************************************
* property_group
* A structure that contains information about a property group 
******************************************************************************/
typedef struct property_group {
	property_group_type type;
	tvuint data_type;
	tvuint count;
}property_group;
/*****************************************************************************/
/* .PLY loading helper functions */
/* helper functions to check if the property is one of several known types. */
static tvbool property_is_position(tvchar *name);
static tvbool property_is_normal(tvchar *name);
static tvbool property_is_color(tvchar *name);
static tvbool property_is_texco(tvchar *name);

/*****************************************************************************/
/* a table of models that have already been loaded. */
static loaded_model* loaded_models = NULL;
/* The ICD for a model's indices */
UT_icd ut_index_icd = {sizeof(GLshort), 0, 0, 0};

/*****************************************************************************/
/* Helper functions */
/******************************************************************************
* ply_open 
* opens the given PLY file.
******************************************************************************/
tvbool ply_open(ply_file* ply, const tvchar* filename)
{
	/* open the file and verify it is a .ply file */
	ply->fp = fopen(filename, "r");
	if(ply->fp == NULL) {
		tv_warning("could not open %s\n", filename);
		return FALSE;;
	}
	/* allocate a buffer for the contents of 1 line */
	ply->line_buffer = (tvchar*)tv_alloc(sizeof(tvchar) * 4096);
	return TRUE;
}
/******************************************************************************
* ply_close
* Closes the given PLY file and frees all resources associated with it.
******************************************************************************/
void ply_close(ply_file* ply)
{
	if(ply) {
		if(ply->line_buffer) {
			free(ply->line_buffer);
		}
	}
}
/******************************************************************************
* ply_next
* Move the ply file to the next line.  If the end of the file is found 
* instead of a newline, the PLY file's resources are freed and FALSE will be
* returned.  Upon successful relocation to a newline, TRUE is returned.
******************************************************************************/
tvbool ply_next(ply_file* ply) 
{
	if(fgets(ply->line_buffer, 4096, ply->fp) == NULL) {
		return FALSE;
	}
	return TRUE;
}
/******************************************************************************
* ply_curr
* Returns the current line of text from the PLY file.
******************************************************************************/
tvchar* ply_curr(ply_file* ply) 
{
	return ply->line_buffer;
}
/******************************************************************************
* read_comment
* Reads the given line of text and returns TRUE if it is a comment, FALSE if
* it is not
******************************************************************************/
tvbool read_comment(tvchar *line)
{
	tvchar a[256];
	tvuint num_read = sscanf(line, "%s", a);

	/* check if there's nothing on this line, sure, we'll call that a comment */
	if(num_read = 0) {
		return 1;
	}
	/* is this line a comment? */
	if(strncmp(a, "comment", 8) == 0) {
		return 1;
	}
	return 0;
}
/******************************************************************************
* read_element
* An element is defined as by the .PLY model format standard to be a group of
* related properties.  A vertex is an element with 3 properties representing
* the 3D coordinates of the vertex in space.
******************************************************************************/
ply_element* ply_read_element(tvchar* element)
{
	/* 3 buffers: "element", the element name, and the # of properties */
	tvchar a[256], b[256], c[256];
	tvuint num_read = sscanf(element, "%s %s %s", a, b, c);

	/* make sure the line we're on is an element (it should be if this function
	* is called) */
	if(strncmp(a, "element", sizeof("element")-1) == 0) {
		ply_element* el;
		/* check that the element matches the expected formatting */
		if(num_read != 3) {
			tv_warning("element %s has unrecognized formatting in declaration.\n", a);
			return NULL;
		}
		/* The only elements recognized are "vertex" and "face" for now. */
		if((strncmp(b, "vertex", 6) != 0) && (strncmp(b, "face", 4) != 0)) {
			tv_warning("unrecognized element %s\n", a);
			return NULL;
		}
		/* create and fill the element to add to the element table */
		el = (ply_element*)tv_alloc(sizeof(ply_element));
		el->count = atoi(c);
		utarray_new(el->properties, &ply_property_icd);
		/* copy the element's name to the name buffer */
		strncpy(el->name, b, 32);
		return el;
	}
	/* no element could be read */
	return NULL;
}
/******************************************************************************
* ply_property_type
* Gets the size (in bytes) and the property type (float, uchar, etc.) of
* the given property name.  Returns FALSE if name doesn't match a known type
* of property else TRUE.
******************************************************************************/
tvbool ply_get_property_type(tvchar* name, tvuint /*out*/ *size, tv_model_property_type /*out*/ *type)
{
	*size = 0;
	/* get the property type and use it to get the size of the property */
	if(strncmp(name, "char", 4) == 0) {
		*size = 1;
		*type = TV_MODEL_PROPERTY_CHAR;
	} else if(strncmp(name, "uchar", 5) == 0) {
		*size = 1;
		*type = TV_MODEL_PROPERTY_UCHAR;
	} else if(strncmp(name, "short", 5) == 0) {
		*size = 2;
		*type = TV_MODEL_PROPERTY_SHORT;
	} else if(strncmp(name, "ushort", 6) == 0) {
		*size = 2;
		*type = TV_MODEL_PROPERTY_USHORT;
	} else if(strncmp(name, "int", 3) == 0) {
		*size = 4;
		*type = TV_MODEL_PROPERTY_INT;
	} else if(strncmp(name, "uint", 4) == 0) {
		*size = 4;
		*type = TV_MODEL_PROPERTY_UINT;
	} else if(strncmp(name, "float", 5) == 0) {
		*size = 4;
		*type = TV_MODEL_PROPERTY_FLOAT;
	} else if(strncmp(name, "double", 6) == 0) {
		*size = 8;
		*type = TV_MODEL_PROPERTY_DOUBLE;
	}
	if((*size) > 0) {
		return TRUE;
	}
	return FALSE;
}
/******************************************************************************
* ply_get_property_group
* Gets the group that the property belongs to (e.g. POSITION) if any. Returns
* TRUE if the property belongs to a known group, else FALSE.
******************************************************************************/
tvbool ply_get_property_group(tvchar* name, property_group_type* group)
{
	*group = GROUP_NONE;
	if(property_is_position(name)) {  /* position? */
		*group = GROUP_POSITION;
	} else if(property_is_normal(name)) { /* normal ? */
		*group = GROUP_NORMAL;
	} else if(property_is_color(name)) {  /* color? */
		*group = GROUP_COLOR;
	} else if(property_is_texco(name)) {  /* texco? */
		*group = GROUP_TEXCO;
	}
	if(*group == GROUP_NONE) {
		return FALSE;
	}
	return TRUE;
}
/******************************************************************************
* ply_read_property
* A property is defined by the .PLY model standard.  A properties defines the 
* type and size of the storage used to represent some attribute (e.g. an 
* x-coordinate is a FLOAT property)
******************************************************************************/
ply_property* ply_read_property(tvchar *line)
{
	/* 3 buffers: "property", property type, property name */
	tvchar a[256], b[256], c[256];
	ply_property *p = (ply_property*)tv_alloc(sizeof(ply_property));

	/* read the property */
	tvuint num_read = sscanf(line, "%s %s %s", a, b, c);

	/* check that the line is a property (should be) */
	if(strncmp("property", a, 8) != 0) {
		free(p);
		return NULL;
	}
	/* get the type of the property and verify it is a recognized type */
	if(ply_get_property_type(b, &p->size, &p->type) == FALSE) {
		tv_warning("property %s has unrecognized type", p->name, b);
		free(p);
		return NULL;
	}
	/* get the group the property belongs to and verify it is recognized */
	if(ply_get_property_group(c, &p->group) == FALSE) {
		tv_warning("property %s does not belong to any know group", p->name);
		free(p);
		return NULL;
	}
	/* copy the name of the property to the property structure */
	strncpy(p->name, c, 32);
	return p;
}
/******************************************************************************
* ply_read_elements
* Reads all the elements (and their properties) from the given ply file and
* returns the array of elements that the file contained.
* The ply_file points to the vertex lists after this call returns.  This 
* fucntion also returns the number of vertices and indices specified according
* to the vertex and face elements in the file.  These two particular elements
* are important as they are integral to the functionality of the mesh.  They
* also happen to generally be the ONLY elements in a model.
******************************************************************************/
tv_array* ply_read_elements(ply_file* ply, tvuint* /*out*/ num_vertices, tvuint* /*out*/ num_faces)
{
	tv_array* elements;
	ply_element* el = NULL;
	ply_property* prop = NULL;
	tvbool header_found = FALSE;
	tvbool ignore_properties = FALSE;	/* in the case of, faces for example, we don't care about properties */

	utarray_new(elements, &ply_element_icd);


	while(!header_found) {
		tvchar* curr = ply_curr(ply);
		/* check if the end of the header has been reached */
		if(strncmp(curr, "end_header", sizeof("end_header")-1) == 0) {
			header_found = TRUE;
			break;
		} else if(strncmp(curr, "comment", sizeof("comment")-1) == 0) {
			/* comment- do nothing */
		} else if(strncmp(curr, "element", sizeof("element")-1) == 0) {
			/* read the element, until another is read all properties apply to
			 * this element */
			el = ply_read_element(curr);
			if(el == NULL) {
				tv_warning("failed to read element");
				continue;
			}
			utarray_push_back(elements, el);
			/* if we just read the vertex or face element, get the # of vertices/faces */
			if(strncmp(el->name, "vertex", sizeof("vertex")-1) == 0) {
				*num_vertices = el->count;
				ignore_properties = FALSE;
			} else if(strncmp(el->name, "face", sizeof("face")-1) == 0) {
				*num_faces = el->count;
				ignore_properties = TRUE;
			}
		} else if(strncmp(curr, "property", sizeof("property")-1) == 0) {
			/* read 1 property (append it to the current element if found) */
			if(!ignore_properties) {
				prop = ply_read_property(curr);
				if(prop != NULL) {
					utarray_push_back(el->properties, prop);
				}
			}
		}
		ply_next(ply);
	}
	return elements;
}
/******************************************************************************
* property_is_color
* A convienience functon for checking if the name of a property appears to 
* represent a per-vertex color attribute.  This allows properties like "r",
* "g", and b to work as well as "red", "green", and "blue". 
* Since properties are not grouped into a "color" section in the PLY foramt,
* but rather given as single r, g, b, and (sometimes) alpha components, this
* recognition allows them to be identified as related and stored as a vector4.
******************************************************************************/
tvbool property_is_color(tvchar *name) 
{
	/* does the given name match any recognized color name pattern? */
	if(strncmp(name, "red", 3) == 0 ||
		strncmp(name, "blue", 4) == 0 ||
		strncmp(name, "green", 5) == 0 ||
		strncmp(name, "alpha", 5) == 0) {
			return TRUE;
	}
	return FALSE;
}
/******************************************************************************
* property_is_position
* This convienience function recognizes common representations of vertex 
* positions in order to allow more flexible matching with material interfaces.
******************************************************************************/
tvbool property_is_position(tvchar *name) 
{
	/* does the given name match any recognized position name pattern? */
	if(strncmp(name, "x", 1) == 0 ||
		strncmp(name, "y", 1) == 0 ||
		strncmp(name, "z", 1) == 0) {
			return TRUE;
	}
	return FALSE;
}
/******************************************************************************
* property_is_normal
* A convienience function for recognizing vertex normals.
******************************************************************************/
tvbool property_is_normal(tvchar *name) 
{
	/* does the given name match any recognized normal name pattern? */
	if(strncmp(name, "nx", 2) == 0 ||
		strncmp(name, "ny", 2) == 0 ||
		strncmp(name, "nz", 2) == 0) {
			return TRUE;
	}
	return FALSE;
}
/******************************************************************************
* property_is_normal
* A convienience function for recognizing vertex texture coordinates.
******************************************************************************/
tvbool property_is_texco(tvchar *name) 
{
	/* does the given name match any recognized texco name pattern? */
	if(strncmp(name, "u", 1) == 0 ||
		strncmp(name, "v", 1) == 0) {
			return TRUE;
	}
	return FALSE;
}
/*******************************************************************************
* tv_model_find
* Searches the table of loaded models for a model matching the given name.
*******************************************************************************/
tv_model* tv_model_find(tvchar* name) 
{
	loaded_model *lup;
	/* if the model table isn't empty check if the model has already been 
	loaded */
	if(loaded_models != NULL) {
		HASH_FIND_STR(loaded_models, name, lup);
		if(lup) {
			/*TODO: (deep) copy model from lup..I guess that'd be preferable? */
			return lup->model;
		}
	}
	return NULL;
}
/*******************************************************************************
* tv_model_save
* Saves the given model in the internal model hash table for future loading 
* without having to reread the model file.
*******************************************************************************/
void tv_model_save(tvchar* name, tv_model* model)
{
	loaded_model* lup = (loaded_model*)tv_alloc(sizeof(loaded_model));
	/* add the model to the hash table of loaded models for future loading */
	strncpy(lup->name, name, 32);
	lup->model = model;
	HASH_ADD_STR(loaded_models, name, lup);

}
/*******************************************************************************
* tv_model_get_properties
* Given an element, finds and groups all the properties associated with that 
* element into a property that can be used in the optimized mesh. 
* For example: x, y, and z get grouped int a GROUP_POSITION where they are 
* collectively treated as a 3D vector representing position.
*******************************************************************************/
void ply_get_vertex_properties(tv_model* model, tv_array* elements)
{
	ply_property* p;
	ply_element* el;
	tvint prev_group = -1;  /* the group of the last handled property */
	tvuint cur_found = 0;	/* the number of a given related property found */

	/* find the vertex element */
	for(el = (ply_element*)utarray_front(elements); el != NULL; 
		el = (ply_element*)utarray_next(elements, el)) {
		if(strncmp("vertex", el->name, sizeof("vertex")-1) == 0) {
			/* found the vertex element */
			break;
		}
	}
	/* if no vertex property was found, quit */
	if(el == NULL) {
		return;
	}
	model->num_vertices = el->count;
	/* iterate over each vertex property */
	for(p = (ply_property*)utarray_front(el->properties); p != NULL; 
		p = (ply_property*)utarray_next(el->properties, p)) {
		tvuint num_to_complete = 0;        /* # of properties needed for the current group */
		tvint cur_group = p->group; 

		/* get the # of related properties needed to complete the current group */
		switch(cur_group) {
		case GROUP_POSITION: num_to_complete = 2; break;
		case GROUP_NORMAL: num_to_complete = 2; break;
		case GROUP_COLOR: num_to_complete = 2; break;
		case GROUP_TEXCO: num_to_complete = 1; break;
		default: tv_warning("unrecognized group"); break;
		}
		if(cur_group == prev_group) {
			cur_found++;
		}
		else {
			cur_found = 1;
		}
		/* if the # of properties needed to complete the group was reached, 
		* add the property to the model. */
		if(cur_found == num_to_complete) {
			/* build an attribute using the values from the property group */
			tv_model_attribute attr;
			attr.data_type = p->type;
			attr.count	   = num_to_complete + 1;
			tv_model_append_property(model, &attr);
		}
		prev_group = cur_group;
		/* clean up - the property has been handled, we're done with it. */
		/* TODO: */
		//free(p);
	}
}
/******************************************************************************
* tv_model_get_attributes
* Using the given elements, get the per-vertex attributes for the given model.
******************************************************************************/
ply_element* tv_model_get_attributes(tv_model* model, tv_array* elements)
{
	ply_element** el;
	for(el = (ply_element**)utarray_front(elements); el != NULL; 
		el = (ply_element**)utarray_next(elements, el)) {
		//ply_get_properties(model, *el);
	}
	return NULL;
}
/******************************************************************************
* ply_get_vertices
* Given a ply file at the vertex list block of the file, read all the vertices
* into the given model.
******************************************************************************/
void ply_get_vertices(ply_file* ply, tv_model* model)
{
	tvuint i, j, k;
	/* allocate a vertex of the size of our vertices */
	tvpointer my_vertex  =  (tvpointer)tv_alloc(model->vertex_size);
	UT_icd my_vertex_icd = {0, NULL, NULL, NULL};

	/* create an array ICD for the vertices we will be adding */
	my_vertex_icd.sz = model->vertex_size;
	utarray_new(model->vertices, &my_vertex_icd);

	/* read all the vertices from the ply file */
	for(i = 0; i < model->num_vertices; ++i) {
		for(j = 0; j < model->num_properties; ++j) {
			for(k = 0; k < model->vertex_attributes[j].count; ++k) {
				tvchar* line_buffer = ply->line_buffer;

				/* the address to write the data we read to.
				* (base + property offset + property component offst).
				* e.g. vertex_base + offset(position) + offset(position.x) */
				tvbyte* property_loc = (tvbyte*)my_vertex + 
					model->vertex_attributes[j].offset + 
					tv_model_get_property_size(model->vertex_attributes[j].data_type)*k;

				/* read a value from the PLY file */
				fscanf(ply->fp, "%s", line_buffer);
				switch(model->vertex_attributes[j].data_type) {
/* if attributes are to be stored as floats, normalize them according to the 
 * maximum value of their integer based storage */
#ifdef TV_MODEL_STORE_ATTRIBUTES_AS_FLOATS
				case TV_MODEL_PROPERTY_CHAR:
				case TV_MODEL_PROPERTY_UCHAR:
					*(tvfloat*)property_loc = ((tvfloat)atof(line_buffer)) / 255.0f;
					break;
				case TV_MODEL_PROPERTY_SHORT:
				case TV_MODEL_PROPERTY_USHORT:
					*(tvfloat*)property_loc = ((tvfloat)atof(line_buffer)) / (tvfloat)0xffff;
					break;
				case TV_MODEL_PROPERTY_INT:
				case TV_MODEL_PROPERTY_UINT:
					*(tvfloat*)property_loc = ((tvfloat)atof(line_buffer)) / (tvfloat)0xffffffff;
					break;
/* if attributes are not to be stored as floats, copy them as is */
#else
				case TV_MODEL_PROPERTY_CHAR:
				case TV_MODEL_PROPERTY_UCHAR:
					*(((tvbyte*)(my_vertex)) + offset) = (tvbyte)atoi(line_buffer);
					break;
				case TV_MODEL_PROPERTY_SHORT:
				case TV_MODEL_PROPERTY_USHORT:
					*(((tvbyte*)(my_vertex)) + offset) = (tvword)atoi(line_buffer);
					break;
				case TV_MODEL_PROPERTY_INT:
				case TV_MODEL_PROPERTY_UINT:
					*(((tvbyte*)(my_vertex)) + offset) = (tvdword)atoi(line_buffer);
					break;
#endif
/* in either case, floats and doubles are stored in a 4-byte float format */
				case TV_MODEL_PROPERTY_FLOAT:
					*(tvfloat*)property_loc = (tvfloat)atof(line_buffer);
					break;
				case TV_MODEL_PROPERTY_DOUBLE:
					*(tvfloat*)property_loc = (tvfloat)atof(line_buffer);
					break;
				}
			}
		}
		utarray_push_back(model->vertices, my_vertex);
	}
}
/******************************************************************************
 * ply_get_indices
 * Reads all the indices from the file.  The given number of faces should 
 * match the number of faces that get defined in the file.
******************************************************************************/
void ply_get_indices(ply_file* ply, tv_model* model, tvuint num_faces)
{
	tvuint i; 
	for(i = 0; i < num_faces; ++i) {
		tvuint face_buff[4];
		tvint face_size = 0;

		/* read the number of vertices in this face */
		fscanf(ply->fp, "%d", &face_size);
		if(feof(ply->fp)) {
			return;
		}
		/* if the face is a quad, get the indices of the decomposed triangles */
		if(face_size == 4) {
			fscanf(ply->fp, "%d %d %d %d", &face_buff[0], &face_buff[1], &face_buff[2], &face_buff[3]);
			utarray_push_back(model->indices, &face_buff[0]);
			utarray_push_back(model->indices, &face_buff[1]);
			utarray_push_back(model->indices, &face_buff[2]);
			utarray_push_back(model->indices, &face_buff[2]);
			utarray_push_back(model->indices, &face_buff[3]);
			utarray_push_back(model->indices, &face_buff[0]);
		}
		/* if the face is a triangle, copy the indices */
		else if(face_size == 3) {
			fscanf(ply->fp, "%d %d %d", &face_buff[0], &face_buff[1], &face_buff[2]);
			utarray_push_back(model->indices, &face_buff[0]);
			utarray_push_back(model->indices, &face_buff[1]);
			utarray_push_back(model->indices, &face_buff[2]);
		}
		/* no other vertex count currently supported */
		else {
			tv_warning("unrecognized number of vertices per face %d\n", face_size);
		}
	}
	model->num_indices = utarray_len(model->indices);
}
/******************************************************************************
* tv_model_load
* The implementation of this function may either use Assimp to load a model 
* from one of many auto-detected filetypes or use the built-in loader for .ply
* files.
*****************************************************************************/
void tv_model_load(tv_model* model, tvchar* filename)
{
#ifdef TV_MODEL_USE_ASSIMP
	const struct aiScene* asset; // = aiImportFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);
	tvuint i, j;

	if(!asset) {
		tv_error("failed to load model %s\n", filename);
		model = NULL;
		return;
	}
	if(asset->mNumMeshes <= 0) {
		tv_warning("no meshes detected in model %s\n", filename);
	}
	if(asset->mNumMeshes > 1) {
		tv_info("multiple meshes in model %s will be combined.\n", filename);
	}
	for(i = 0; i < asset->mMeshes; ++i) {
		tvfloat v[5];	/* TODO: generalize - must be size of max # of vertex attributes */
		struct aiMesh* m = asset->mMeshes[i];
		tvuint vertex_size = 0;
		tvuint vertex_num_properties = 1;
		tv_model_attribute attr;
		tvuint num_faces;

		attr.data_type = TV_MODEL_PROPERTY_FLOAT;

		/* position - all models contain position data */
		attr.count = 3;
		vertex_size += attr.count;
		tv_model_append_property(model, &attr);

		/* normals - TODO: */
		if(1) {
			attr.count = 3;
			vertex_size += attr.count;
			tv_model_append_property(model, &attr);
			++vertex_num_properties;
		}
		/* color - TODO: not sure how to get # color channels with C API yet. */
		if(1) {
			attr.count = 4;
			vertex_size += attr.count;
			tv_model_append_property(model, &attr);
			++vertex_num_properties;
		}
		/* texture coordinates - TODO: check if UV's exist */
		if(0) {
			attr.count = 2;
			vertex_size += attr.count;
			tv_model_append_property(model, &attr);
			++vertex_num_properties;
		}
		/* TODO: assumes triangulation */
		num_faces = 3 * m->mNumVertices;

		/* set vertex format TODO: for all meshes being combined - make sure it's the same */
		tv_model_vertex_format(model, vertex_num_properties, &attr);
		/* copy all the vertices and vertex attributes to our model format */
		for(j = 0; j < m->mNumVertices; ++j) {
			tvuint offset = 0;
			/* position - all models contain position data */
			v[offset++] = m->mVertices[i].x;
			v[offset++] = m->mVertices[i].y;
			v[offset++] = m->mVertices[i].z;

			tv_model_append_property(model, &attr);
			/* normals - TODO: */
			if(1) {
				v[offset++] = m->mNormals[i].x;
				v[offset++] = m->mNormals[i].y;
				v[offset++] = m->mNormals[i].z;
			}
			/* color - TODO: not sure how to get # color channels with C API yet. */
			if(1) {
				v[offset++] = m->mColors[i]->r;
				v[offset++] = m->mColors[i]->g;
				v[offset++] = m->mColors[i]->b;
				v[offset++] = m->mColors[i]->a;
			}
			/* texture coordinates - TODO: check if UV's exist */
			if(0) {
				v[offset++] = m->mTextureCoords[i]->x;
				v[offset++] = m->mTextureCoords[i]->y;
			}
			tv_model_append_vertex(model, &v);
		}
	}
	model->primitive = GL_TRIANGLES;
#else
	tv_warning("Assimp not enabled - model will not be loaded.");
#endif
}

/******************************************************************************
* tv_model_load_ply
* This is the dependency free model loader for .ply files.  This is roughly
* conformant with the .PLY specification.  Specifically it should work with
* models exported by blender.
* TODO: refactor (break into smaller pieces)
*****************************************************************************/
void tv_model_load_ply(tv_model *model, tvchar* file)
{
	ply_file ply;
	tv_model* lup;
	tv_array* elements;
	tvuint num_vertices;  /* the number of vertices in the mesh */
	tvuint num_faces;	  /* the number of faces in the mesh */

	/* load the model from the model table if it is already loaded */
	lup = tv_model_find(file);
	if(lup) {
		model = lup;
		return;
	}
	/* if caller gave no model reference, create a new model */
	if(model == NULL) {
		model = tv_model_new();
	}
	/* open the ply file */
	if(!ply_open(&ply, file)) {
		return;
	}
	/* read the elements from the file */
	elements = ply_read_elements(&ply, &num_vertices, &num_faces);
	model->num_vertices = num_vertices;

	/* extract all the data we collected about the elements and properties of this
	* model. */
	model->num_properties = 0;
	model->vertex_size = 0;

	/* get the properties, vertices, and indices for the model */
	ply_get_vertex_properties(model, elements);
	ply_get_vertices(&ply, model);
	ply_get_indices(&ply, model, num_faces);
	/* the only rendering primitive is triangles, other types are decomposed */
	model->primitive = GL_TRIANGLES;

	/* store the model for future lookup */
	tv_model_save(file, model);

	/* clean up */
	ply_close(&ply);
	utarray_free(elements);
}

/******************************************************************************
* tv_model_optimize
* Optimize the mesh (generate an OpenGL VAO for the attributes and buffer
* them.  This function calls tv_model_reoptimize to complete this task.
******************************************************************************/
void tv_model_optimize(tv_model* model, tvbool optimize_vertices, tvbool optimize_indices)
{
	/* create a VAO for the model */
	glGenVertexArrays(1, &model->vao);

	/* generate the per-vertex data buffer */
	glGenBuffers(1, &model->vertex_vbo);

	/* check if there are any indices in the model, if so make index buffer */
	if(optimize_indices && utarray_len(model->indices) > 0) {
		/* generate the index buffer */
		glGenBuffers(1, &model->index_vbo);
	}
	else {
		model->index_vbo = 0;
	}
	tv_model_reoptimize(model, optimize_vertices, optimize_indices);
}
/******************************************************************************
* tv_model_reoptimize
* Anytime a change to a mesh is made, the per-vertex attributes must be 
* reuploaded to OpenGL for changes to take effect.  This function generates
* buffers for the per-vertex attributes of a mesh and buffers them.
******************************************************************************/
void tv_model_reoptimize(tv_model* model, tvbool optimize_vertices, tvbool optimize_indices)
{
	tvuint i, j;
	glBindVertexArray(model->vao);

	/* optimize vertices */
	glBindBuffer(GL_ARRAY_BUFFER, model->vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, 
		model->vertex_size * utarray_len(model->vertices),
		(GLfloat*)utarray_front(model->vertices),
		GL_STATIC_DRAW);

	/* optimize indices */
	if(optimize_indices) {
		if(utarray_len(model->indices) > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->index_vbo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,	
				utarray_len(model->indices) * sizeof(GLshort),
				(GLshort*)utarray_front(model->indices),
				GL_STATIC_DRAW);
		}
	}
	/* set the attribute pointers for each per-vertex property */
	for(i = 0, j = 0; i < model->num_properties; ++i) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer((GLuint)i, model->vertex_attributes[i].count, GL_FLOAT, GL_FALSE, 
			model->vertex_size,
			(GLvoid*)j); /* TODO: use model->vertex_properties[i].offset? Not sure if I want to keep offset at all  */
		j += model->vertex_attributes[i].count * tv_model_get_property_size(model->vertex_attributes[i].data_type);
	}
	/* Unbind. */
	glBindVertexArray(0);
}

/******************************************************************************
* tv_model_vertex_format
* Tell OpenGL the format of the data for the mesh to allow proper 
* optimization of said mesh.
******************************************************************************/
void tv_model_vertex_format(tv_model* model, tvuint num_properties, tv_model_attribute *properties)
{
	tvuint i;
	tvuint offset;
	UT_icd my_vertex_icd = {0, 0, 0, 0};

	/* clear the existing properties. */
	model->num_properties = 0;
	/* append all the properties */
	for(i = 0, offset = 0; i < num_properties; ++i) {
		tv_model_append_property(model, &properties[i]);
	}
	/* if there is an existing vertex array, get rid of it. */
	if(model->vertices != NULL) {
		utarray_free(model->vertices);
	}
	/* make an ICD for the new vertex size and allocate a new vertex array */
	my_vertex_icd.sz = model->vertex_size;
	utarray_new(model->vertices, &my_vertex_icd);
}
/******************************************************************************
* tv_model_append_property
* Copies the property that is given to the next available spot in the model's
* attribute array.
******************************************************************************/
void tv_model_append_property(tv_model* model, tv_model_attribute *prop)
{
	model->vertex_attributes[model->num_properties].data_type = prop->data_type;
	model->vertex_attributes[model->num_properties].count = prop->count;
	model->vertex_attributes[model->num_properties].offset = model->vertex_size;

	model->vertex_size += tv_model_get_property_size(prop->data_type) * prop->count;
	model->num_properties++;
}
/*****************************************************************************/
void tv_model_set_vertex(tv_model *model, tvuint index, GLvoid *data)
{
	/* TODO */
}
void tv_model_set_index(tv_model *model, tvuint index, tvuint new_index)
{
	*utarray_eltptr(model->indices, index) = new_index;
}

void tv_model_append_vertex(tv_model *model, GLvoid* data)
{
	utarray_push_back(model->vertices, data);
	model->num_vertices++;
}
void tv_model_insert_vertex(tv_model *model, tvuint index, GLvoid *data)
{
	utarray_insert(model->vertices, data, index);
}
void tv_model_append_indices1(tv_model* model, tvuint i)
{
	utarray_push_back(model->indices, &i);
	model->num_indices++;
}
void tv_model_append_indices2(tv_model* model, tvuint i0, tvuint i1)
{
	utarray_push_back(model->indices, &i0);
	utarray_push_back(model->indices, &i1);
	model->num_indices += 2;
}
void tv_model_append_indices3(tv_model* model, tvuint i0, tvuint i1, tvuint i2)
{
	utarray_push_back(model->indices, &i0);
	utarray_push_back(model->indices, &i1);
	utarray_push_back(model->indices, &i2);
	model->num_indices += 3;
}
void tv_model_append_indices4(tv_model* model, tvuint i0, tvuint i1, tvuint i2, tvuint i3)
{
	utarray_push_back(model->indices, &i0);
	utarray_push_back(model->indices, &i1);
	utarray_push_back(model->indices, &i2);
	utarray_push_back(model->indices, &i3);
	model->num_indices += 4;
}
void tv_model_append_indices(tv_model* model, tvuint count, tvuint* indices)
{
	tvuint i;
	for(i = 0; i < count; ++i) {
		utarray_push_back(model->indices, &indices[i]);
	}
	model->num_indices += count;
}

/*******************************************************************************
* tv_model_append_vertices
* Appends the vertices from a model to the this model.  Remember that vertices
* include both the position coordinates as well as all attributes (color, etc.)
*******************************************************************************/
void tv_model_append_vertices(tv_model* model, tv_array* vertices)
{
	GLvoid *v;
	for(v = (GLvoid*)utarray_front(vertices); v != NULL; v = (GLvoid*)utarray_next(vertices, v)) {
		tv_model_append_vertex(model, v);
	}
}

/******************************************************************************
* do_attributes_match
* Checks if the two models match (have all the same per-vertex attributes )
******************************************************************************/
tvbool METHOD(tv_model, attributes_match, tv_model* other)
{
	tvuint i;
	if(self->num_properties != other->num_properties) {
		/* different # of properties */
		return FALSE;
	}
	/* check each vertex attribute */
	for(i = 0; i < self->num_properties; ++i) {
		if(!((self->vertex_attributes[i].count == other->vertex_attributes[i].count) &&
			(self->vertex_attributes[i].data_type == other->vertex_attributes[i].data_type) &&
			(self->vertex_attributes[i].offset == other->vertex_attributes[i].offset))) {
				/* different attribute types */
				return FALSE;
		}
	}
	/* the two models have identical attributes */
	return TRUE;
}
/******************************************************************************
* tv_model_append_model
* Concatenates one mesh to another.  This function verifies that the models
* have matching per-vertex attributes and then adds each vertex from the 
* "append" mesh to the self mesh.
******************************************************************************/
void METHOD(tv_model, append_model, tv_model* append)
{
	GLshort* index;
	GLshort offset;

	/* do not append NULL models (or append TO NULL models) */
	if(!self || !append) {
		return;
	}
	/* verify that the two models have matching attributes */
	if(!tv_model_attributes_match(self, append)) {
		return;
	}
	/* models are compatible for appending- get the index offset */
	offset = (GLshort)utarray_len(self->vertices);
	/* append vertices */
	tv_model_append_vertices(self, append->vertices);
	/* append indices - we must add an offset to each index before appending */
	for(index = (GLshort*)utarray_front(append->indices); index != NULL; 
		index = (GLshort*)utarray_next(append->indices, index)) {
			tv_model_append_indices1(self, *index + offset);
	}
}

/******************************************************************************
* add_submesh
* Appends the given mesh to this mesh's submesh array.
******************************************************************************/
void METHOD(tv_model, add_submesh, tv_model* mesh)
{
	utarray_push_back(self->submeshes, mesh);
}
/******************************************************************************
* set_submesh
* Sets the mesh at the given index in the array (reallocating the array if
* necessary before doing so).
******************************************************************************/
void METHOD(tv_model, set_submesh, tvuint index, tv_model* mesh)
{
	tvuint len = utarray_len(self->submeshes);
	if(len <= index) {
		/* reserve space for this index and some arbitrary additional extras */
		utarray_reserve(self->submeshes, len + 2);
	}
	/* insert the mesh at the specified index */
	utarray_insert(self->submeshes, mesh, index);
}
/******************************************************************************
* get_submesh
* Look up the given submesh index in the mesh.
******************************************************************************/
tv_model* METHOD(tv_model, get_submesh, tvuint index)
{
	tvuint len = utarray_len(self->submeshes);
	if(len <= index) {
		return NULL;
	}
	return *((tv_model**)utarray_eltptr(self->submeshes, index));
}
/******************************************************************************
* tv_model_get_aabb
* Iterate over the mesh to find the bounds of the axis-aligned bounding box
* for that mesh.
******************************************************************************/
TvAABB tv_model_get_aabb(tv_model* model)
{
	TvAABB aabb;
	/* TODO: */
	aabb.d = 0;
	aabb.h = 0;
	aabb.w = 0;
	return aabb;
}

/******************************************************************************
* tv_model_get_property_size
* Get the size of the PLY property in bytes.  If 
* TV_MODEL_STORE_ATTRIBUTES_AS_FLOATS is defined, then this function always
* returns 4. Attributes should be properly converted to a GLfloat 
* representation when converting from other types.
******************************************************************************/
tvuint tv_model_get_property_size(tvuint data_type)
{
#ifdef TV_MODEL_STORE_ATTRIBUTES_AS_FLOATS
	return 4;
#else
	switch(data_type) {
	case TV_MODEL_PROPERTY_CHAR: return 1;
	case TV_MODEL_PROPERTY_UCHAR: return 1;
	case TV_MODEL_PROPERTY_SHORT: return 2;
	case TV_MODEL_PROPERTY_USHORT: return 2;
	case TV_MODEL_PROPERTY_INT: return 4;
	case TV_MODEL_PROPERTY_UINT: return 4;
	case TV_MODEL_PROPERTY_FLOAT: return 4;
	case TV_MODEL_PROPERTY_DOUBLE: return 8;
	case TV_MODEL_PROPERTY_LIST: return 0;
	default: return 0;
	}
#endif
}
/******************************************************************************
* tv_model_get_attribute
* Look up the attribute associated with the given index offset.
******************************************************************************/
tvpointer tv_model_get_attribute(tv_model* model, tvuint i, tvuint attribute)
{
	return (tvpointer)(utarray_eltptr(model->vertices, i) + model->vertex_attributes[attribute].offset);
}
/******************************************************************************
* tv_model_apply_scale
* Multiplies every vertex in the mesh by a vector. 
******************************************************************************/
void tv_model_apply_scale(tv_model *model, tv_vector3 scale)
{
	tvuint i;
	/* TODO: probably the wrong usage of the term "scale".... */
	for(i = 0; i < model->num_vertices; ++i) {
		tv_vector3* v = (tv_vector3*)tv_model_get_attribute(model, i, MODEL_ATTRIBUTE_VERTEX);
		assert(v != NULL);
		v->x *= scale.x;
		v->y *= scale.y;
		v->z *= scale.z;
	}
}
/*******************************************************************************
 * tv_model_apply_translate
 * Adds the given vector offset to all vertices in the mesh.
 ******************************************************************************/
void tv_model_apply_translate(tv_model *model, tv_vector3 offset, tvuint attribute)
{
	tvuint i;
	for(i = 0; i < model->num_vertices; ++i) {
		tv_vector3* v = (tv_vector3*)tv_model_get_attribute(model, i, attribute);
		assert(v != NULL);
		v->x += offset.x;
		v->y += offset.y;
		v->z += offset.z;
	}
}
/*******************************************************************************
 * print
 ******************************************************************************/
void print_attribute(tv_model* mesh, tvuint attr_id, tvuint vertex)
{
	tvuint i;
	tvchar line[256];
	tv_model_attribute attr = mesh->vertex_attributes[attr_id];
	/* get the data that this vertex contains for the specified attribute */
	tvpointer data = tv_model_get_attribute(mesh, vertex, attr_id);

	/* print the index of the attribute being printed */
	printf(" [%d]: <", attr_id);
	for(i = 0; i < attr.count; ++i) {
		switch(attr.data_type) {
		/* TODO: currently only FLOAT is supported as a type */
		case TV_MODEL_PROPERTY_FLOAT:
			printf("%f,", ((tvfloat*)data)[i]);
			break;
		case TV_MODEL_PROPERTY_UCHAR:
			printf("%f,", ((tvfloat*)data)[i]);
			break;
		default:
			printf("unknown type");
		}
	}
	printf(">\n");
}
void METHOD(tv_model, print)
{
	GLvoid* v;
	tvuint i;

	tv_info("vertices");

	for(v = (GLvoid*)utarray_front(self->vertices); v != NULL; 
		v = (GLvoid*)utarray_next(self->vertices, v)) {
		/* get the current vertex # that we're on */
		tvuint j = utarray_eltidx(self->vertices, v);
		printf("vertex %d\n", j);
		/* print each attribute for the current vertex */
		for(i = 0; i < self->num_properties; ++i) {
			print_attribute(self, i, j);
		}
	}
}

/*****************************************************************************/
/* Getters and Setters */
tv_array *tv_model_get_vertices(tv_model* model)
{
	return model->vertices;
}
void tv_model_set_vertices(tv_model *model, tv_array *vertices)
{
	model->vertices = vertices;
}
tv_array *tv_model_get_indices(tv_model* model)
{
	return model->indices;
}
void tv_model_set_indices(tv_model *model, tv_array *indices)
{
	model->indices = indices;
}
tv_model_vbo_handle tv_model_get_vertex_handle(tv_model *model)
{
	return model->vertex_vbo;
}
void tv_model_set_vertex_handle(tv_model *model, tvuint new_handle)
{
	model->vertex_vbo = new_handle;
}
tv_model_vbo_handle tv_model_get_index_handle(tv_model *model)
{
	return model->index_vbo;
}
void tv_model_set_index_handle(tv_model *model, tvuint new_handle)
{
	model->index_vbo = new_handle;
}

/*****************************************************************************
******************************** COMPONENT **********************************
*****************************************************************************/
COMPONENT_NEW(tv_model, tv_component)
	self->vertices = NULL;
	self->num_vertices = 0;
	self->num_properties = 0;
	self->vertex_size = 0;
	utarray_new(self->indices, &ut_index_icd);
	utarray_new(self->submeshes, &ut_ptr_icd);
END_COMPONENT_NEW(tv_model)
/*****************************************************************************
************************************ START **********************************
*****************************************************************************/
COMPONENT_START(tv_model)
END_COMPONENT_START
/*****************************************************************************
*********************************** UPDATE **********************************
*****************************************************************************/
COMPONENT_UPDATE(tv_model)
END_COMPONENT_UPDATE
/*****************************************************************************
********************************** DESTROY **********************************
*****************************************************************************/
COMPONENT_DESTROY(tv_model)
if(self->vertices) {
	utarray_free(self->vertices);
}
if(self->indices) {
	utarray_free(self->indices);
}
glDeleteBuffers(1, &self->index_vbo);
glDeleteBuffers(1, &self->vertex_vbo);
glDeleteVertexArrays(1, &self->vao);
END_COMPONENT_DESTROY

#ifdef __cplusplus
}
#endif