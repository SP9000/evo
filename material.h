#ifndef _MATERIAL_H
#define _MATERIAL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "component.h"
#include "types.h"
#include "tv_alloc.h"
#include "cJSON.h"
#include "util.h"

COMPONENT(tv_material, tv_component) 
	tvchar* name;
	GLuint program;
	GLuint model_mat;
	GLuint view_mat;
	GLuint projection_mat;

	TvHashHandle hh;
ENDCOMPONENT(tv_material)

typedef struct tagMaterialShader {
	tvchar* name;
	tvuint id;
	TvHashHandle hh;
}TvMaterialShader;

GLuint tv_material_compile_shader(GLchar* shader, GLuint type);
GLuint tv_material_compile_program(GLuint vert_shader, GLuint frag_shader,
								   GLuint geom_shader, tvchar **attributes, 
								   tvuint num_attributes);
void tv_material_get_uniforms(GLuint program, GLuint* model, GLuint* view, GLuint* projection);
void tv_material_load(tv_material *mat, const char* file);

#ifdef __cplusplus
}
#endif
#endif