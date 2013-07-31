#include "model.h"

TvModel* loaded_models;
UT_icd float_icd = {sizeof(float), NULL, NULL, NULL};
UT_icd short_icd = {sizeof(GLshort), NULL, NULL, NULL};

int tv_model_init()
{
	return 0;
}

TvModel* tv_model_load_ply(tvchar* file)
{
	TvModelAttributeList* it;
	TvModel* model;
	FILE* fp;
	int i;
	int j;

	HASH_FIND_PTR(loaded_models, file, model);
	if(model) {
		return model;
	}
	/* open the file and verify it is a .ply file */
	fp = fopen(file, "r");
	if(fp == NULL) {
		fprintf(stderr, "Error: could not open %s\n", file);
		return NULL;
	}

	model = (TvModel*)malloc(sizeof(TvModel));
	model->attributes = NULL;

	/* read the file header - alloc the buffers to store the model data */
	while(!feof(fp)) {
		tvchar a[256];
		tvchar b[256];
		tvint c;
		int num_read = fscanf(fp, "%s %s %d", a, b, &c);
		if(num_read == 0 || strncmp(a, "comment", 8) == 0) {
			continue;
		}
	    if(strncmp(a, "end_header", 11) == 0) {
			break;
		}
		if(num_read != 3) {
			continue;
		}

		/* read all the attributes that are used */
		if(strncmp(a, "element", 8) == 0) {
			TvModelAttribute attr;
			TvModelAttributeList* to_add;
			attr.type = MODEL_ATTRIBUTE_NONE;
			if(strncmp(b, "vertex", 6) == 0) {
				attr.size = MODEL_ATTRIBUTE_VERTEX_SIZE;
				attr.type = MODEL_ATTRIBUTE_VERTEX;
			}
			else if(strncmp(b, "color", 5) == 0) {
				attr.size = MODEL_ATTRIBUTE_COLOR_SIZE;
				attr.type = MODEL_ATTRIBUTE_COLOR;
			}
			else if(strncmp(b, "face", 4) == 0) {
				attr.size = MODEL_ATTRIBUTE_INDEX_SIZE;
				attr.type = MODEL_ATTRIBUTE_INDEX;
			}
			if(attr.type != MODEL_ATTRIBUTE_NONE) {
				to_add = (TvModelAttributeList*)tv_alloc(sizeof(TvModelAttributeList));
				utarray_new(attr.data, &float_icd);
				to_add->attr = attr;
				LL_PREPEND(model->attributes, to_add);
			}
		}
		else if(strncmp(a, "property", 9) == 0) {
			continue;
		}
	}

	/* read all the attributes in - everything but faces are floats */
	for(it = model->attributes; it != NULL; it = it->next) {
		TvModelAttribute* ma = (TvModelAttribute*)it;
		for(i = 0; i < utarray_len(ma->data); ++i) {
			float f_buff[6];
			int i_buff[4];
			int read;
			if(ma->type != MODEL_ATTRIBUTE_INDEX) {
				read = fscanf(fp, "%f %f %f %f %f %f", 
					&f_buff[0], &f_buff[1], &f_buff[2], &f_buff[3], &f_buff[4], &f_buff[5]);
				for(j = 0; j < read; ++j) {
					utarray_push_back(ma->data, &f_buff[j]);
				}
			}
			else {
				read = fscanf(fp, "%d %d %d %d",
					&i_buff[0], &i_buff[1], &i_buff[2], &i_buff[3], &i_buff[4], &i_buff[5]);
				for(j = 1; j < read; ++j) {
					utarray_push_back(ma->data, &i_buff[j]);
				}
			}
		}
	}
	HASH_ADD_PTR(loaded_models, name, model);
	return model;
}

void tv_model_add_attribute(TvModel* model, tvuint attribute)
{
	UT_icd* icd;
	TvModelAttributeList* node = (TvModelAttributeList*)tv_alloc(sizeof(TvModelAttributeList));
	
	/* for index arrays, set type to be GLshort, otherwise float */
	icd = (attribute == MODEL_ATTRIBUTE_INDEX) ? &short_icd : &float_icd;
	utarray_new(node->attr.data, &float_icd);
	node->attr.type = attribute;
	node->attr.size = 0;
	LL_APPEND(model->attributes, node);
}

void tv_model_buffer_attribute(TvModel* model, tvuint attribute, TvArray* buffer)
{
	TvModelAttributeList* node = (TvModelAttributeList*)tv_alloc(sizeof(TvModelAttributeList));
	node->attr.data = buffer;
	node->attr.type = attribute;
	node->attr.size = utarray_len(buffer);
	LL_APPEND(model->attributes, node);
}

TvModelAttribute* tv_model_get_attribute(TvModel* model, tvuint attribute)
{
	TvModelAttributeList* it;
	for(it = model->attributes; it != NULL; it = it->next) {
		if(it->attr.type == attribute) {
			return &it->attr;
		}
	}
	return NULL;
}

TvAABB tv_model_get_aabb(TvModel* model)
{
	TvAABB aabb;
    int i;
    float min_x, min_y, min_z;
    float max_x, max_y, max_z;
	TvModelAttribute* vertices = tv_model_get_attribute(model, MODEL_ATTRIBUTE_VERTEX);

	/* TODO:
	for(i = 0; i < utarray_len(vertices->data); i += 3) {
        if(utarray_atvertices[i] < min_x) {
            min_x = vertices[i];
        }
        if(vertices[i] > max_x) {
            max_x = vertices[i];
        }
        if(vertices[i+1] < min_y) {
            min_y = vertices[i];
        }
        if(vertices[i+1] > max_y) {
            max_y = vertices[i+1];
        }
        if(vertices[i+2] < min_z) {
            min_z = vertices[i];
        }
        if(vertices[i+2] > max_z) {
            max_z = vertices[i+2];
        }
    }
    return (AABB){max_x-min_x, max_y-min_y, max_z-min_z};
	*/
	return aabb;
}

TvModel* tv_model_new()
{
	TvModel* model = (TvModel*)tv_alloc(sizeof(TvModel*));
	model->attributes = NULL;
	model->num_vertices = 0;
	model->vao = 0;
	model->vbo_ids = NULL;
	model->name = NULL;
	model->indices = NULL;
	return model;
}

void tv_model_optimize(TvModel* model)
{
	int i;
	int num_attrs;
	TvModelAttributeList* it;

    /* create a VAO for the model */
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);

    /* buffer all the attributes of the model into VBO's */
  	LL_COUNT(model->attributes, it, num_attrs);
	model->vbo_ids = (GLuint*)malloc(num_attrs * sizeof(GLuint));
    glGenBuffers(num_attrs, model->vbo_ids);

	for(it = model->attributes, i = 0; it != NULL; it = it->next, ++i) {
		TvModelAttribute* attr = (TvModelAttribute*)it;
		/* if indices, save locataion and continue */
		if(attr->type == MODEL_ATTRIBUTE_INDEX) {
			model->indices = (GLshort*)attr->data;
			continue;
		}
		/* if not indices, buffer the attribute */
        glBindBuffer(GL_ARRAY_BUFFER, model->vbo_ids[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attr->size * utarray_len(attr->data),
                attr->data, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)i, attr->size, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(i);
    }
    /* Unbind. */
    glBindVertexArray(0);
}

void tv_model_free(TvModel* model)
{
	TvModelAttributeList* it;
	for(it = model->attributes; it != NULL; it = it->next) {
		TvModelAttribute* a = (TvModelAttribute*)it;
		utarray_free(a->data);
	}
	/* TODO: free LL of attributes */
}
