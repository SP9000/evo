#include "model.h"
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* a structure for holding the names and references of loaded models */
typedef struct loaded_model {
	tv_model *model;
	tvchar name[32];
	TvHashHandle hh;
} loaded_model;

/* information pertaining to the content of a .PLY property */
typedef struct ply_property {
	tvint value;
	tv_model_property_type type;
	tvuint size;
	/* if type is TV_MODEL_PROPERTY_LIST, this is the list of types */
	tvuint type_list[32];
	tvchar name[32];
	UT_hash_handle hh;
}ply_property;

/* information pertaining to the content of a .PLY element */
typedef struct ply_element {
	tvint value;
	ply_property *properties;
	tvchar name[32];
	UT_hash_handle hh;
}ply_element;

/* A structure for holding info about the PLY file */
typedef struct ply_info {
	ply_element *elements_table;
}ply_info;

/* An enumeration of the different groups of properties */
typedef enum {
	GROUP_POSITION,
	GROUP_NORMAL,
	GROUP_COLOR,
	GROUP_TEXCO
}property_group_type;

/* A structure that contains information about a property group */
typedef struct property_group {
	property_group_type type;
	tvuint data_type;
	tvuint count;
}property_group;

/*****************************************************************************/
/* a helper function to read an element from a PLY file. TRUE=success */
static tvbool read_element(tvchar *line, ply_info *info, tvchar *element_name);
/* a helper function to read a property from a PLY file. TRUE=success */
tvbool read_property(tvchar *line, ply_info *info, tvchar *cur_element);
/* helper functions to check if the property is one of several known types. */
static tvbool property_is_position(tvchar *name);
static tvbool property_is_normal(tvchar *name);
static tvbool property_is_color(tvchar *name);
static tvbool property_is_texco(tvchar *name);

/* a table of models that have already been loaded. */
static loaded_model* loaded_models = NULL;
/* The ICD for a model's indices */
UT_icd ut_index_icd = {sizeof(GLshort), 0, 0, 0};

/*****************************************************************************/
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

tvbool read_element(tvchar *line, ply_info *info, tvchar *element_name)
{
	/* 3 buffers: "element", the element name, and the # of properties */
	tvchar a[256];
	tvchar b[256];
	tvchar c[256];

	tvuint num_read = sscanf(line, "%s %s %s", a, b, c);
	ply_element *el = (ply_element*)tv_alloc(sizeof(ply_element));

	/* make sure the line we're on is an element */
	if(strncmp(a, "element", 8) == 0) {
		/* see that the element matches the expected formatting */
		if(num_read != 3) {
			tv_warning("element %s has unrecognized formatting in declaration.\n", a);
		}
		if((strncmp(b, "vertex", 6) != 0) && (strncmp(b, "face", 4) != 0)) {
			tv_warning("unrecognized element %s\n", a);
		}
		el->value = atoi(c);
		/* copy the element's name to the name buffer */
		strncpy(el->name, b, 32);
		/* copy the name of the read element for the caller */
		strncpy(element_name, b, 32);
		/* clear the element's property table */
		el->properties = NULL;
		/* add the element to the PLY info's elements table */
		HASH_ADD_STR(info->elements_table, name, el);

		/* successfully read an element */
		return TRUE;
	}
	/* no element could be read */
	return FALSE;
}

