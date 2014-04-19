#include "model.h"

tv_model* loaded_models;

COMPONENT_NEW(tv_model, tv_component)
	tv_component *c = (tv_component*)self;
	c->Update = update;
	c->Start = start;
	utarray_new(self->attributes, &ut_ptr_icd);
	utarray_new(self->attribute_types, &ut_int_icd);
	utarray_new(self->attribute_sizes, &ut_int_icd);
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
	TvArray *attribute;
	TvArray **attribute_it;
	TvArray *attribute_num_elements;
	FILE* fp;
	tvuint i, j, k;
	tvchar line_buffer[1024];

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

	utarray_new(model->attributes, &ut_ptr_icd);
	utarray_new(model->attribute_types, &ut_int_icd);
	utarray_new(model->attribute_sizes, &ut_int_icd);
	utarray_new(attribute_num_elements, &ut_int_icd);

	/* read the file header - alloc the buffers to store the model data */
    while(!feof(fp)) {
		tvchar a[256];
		tvchar b[256];
		tvint c;
		tvint num_read;

		fgets(line_buffer, 1024, fp);
		num_read = sscanf(line_buffer, "%s %s %d", a, b, &c);

	    if(strncmp(line_buffer, "end_header", 10) == 0) {
			puts("END HEADER");
			break;
		}
		if(num_read == 0 || strncmp(a, "comment", 8) == 0) {
			continue;
		}
		if(num_read != 3) {
			continue;
		}

		/* read all the attributes that are used */
		if(strncmp(a, "element", 8) == 0) {
			tvuint type = MODEL_ATTRIBUTE_NONE;
			tvuint size;
			if(strncmp(b, "vertex", 6) == 0) {
				size = MODEL_ATTRIBUTE_VERTEX_NUM_ELEMENTS;
				type = MODEL_ATTRIBUTE_VERTEX;
			}
			else if(strncmp(b, "color", 5) == 0) {
				size = MODEL_ATTRIBUTE_COLOR_NUM_ELEMENTS;
				type = MODEL_ATTRIBUTE_COLOR;
			}
			else if(strncmp(b, "face", 4) == 0) {
				size = MODEL_ATTRIBUTE_INDEX_NUM_ELEMENTS;
				type = MODEL_ATTRIBUTE_INDEX;
			}
			if(type != MODEL_ATTRIBUTE_NONE) {
				/* allocate a new array for this attribute */
				if(type != MODEL_ATTRIBUTE_INDEX) {
					utarray_new(attribute, &ut_float_icd);
				}
				else {
					utarray_new(attribute, &ut_short_icd);
					model->num_indices = c * 4;
				}
				utarray_reserve(attribute, c * size);
				utarray_push_back(model->attributes, &attribute);
				utarray_push_back(model->attribute_sizes, &size);
				utarray_push_back(model->attribute_types, &type);
				utarray_push_back(attribute_num_elements, &c);
			}
		}
		else if(strncmp(a, "property", 9) == 0) {
			continue;
		}
	}

	/* read all the attributes in - everything but faces are floats */
	for(attribute_it = (TvArray**)utarray_front(model->attributes), i = 0;
		attribute_it != NULL;
		attribute_it = (TvArray**)utarray_next(model->attributes, attribute_it), ++i) {

		tvuint attribute_type = *(tvuint*)(utarray_eltptr(model->attribute_types, i));
		utarray_clear(*attribute_it);
		for(j = 0; j < *(tvint*)(utarray_eltptr(attribute_num_elements, i));  ++j) {
			int num_read = 0;
			float f_buff[6];
			int i_buff[5];

			if(feof(fp)) {
				fprintf(stderr, "Error: reached end of model PLY file prematurely\n");
				return;
			}
			fgets(line_buffer, 1024, fp);
			if(attribute_type != MODEL_ATTRIBUTE_INDEX) {
				num_read = sscanf(line_buffer, "%f %f %f %f %f %f",
					&f_buff[0], &f_buff[1], &f_buff[2], &f_buff[3], &f_buff[4], &f_buff[5]);
				if(num_read < 2) {
					fprintf(stderr, "Error: failed to read attribute (read %d)\n", j);
				}
				else {
					if(num_read == 6) num_read = 3;
					//TODO: for(k = 0; k < num_read; ++k) {
					for(k = 0; k < num_read; ++k) {
						utarray_push_back(*attribute_it, &f_buff[k]);
					}
					//TODO: delete
					if(attribute_type == MODEL_ATTRIBUTE_COLOR) {
						//utarray_push_back(*attribute_it, &f_buff[k-1]);
					}
				}
			}
			else {
				num_read = sscanf(line_buffer, "%d %d %d %d %d",
					&i_buff[0], &i_buff[1], &i_buff[2], &i_buff[3], &i_buff[4]);
				if(i_buff[0] == 4) {
					utarray_push_back(*attribute_it, &i_buff[1]);
					utarray_push_back(*attribute_it, &i_buff[2]);
					utarray_push_back(*attribute_it, &i_buff[3]);
					utarray_push_back(*attribute_it, &i_buff[3]);
					utarray_push_back(*attribute_it, &i_buff[4]);
					utarray_push_back(*attribute_it, &i_buff[1]);
				}
				else if(i_buff[0] == 3) {
					utarray_push_back(*attribute_it, &i_buff[1]);
					utarray_push_back(*attribute_it, &i_buff[2]);
					utarray_push_back(*attribute_it, &i_buff[3]);
				}
				else {
					fprintf(stderr, "Error: failed to read index (read %d)\n", j);
				}
			}
		}
		printf("j: %d len %d\n", j, utarray_len(*attribute_it));
	}
	model->primitive = GL_TRIANGLES;
	HASH_ADD_PTR(loaded_models, name, model);
}

