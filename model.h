#ifndef MODEL_H
#define MODEL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "component.h"

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
tv_model* tv_model_new();
void tv_model_load_ply(tv_model *model, tvchar* file);
void tv_model_add_attribute(tv_model* model, tvuint attribute);
void tv_model_buffer_attribute(tv_model* model, tvuint attribute, TvArray* buffer);
void tv_model_set_attribute(tv_model *model, tvuint attribute_id, tvuint vertex, GLfloat *data);

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