tvbool read_property(tvchar *line, ply_info *info, tvchar *cur_element)
{
	/* 3 buffers: "property", property type, property name */
	tvchar a[256];
	tvchar b[256];
	tvchar c[256];
	
	ply_element *el;
	ply_property *p = (ply_property*)tv_alloc(sizeof(ply_property));

	/* read the property */
	tvuint num_read = sscanf(line, "%s %s %s", a, b, c);

	/* check that the line is a property */
	if(strncmp("property", a, 8) != 0) {
		return FALSE;
	}
	/* get the property type and use it to get the size of the property */
	if(strncmp(b, "char", 4) == 0) {
		p->size = 1;
		p->type = TV_MODEL_PROPERTY_CHAR;
	}
	else if(strncmp(b, "uchar", 5) == 0) {
		p->size = 1;
		p->type = TV_MODEL_PROPERTY_UCHAR;
	}
	else if(strncmp(b, "short", 5) == 0) {
		p->size = 2;
		p->type = TV_MODEL_PROPERTY_SHORT;
	}
	else if(strncmp(b, "ushort", 6) == 0) {
		p->size = 2;
		p->type = TV_MODEL_PROPERTY_USHORT;
	}
	else if(strncmp(b, "int", 3) == 0) {
		p->size = 4;
		p->type = TV_MODEL_PROPERTY_INT;
	}
	else if(strncmp(b, "uint", 4) == 0) {
		p->size = 4;
		p->type = TV_MODEL_PROPERTY_UINT;
	}
	else if(strncmp(b, "float", 5) == 0) {
		p->size = 4;
		p->type = TV_MODEL_PROPERTY_FLOAT;
	}
	else if(strncmp(b, "double", 6) == 0) {
		p->size = 8;
		p->type = TV_MODEL_PROPERTY_DOUBLE;
	}
	/* copy the name of the property to the property structure */
	strncpy(p->name, c, 32);
	
	/* make sure an element is already defined, if so add this property to it*/
	HASH_FIND_STR(info->elements_table, cur_element, el);
	if(el) {
		HASH_ADD_STR(el->properties, name, p);
		return TRUE;
	}
	/* if there's no element defined already, quit */
	else {
		tv_warning("read property %s before any element was defined.\n", c);
		return FALSE;
	}
}

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
tvbool property_is_texco(tvchar *name) 
{
	/* does the given name match any recognized texco name pattern? */
	if(strncmp(name, "u", 1) == 0 ||
		strncmp(name, "v", 1) == 0) {
			return TRUE;
	}
	return FALSE;
}

/*****************************************************************************/
COMPONENT_NEW(tv_model, tv_component)
  	self->vertices = NULL;
	self->num_vertices = 0;
	self->num_properties = 0;
	self->vertex_size = 0;
	utarray_new(self->indices, &ut_index_icd);
END_COMPONENT_NEW(tv_model)

COMPONENT_START(tv_model)
END_COMPONENT_START