void tv_model_add_attribute(tv_model* model, tvuint attribute)
{
	UT_icd* icd;
	TvArray *attr;
	tvuint num_elements;

	/* for index arrays, set type to be GLshort, otherwise float */
	icd = (attribute == MODEL_ATTRIBUTE_INDEX) ? &ut_short_icd : &ut_float_icd;

	num_elements = tv_model_get_attribute_num_elements(attribute);

	utarray_new(attr, icd);
	utarray_push_back(model->attribute_types, &attribute);
	utarray_push_back(model->attribute_sizes, &num_elements);
	utarray_push_back(model->attributes, &attr);
}

void tv_model_buffer_attribute(tv_model* model, tvuint attribute, TvArray* buffer)
{
	utarray_push_back(model->attributes, &buffer);
	utarray_push_back(model->attribute_types, &attribute);
	utarray_push_back(model->attribute_sizes, &(utarray_len(buffer)));
}

void tv_model_set_attribute(tv_model *model, tvuint attribute_id, tvuint vertex, GLfloat *data)
{
	TvArray *attr;
	tvuint element_size;
	tvuint i;

	attr = tv_model_get_attribute(model, attribute_id);
	element_size = tv_model_get_attribute_num_elements(attribute_id);
	for(i = vertex*element_size; i < vertex*element_size + element_size; ++i) {
		utarray_insert(attr, &data[i], i);
	}
}

void tv_model_append_attribute(tv_model *model, tvuint attribute_id, GLfloat *data)
{
	TvArray *attr;
	tvuint element_size;
	tvuint i;

	attr = tv_model_get_attribute(model, attribute_id);
	element_size = tv_model_get_attribute_num_elements(attribute_id);
	for(i = 0; i < element_size; ++i) {
		utarray_push_back(attr, &data[i]);
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

void tv_model_optimize(tv_model* model)
{
	tvuint i;

    /* create a VAO for the model */
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);

    /* buffer all the attributes of the model into VBO's */
	model->vbo_ids = (GLuint*)tv_alloc(utarray_len(model->attributes) * sizeof(GLuint));
	glGenBuffers(utarray_len(model->attributes), model->vbo_ids);

	for(i = 0; i < utarray_len(model->attributes); ++i) {
		TvArray **attr = (TvArray**)utarray_eltptr(model->attributes, i);
		tvuint *attr_size = (tvuint*)utarray_eltptr(model->attribute_sizes, i);
		tvuint *attr_type = (tvuint*)utarray_eltptr(model->attribute_types, i);

		/* if indices, save locataion and continue */
		if(*attr_type == MODEL_ATTRIBUTE_INDEX) {
			model->indices = (*attr);
			continue;
		}
		/* if not indices, buffer the attribute */
        glBindBuffer(GL_ARRAY_BUFFER, model->vbo_ids[i]);
		glBufferData(GL_ARRAY_BUFFER, 
			sizeof(GLfloat) * utarray_len(*attr),
			(GLfloat*)utarray_front(*attr),
			GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)i, *(tvuint*)utarray_eltptr(model->attribute_sizes, i) /* TODO *attr_size */, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(i);
    }
    /* Unbind. */
    glBindVertexArray(0);
}

void tv_model_free(tv_model* model)
{
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
}

GLvoid *tv_model_get_attribute_idx(tv_model *model, tvuint index)
{
	return (GLvoid*)utarray_eltptr(model->attributes, index); 
}

tvuint tv_model_get_attribute_size_idx(tv_model *model, tvuint index)
{
	return utarray_len(*(TvArray**)utarray_eltptr(model->attributes, index));
}