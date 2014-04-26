#include "model.h"

tv_model* loaded_models;

UT_icd ut_index_icd = {sizeof(GLshort), 0, 0, 0};

COMPONENT_NEW(tv_model, tv_component)
	tv_component *c = (tv_component*)self;
	c->Update = update;
	c->Start = start;
	self->vertices = NULL;
	self->num_vertices = 0;
	utarray_new(self->indices, &ut_index_icd);
	utarray_new(self->vertex_property_offsets, &ut_int_icd);
	utarray_new(self->vertex_property_types, &ut_int_icd);
END_COMPONENT_NEW(tv_model)

COMPONENT_START(tv_model)
END_COMPONENT_START

COMPONENT_UPDATE(tv_model)
END_COMPONENT_UPDATE

int tv_model_init()
{
	return 0;
}

void tv_model_load_ply(tv_model *model, tvchar* file)
{
	tv_model* lup;
	FILE* fp;
	tvuint i, j;
	tvchar line_buffer[1024];

	tvuint num_faces;
	tvuint num_vertices;
	tvpointer my_vertex;
	UT_icd my_vertex_icd = {0, NULL, NULL, NULL};

	GLfloat *vertices;
	GLfloat *colors;

	HASH_FIND_PTR(loaded_models, file, lup);
	if(lup) {
		/*TODO: copy model from lup */
		return;
	}

	/* open the file and verify it is a .ply file */
	fp = fopen(file, "r");
	if(fp == NULL) {
		fprintf(stderr, "Error: could not open %s\n", file);
		return;
	}

	model->vertex_size = 0;
	/* read the file header - alloc the buffers to store the model data */
    while(!feof(fp)) {
		tvchar a[256];
		tvchar b[256];
		tvchar c[256];
		tvint num_read;

		fgets(line_buffer, 1024, fp);
		num_read = sscanf(line_buffer, "%s %s %s", a, b, c);

	    if(strncmp(line_buffer, "end_header", 10) == 0) {
			puts("END HEADER");
			break;
		}
		/* if nothing was read or this line is a comment, continue */
		if(num_read == 0 || strncmp(a, "comment", 8) == 0) {
			continue;
		}
		/* make sure 3 items were read */
		if(num_read != 3) {
			continue;
		}

		/* read all the attributes that are used */
		if(strncmp(a, "element", 8) == 0) {
			if(strncmp(b, "vertex", 6) == 0) {
				num_vertices = atoi(c);
			}
			else if(strncmp(b, "face", 4) == 0) {
				num_faces = atoi(c);
			}
		}
		else if(strncmp(a, "property", 8) == 0) {
			tvuint p_size = 0;
			tvuint p_type = 0;
			/*
			char       character                 1
			uchar      unsigned character        1
			short      short integer             2
			ushort     unsigned short integer    2
			int        integer                   4
			uint       unsigned integer          4
			float      single-precision float    4
			double     double-precision float    8
			*/
			if(strncmp(b, "char", 4) == 0) {
				p_size = 1;
				p_type = TV_MODEL_PROPERTY_CHAR;
			}
			else if(strncmp(b, "uchar", 5) == 0) {
				p_size = 1;
				p_type = TV_MODEL_PROPERTY_UCHAR;
			}
			else if(strncmp(b, "short", 5) == 0) {
				p_size = 2;
				p_type = TV_MODEL_PROPERTY_SHORT;
			}
			else if(strncmp(b, "ushort", 6) == 0) {
				p_size = 2;
				p_type = TV_MODEL_PROPERTY_USHORT;
			}
			else if(strncmp(b, "int", 3) == 0) {
				p_size = 4;
				p_type = TV_MODEL_PROPERTY_INT;
			}
			else if(strncmp(b, "uint", 4) == 0) {
				p_size = 4;
				p_type = TV_MODEL_PROPERTY_UINT;
			}
			else if(strncmp(b, "float", 5) == 0) {
				p_size = 4;
				p_type = TV_MODEL_PROPERTY_FLOAT;
			}
			else if(strncmp(b, "double", 6) == 0) {
				p_size = 8;
				p_type = TV_MODEL_PROPERTY_DOUBLE;
			}
			else {
				/* TODO: hack to allow face properties to be ignored */
				continue;
			}
#ifdef TV_MODEL_STORE_ATTRIBUTES_AS_FLOATS
			p_size = sizeof(tvfloat);
#endif
			utarray_push_back(model->vertex_property_types, &p_type);
			utarray_push_back(model->vertex_property_offsets, &model->vertex_size);
			model->vertex_size += p_size;
		}
	}

	/* allocate a vertex of the size of our vertices */
	my_vertex =  (tvpointer)tv_alloc(model->vertex_size);
	/* create an array ICD for the vertices we will be adding */
	my_vertex_icd.sz =model->vertex_size;
	utarray_new(model->vertices, &my_vertex_icd);

	for(i = 0; i < num_vertices; ++i) {
		for(j = 0; j < utarray_len(model->vertex_property_types); ++j) {
			tvbyte* property_loc = (tvbyte*)my_vertex + (*(tvuint*)utarray_eltptr(model->vertex_property_offsets, j));

			fscanf(fp, "%s", line_buffer);
			switch(*((tvint*)(utarray_eltptr(model->vertex_property_types,j)))) {
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
	HASH_ADD_PTR(loaded_models, name, model);
}

void tv_model_vertex_format(tv_model* model, tvuint num_properties, tvuint *property_sizes)
{
	tvuint i;
	tvuint offset;
	UT_icd my_vertex_icd = {0, 0, 0, 0};

	if(model->vertex_property_offsets == NULL) {
		utarray_new(model->vertex_property_offsets, &ut_int_icd);
	}
	utarray_clear(model->vertex_property_offsets);

	for(i = 0, offset = 0; i < num_properties; ++i) {
		utarray_push_back(model->vertex_property_offsets, &offset);
		offset += property_sizes[i];
	}
	model->vertex_size = offset;
	my_vertex_icd.sz = offset;

	if(model->vertices != NULL) {
		utarray_free(model->vertices);
	}
	utarray_new(model->vertices, &my_vertex_icd);
}

void tv_model_buffer_attribute(tv_model* model, tvuint attribute, TvArray* buffer)
{
#if 0 
	utarray_push_back(model->attributes, &buffer);
	utarray_push_back(model->attribute_types, &attribute);
	utarray_push_back(model->attribute_sizes, &(utarray_len(buffer)));
#endif
}

void tv_model_set_vertex(tv_model *model, tvuint index, GLvoid *data)
{
	/* TODO */
}

void tv_model_append_vertex(tv_model *model, GLvoid* data)
{
	utarray_push_back(model->vertices, data);
}

void tv_model_insert_vertex(tv_model *model, tvuint index, GLvoid *data)
{
	utarray_insert(model->vertices, data, index);
}

void tv_model_append_tri(tv_model* model, tvuint i0, tvuint i1, tvuint i2)
{
	utarray_push_back(model->indices, &i0);
	utarray_push_back(model->indices, &i1);
	utarray_push_back(model->indices, &i2);
}

void tv_model_append_quad(tv_model* model, tvuint i0, tvuint i1, tvuint i2, tvuint i3)
{
	utarray_push_back(model->indices, &i0);
	utarray_push_back(model->indices, &i1);
	utarray_push_back(model->indices, &i2);
	utarray_push_back(model->indices, &i3);
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

void tv_model_optimize(tv_model* model)
{
	tvuint i;

    /* create a VAO for the model */
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);

	printf("v: %d, i: %d\n", utarray_len(model->vertices), utarray_len(model->indices));

	/* generate/bind/buffer the per-vertex data buffer */
	glGenBuffers(1, &model->vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, model->vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, 
		model->vertex_size * utarray_len(model->vertices),
		(GLfloat*)utarray_front(model->vertices),
		GL_STATIC_DRAW);

	/* generate/bind/buffer the index buffer */
	glGenBuffers(1, &model->index_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->index_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,	
		utarray_len(model->indices) * sizeof(GLshort),
		(GLshort*)utarray_front(model->indices),
		GL_STATIC_DRAW);

	/* set the attribute pointers for each per-vertex property */
	for(i = 0; i < utarray_len(model->vertex_property_offsets); ++i) {
	    glEnableVertexAttribArray(i);
		glVertexAttribPointer((GLuint)i, 3, GL_FLOAT, GL_FALSE, 
			model->vertex_size,
			(GLvoid*)(i*3*sizeof(tvfloat)));
    }
    /* Unbind. */
    glBindVertexArray(0);
}

void tv_model_free(tv_model* model)
{
#if 0
	TvArray **attr_it;
	
	/* free each attribute buffer */
	for(attr_it = (TvArray**)utarray_front(model->attributes); 
		attr_it != NULL; 
		attr_it = (TvArray**)utarray_next(model->attributes, attr_it)) {
			utarray_free(*attr_it);
	}
	utarray_free(model->attributes);
	utarray_free(model->attribute_types);
	utarray_free(model->attribute_sizes);
#endif
}

tvuint tv_model_get_attribute_num_elements(tvuint attribute_id)
{
	switch(attribute_id) {
	case MODEL_ATTRIBUTE_NONE: return 0;
	case MODEL_ATTRIBUTE_VERTEX: return MODEL_ATTRIBUTE_VERTEX_NUM_ELEMENTS;
	case MODEL_ATTRIBUTE_COLOR: return MODEL_ATTRIBUTE_COLOR_NUM_ELEMENTS;
	case MODEL_ATTRIBUTE_NORMAL: return MODEL_ATTRIBUTE_NORMAL_NUM_ELEMENTS;
	case MODEL_ATTRIBUTE_TEXCO: return MODEL_ATTRIBUTE_TEXCO_NUM_ELEMENTS;
	case MODEL_ATTRIBUTE_INDEX: return MODEL_ATTRIBUTE_INDEX_NUM_ELEMENTS;
	}
	return 0;
}

TvArray* tv_model_get_attribute(tv_model* model, tvuint attribute)
{
#if 0
	tvuint i;
	tvuint *type;

	for(type = (tvuint*)utarray_front(model->attribute_types), i = 0; 
		type != 0;
		type = (tvuint*)utarray_next(model->attribute_types, type), ++i) {
		if(*type == attribute) {
			return *(TvArray**)utarray_eltptr(model->attributes, i);
		}
	}
	return NULL;
#endif
}

GLvoid *tv_model_get_attribute_idx(tv_model *model, tvuint index)
{
	//return (GLvoid*)utarray_eltptr(model->attributes, index); 
}

tvuint tv_model_get_attribute_size_idx(tv_model *model, tvuint index)
{
	//return utarray_len(*(TvArray**)utarray_eltptr(model->attributes, index));
}