COMPONENT_UPDATE(tv_model)
END_COMPONENT_UPDATE
COMPONENT_DESTROY(tv_model)
utarray_free(self->vertices);
utarray_free(self->indices);
glDeleteBuffers(1, &self->index_vbo);
glDeleteBuffers(1, &self->vertex_vbo);
glDeleteVertexArrays(1, &self->vao);
END_COMPONENT_DESTROY
/*****************************************************************************/
void tv_model_load_ply(tv_model *model, tvchar* file)
{
	loaded_model *lup;

	FILE *fp;
	tvuint i, j, k;
	tvchar line_buffer[1024];

	tvuint num_faces;
	tvuint num_vertices;
	tvpointer my_vertex;
	UT_icd my_vertex_icd = {0, NULL, NULL, NULL};

	ply_info file_info;
	ply_property *prop, *prop_tmp;
	ply_element *element, *element_tmp;

	/* if the model table isn't empty check if the model has already been 
	loaded */
	if(loaded_models != NULL) {
		HASH_FIND_STR(loaded_models, file, lup);
		if(lup) {
			/*TODO: (deep) copy model from lup..I guess that'd be preferable? */
			model = lup->model;
			return;
		}
	}

	/* open the file and verify it is a .ply file */
	fp = fopen(file, "r");
	if(fp == NULL) {
		fprintf(stderr, "Error: could not open %s\n", file);
		return;
	}

	file_info.elements_table = NULL;
	model->vertex_size = 0;
	
	/* read the file header - alloc the buffers to store the model data */
    while(!feof(fp)) {
		tvchar element[256];

		/* read a line from the file for parsing */
		fgets(line_buffer, 1024, fp);
		/* if we've reached the end of the header, we're done */
	    if(strncmp(line_buffer, "end_header", 10) == 0) {
			break;
		}
		/* comment? */
		if(read_comment(line_buffer)) {
			continue;
		}
		/* element? */
		if(read_element(line_buffer, &file_info, element)) {
			continue;
		}
		/* property? */
		if(read_property(line_buffer, &file_info, element)) {
			continue;
		}
	}

	/* extract all the data we collected about the elements and properties of this
	 * model. */
	model->num_properties = 0;
	/* iterate over all the elements to construct the attribute format for the 
	model */
	HASH_ITER(hh, file_info.elements_table, element, element_tmp) {
		if(strncmp("vertex", element->name, 6) == 0) {
			tv_model_property_type prev_type = TV_MODEL_PROPERTY_NONE;
			tvint cur_group = -1;
			tvint prev_group = -1;
			tvuint cur_found = 0;
			num_vertices = element->value;

			/* foreach property, add its info to the model. */
			HASH_ITER(hh, element->properties, prop, prop_tmp) {
				tv_model_attribute p = {prev_type, cur_found+1, 0};
				/* position? */
				if(property_is_position(prop->name)) {
					cur_group = GROUP_POSITION;
					if(cur_found == 2) {
						tv_model_append_property(model, &p);
					}
				}
				/* normal? */
				else if(property_is_normal(prop->name)) {
					cur_group = GROUP_NORMAL;
					if(cur_found == 2) {
						tv_model_append_property(model, &p);
					}
				}
				/* color? */
				else if(property_is_color(prop->name)) {
					fprintf(stdout, "found color\n");
					cur_group = GROUP_COLOR;
					if(cur_found == 2) {
						tv_model_append_property(model, &p);
					}
				}
				/* texco? */
				else if(property_is_texco(prop->name)) {
					fprintf(stdout, "found texco\n");
					cur_group = GROUP_TEXCO;
					if(cur_found == 1) {
						tv_model_append_property(model, &p);
					}
				}
				/* unrecognized property type */
				else {
					tv_warning("Warning: unrecognized property type %s.\n", prop->name);
				}

				if(cur_found == 0 || cur_group == prev_group) {
					++cur_found;
				}
				else {
					cur_found = 1;
				}
				prev_type = prop->type;
				prev_group = cur_group;

				/* clean up */
				HASH_DEL(element->properties, prop);
				free(prop);
			}
		}
		else if(strncmp("face", element->name, 4) == 0) {
			num_faces = element->value;
		}
		/* clean up */
		HASH_DEL(file_info.elements_table, element);
		free(element);
	}

	/* allocate a vertex of the size of our vertices */
	my_vertex =  (tvpointer)tv_alloc(model->vertex_size);
	/* create an array ICD for the vertices we will be adding */
	my_vertex_icd.sz =model->vertex_size;
	utarray_new(model->vertices, &my_vertex_icd);

	for(i = 0; i < num_vertices; ++i) {
		for(j = 0; j < model->num_properties; ++j) {
			for(k = 0; k < model->vertex_attributes[j].count; ++k) {
				/* the address to write the data we read to.
				 * (base + property offset + property component offst).
				 * e.g. vertex_base + offset(position) + offset(position.x) */
				tvbyte* property_loc = (tvbyte*)my_vertex + 
					model->vertex_attributes[j].offset + 
					tv_model_get_property_size(model->vertex_attributes[j].data_type)*k;

				fscanf(fp, "%s", line_buffer);
				switch(model->vertex_attributes[j].data_type) {
	#ifdef TV_MODEL_STORE_ATTRIBUTES_AS_FLOATS
				case TV_MODEL_PROPERTY_CHAR:
				case TV_MODEL_PROPERTY_UCHAR:
					*(tvfloat*)property_loc = atof(line_buffer) / 255.0f;
					break;
				case TV_MODEL_PROPERTY_SHORT:
				case TV_MODEL_PROPERTY_USHORT:
					*(tvfloat*)property_loc = (tvfloat)atoi(line_buffer) / (tvfloat)0xffff;
					break;
				case TV_MODEL_PROPERTY_INT:
				case TV_MODEL_PROPERTY_UINT:
					*(tvfloat*)property_loc = (tvfloat)atoi(line_buffer) / (tvfloat)0xffffffff;
					break;
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
				case TV_MODEL_PROPERTY_FLOAT:
					*(tvfloat*)property_loc = (tvfloat)atof(line_buffer);
					break;
				case TV_MODEL_PROPERTY_DOUBLE:
					*(tvfloat*)property_loc = (tvdouble)atof(line_buffer);
					break;
				}
			}
		}
		utarray_push_back(model->vertices, my_vertex);
	}
	for(i = 0; i < num_faces; ++i) {
		tvuint face_buff[4];
		tvint face_size = 0;

		fgets(line_buffer, 1024, fp);
		sscanf(line_buffer, "%d", &face_size);
		if(face_size == 4) {
			sscanf(line_buffer, "%d %d %d %d %d", &face_size, &face_buff[0], &face_buff[1], &face_buff[2], &face_buff[3]);
			utarray_push_back(model->indices, &face_buff[0]);
			utarray_push_back(model->indices, &face_buff[1]);
			utarray_push_back(model->indices, &face_buff[2]);
			utarray_push_back(model->indices, &face_buff[2]);
			utarray_push_back(model->indices, &face_buff[3]);
			utarray_push_back(model->indices, &face_buff[0]);
		}
		else if(face_size == 3) {
			sscanf(line_buffer, "%d %d %d %d", &face_size, &face_buff[0], &face_buff[1], &face_buff[2]);
			utarray_push_back(model->indices, &face_buff[0]);
			utarray_push_back(model->indices, &face_buff[1]);
			utarray_push_back(model->indices, &face_buff[2]);
		}
		else {
			fprintf(stderr, "Warning: unrecognized # of vertices per face %d\n", face_size);
		}
	}
	model->primitive = GL_TRIANGLES;

	lup = (loaded_model*)tv_alloc(sizeof(loaded_model));
	strncpy(lup->name, file, 32);
	lup->model = model;
	HASH_ADD_STR(loaded_models, name, lup);

	/* clean up */
	free(my_vertex);
}

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

