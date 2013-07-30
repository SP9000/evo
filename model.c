#include "model.h"

TvHashTable* loaded_model_names;
TvHashTable* loaded_models;

int tv_model_init()
{
	loaded_model_names = g_hash_table_new(g_str_hash, g_str_equal);
	loaded_models = g_hash_table_new(g_direct_hash, g_direct_equal);
	return 0;
}

TvModel* tv_model_load_ply(tvchar* file)
{
	TvModelAttributeList* it;
	TvModel* model;
	FILE* fp;
	
	int i;
	int j;

	model = (TvModel*)g_hash_table_lookup(loaded_model_names, file);
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
				to_add = (TvModelAttributeList*)malloc(sizeof(TvModelAttributeList));
				to_add->attr.data = g_array_new(false, false, c);
				LL_PREPEND(model->attributes, to_add);
			}
		}
		else if(strncmp(a, "property", 9) == 0) {
			continue;
		}
	}

	/* read all the attributes in - everything but faces are floats */
	for(it = model->attributes; it != NULL; it = g_list_next(model->attributes)) {
		TvModelAttribute* ma = (TvModelAttribute*)it->data;
		for(i = 0; i < ma->data->len; ++i) {
			float f_buff[6];
			int i_buff[4];
			int read;
			if(ma->type != MODEL_ATTRIBUTE_INDEX) {
				read = fscanf(fp, "%f %f %f %f %f %f", 
					&f_buff[0], &f_buff[1], &f_buff[2], &f_buff[3], &f_buff[4], &f_buff[5]);
				for(j = 0; j < read; ++j) {
					g_array_append_val(ma->data, f_buff[j]);
				}
			}
			else {
				read = fscanf(fp, "%d %d %d %d",
					&i_buff[0], &i_buff[1], &i_buff[2], &i_buff[3], &i_buff[4], &i_buff[5]);
				for(j = 1; j < read; ++j) {
					g_array_append_val(ma->data, i_buff[j]);
				}
			}
		}
	}
	g_hash_table_insert(loaded_model_names, file, (gpointer)model);
	return model;
}

void tv_model_add_attribute(TvModel* model, tvuint attribute)
{
	
}
void tv_model_buffer_attribute(TvModel* model, tvuint attribute, tvfloat* buffer)
{

}

TvAABB tv_model_get_aabb(TvModel* model)
{
	TvAABB aabb;
	return aabb;
}

TvModel* tv_model_new()
{
	return (TvModel*)malloc(sizeof(TvModel*));
}

void tv_model_optimize(TvModel* model)
{
	int i;
	int num_attrs;
	GList* it;

    /* create a VAO for the model */
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);

    /* buffer all the attributes of the model into VBO's */
	num_attrs = g_list_length(model->attributes);
	model->vbo_ids = (GLuint*)malloc(num_attrs * sizeof(GLuint));
    glGenBuffers(num_attrs, model->vbo_ids);

	for(it = model->attributes; it != NULL; it = g_list_next(it)) {
		TvModelAttribute* attr = (TvModelAttribute*)it->data;
		/* if indices, save locataion and continue */
		if(attr->type == MODEL_ATTRIBUTE_INDEX) {
			model->indices = (GLshort*)attr->data->data;
			continue;
		}
		/* if not indices, buffer the attribute */
        glBindBuffer(GL_ARRAY_BUFFER, model->vbo_ids[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attr->size * attr->data->len,
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
