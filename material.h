#ifndef _MATERIAL_H
#define _MATERIAL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "tv_alloc.h"
#include "cJSON.h"
#include "util.h"

typedef struct tagTvMaterial {
	tvchar* name;
	GLuint program;
	GLuint model_mat;
	GLuint view_mat;
	GLuint projection_mat;

	TvHashHandle hh;
}TvMaterial;

typedef struct tagMaterialShader {
	tvchar* name;
	tvuint id;
	TvHashHandle hh;
}TvMaterialShader;

GLuint tv_material_compile_shader(GLchar* shader, GLuint type);
GLuint tv_material_compile_program(GLuint vert_shader, GLuint frag_shader,
								   GLuint geom_shader, tvchar **attributes, 
								   tvuint num_attributes);
TvMaterial* tv_material_load(char* file);
void tv_material_get_uniforms(GLuint program, GLuint* model, GLuint* view, GLuint* projection);

#ifdef __cplusplus
}
#endif
#endif