void tv_model_reoptimize(tv_model* model, tvbool optimize_vertices, tvbool optimize_indices)
{
	tvuint i;
	tvuint j;
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
}

void tv_model_insert_vertex(tv_model *model, tvuint index, GLvoid *data)
{
	utarray_insert(model->vertices, data, index);
}

void tv_model_append_indices1(tv_model* model, tvuint i)
{
	utarray_push_back(model->indices, &i);
}

void tv_model_append_indices2(tv_model* model, tvuint i0, tvuint i1)
{
	utarray_push_back(model->indices, &i0);
	utarray_push_back(model->indices, &i1);
}

void tv_model_append_indices3(tv_model* model, tvuint i0, tvuint i1, tvuint i2)
{
	utarray_push_back(model->indices, &i0);
	utarray_push_back(model->indices, &i1);
	utarray_push_back(model->indices, &i2);
}

void tv_model_append_indices4(tv_model* model, tvuint i0, tvuint i1, tvuint i2, tvuint i3)
{
	utarray_push_back(model->indices, &i0);
	utarray_push_back(model->indices, &i1);
	utarray_push_back(model->indices, &i2);
	utarray_push_back(model->indices, &i3);
}

void tv_model_append_indices(tv_model* model, tvuint count, tvuint* indices)
{
	tvuint i;
	for(i = 0; i < count; ++i) {
		utarray_push_back(model->indices, &indices[i]);
	}
}

TvAABB tv_model_get_aabb(tv_model* model)
{
	TvAABB aabb;
	/* TODO: */
	aabb.d = 0;
	aabb.h = 0;
	aabb.w = 0;
	return aabb;
}

void tv_model_free(tv_model* model)
{
	if(model == NULL) {
		/* I don't think so, m8 */
		return;
	}
	if(model->vertex_vbo) {
		glDeleteBuffers(1, &model->vertex_vbo);
	}
	if(model->index_vbo) {
		glDeleteBuffers(1, &model->index_vbo);
	}
	utarray_free(model->vertices);
	utarray_free(model->indices);

	tv_component_free((tv_component*)model);
}

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
tvpointer tv_model_get_attribute(tv_model* model, tvuint i, tvuint attribute)
{
	return (tvpointer)(utarray_eltptr(model->vertices, i) + model->vertex_attributes[attribute].offset);
}

void tv_model_apply_scale(tv_model *model, tv_vector3 scale)
{
	tvuint i;
	for(i = 0; i < model->num_vertices; ++i) {
		tv_vector3* v = (tv_vector3*)tv_model_get_attribute(model, i, MODEL_ATTRIBUTE_VERTEX);
		assert(v != NULL);
		*v = tv_vector3_scale(*v, scale);
	}
}

/*****************************************************************************/
/* Getters and Setters														 */
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

#ifdef __cplusplus
}
#endif