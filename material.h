#ifndef _MATERIAL_H
#define _MATERIAL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "cJSON.h"
#include "util.h"

typedef struct tagTvMaterial {
	GLuint program;
	GLuint model_mat;
	GLuint view_mat;
	GLuint projection_mat;
}TvMaterial;

GLuint tv_material_compile_shader(GLchar* shader, GLuint type);
GLuint tv_material_compile_program(GLuint vertShader, GLuint fragShader,
								   GLuint geomShader, char **attributes, 
								   int numAttributes);
TvMaterial* tv_material_load(char* file);
void tv_material_get_uniforms(GLuint program, GLuint* model, GLuint* view, GLuint* projection);

#ifdef __cplusplus
}
#endif
#endif