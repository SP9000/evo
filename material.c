#include "material.h"

/* IDs of loaded programs (materials) */
static tv_material* loaded_materials;
/* translation table from material names -> IDs */
static TvMaterialShader* loaded_fragment_shaders;
static TvMaterialShader* loaded_vertex_shaders;
static TvMaterialShader* loaded_geometry_shaders;

static void tv_material_init_uniform_buffer_(tv_material *material);

#if 0
/**
 * This structure is used to store information about a material as it is being
 * built (before it is optimized and stored into the material component).
 */
typedef struct tv_material_effect_info {
	tvchar* vertex_shader_prefix;
	tvchar* vertex_shader_main_prefix;
	tvchar* vertex_shader_main_suffix;
	tvchar* vertex_shader_suffix;
	tvchar* fragment_shader_prefix;
	tvchar* fragment_shader_main_prefix;
	tvchar* fragment_shader_main_suffix;
	tvchar* fragment_shader_suffix;
	tvchar* geometry_shader_prefix;
	tvchar* geometry_shader_main_prefix;
	tvchar* geometry_shader_main_suffix;
	tvchar* geometry_shader_suffix;
	tvchar name[32];
	TvHashHandle hh;
}tv_material_effect_info;

/* table of the materials that are in the process of being built */
static tv_material_effect_info* effects_info_table;
const tvchar* base_vertex_shader_prefix = 
	"#version 400\n"
	"#define MAX_NUM_LIGHTS 100\n";
const tvchar* base_vertex_shader_main = 
	"void main() {\n";
const tvchar* base_vertex_shader_main_end = 
	"}\n";
const tvchar* base_vertex_shader_suffix = 
	"";

const tvchar* base_fragment_shader_prefix = 
	"#version 400\n"
	"#define MAX_NUM_LIGHTS 100\n";
const tvchar* base_fragment_shader_main = 
	"void main() {\n";
const tvchar* base_fragment_shader_main_end = 
	"}\n";
const tvchar* base_fragment_shader_suffix = 
	"";

const tvchar* base_geometry_shader_prefix = 
	"#version 400\n"
	"#define MAX_NUM_LIGHTS 100\n";
const tvchar* base_geometry_shader_main = 
	"void main() {\n";
const tvchar* base_geometry_shader_main_end = 
	"}\n";
const tvchar* base_geometry_shader_suffix = 
	"";
#endif

COMPONENT_NEW(tv_material, tv_component)
END_COMPONENT_NEW(tv_material)

COMPONENT_START(tv_material)
	tv_material_init_uniform_buffer_(self);
	self->lit = 1;
END_COMPONENT_START

COMPONENT_UPDATE(tv_material)
END_COMPONENT_UPDATE

void tv_material_init_uniform_buffer_(tv_material *material)
{
	// the binding point must be smaller than GL_MAX_UNIFORM_BUFFER_BINDINGS
	GLuint binding_point = TV_MATERIAL_BUFFER_BINDING_POINT;
	GLuint bindingPoint = 1, blockIndex;
	float myFloats[8] = {1.0, 0.0, 0.0, 1.0, 0.4, 0.0, 0.0, 1.0};
	blockIndex = glGetUniformBlockIndex(material->program, "AttributeBlock");
	glUniformBlockBinding(material->program, blockIndex, bindingPoint);
	
	glGenBuffers(1, &material->buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, material->buffer);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(myFloats), myFloats, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, material->buffer);
}

GLuint tv_material_compile_shader(const GLchar* shader, GLuint type)
{
	GLuint s;
	GLsizei len;
	GLchar* log;
	int success;

	s = glCreateShader(type);
	glShaderSource(s, 1, &shader, NULL);
	glCompileShader(s);
	glGetShaderiv(s, GL_COMPILE_STATUS, &success);
	glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
	if(len > 0) {
		log = (char*)malloc(sizeof(GLchar) * len);
		glGetShaderInfoLog(s, len, &len, log);
		puts(log);
		free(log);
	}
	if(success == GL_FALSE) {
		if(type == GL_VERTEX_SHADER) {
			fprintf(stderr, "Error: vertex shader was not compiled successfully.\n");
		}
		else if(type == GL_FRAGMENT_SHADER) {
			fprintf(stderr, "Error: fragment shader was not compiled successfully.\n");
		}
		else if(type == GL_GEOMETRY_SHADER) {
			fprintf(stderr, "Error: geometry shader was not compiled successfully.\n");
		}
		else {
			fprintf(stderr, "Error: shader was not compiled successfully.\n");
		}
		return 0;
	}
	return s;
}

GLuint tv_material_compile_program(GLuint vert_shader, GLuint frag_shader,
								   GLuint geom_shader, tvchar **attributes, 
								   tvuint num_attributes)
{
	tvuint i;
	tvint len;
	tvint success;
	tvchar* log;
	GLuint program;

	program = glCreateProgram();
	glAttachShader(program, vert_shader);
	if(geom_shader != 0) {
		glAttachShader(program, geom_shader);
	}
	glAttachShader(program, frag_shader);

	for(i = 0; i < num_attributes; i++) {
		glBindAttribLocation(program, i, attributes[i]);
	}

	glLinkProgram(program);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
	if(len > 0) {
		log = (char*)malloc(sizeof(GLchar) * len);
		glGetProgramInfoLog(program, len, &len, log);
		puts(log);
		free(log);
	}

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(success == GL_FALSE) {
		fprintf(stderr, "Error: shader could not be linked successfully.\n");
		return 0;
	}
	return program;
}

tvint tv_material_get_uniform(tv_material *material, tvchar *name)
{
	return glGetUniformLocation(material->program, name);
}

void tv_material_load(tv_material *mat, const char* file)
{       
	TvMaterialShader* lup;
	GLuint v;
	GLuint f;
	GLuint g;
	int i;

	int nAttributes;
	char** attributes;
	char* text;
	char* buffer;
	char* vertFile = NULL;
	char* fragFile = NULL;
	char* geomFile = NULL;

	cJSON* root = NULL;
	cJSON* json = NULL;
	tv_material *lup_mat = NULL;

	HASH_FIND_PTR(loaded_materials, file, lup_mat);
	if(lup_mat) {
		mat->program = lup_mat->program;
		mat->name = lup_mat->name;
		mat->projection_mat = lup_mat->projection_mat;
		mat->view_mat = lup_mat->view_mat;
		mat->model_mat = lup_mat->model_mat;
	}

	UtilReadFile(file, &text);
	root = cJSON_Parse(text);

	/* check for errors, return NULL if encountered */
	if(!root) {
		fprintf(stderr, "Error: JSON parse error before: [%s]\n", 
			cJSON_GetErrorPtr());
		return;
	}

	root = root->child;
	/* make sure "material" object exists */
	if(strncmp(root->string, "material", 8) != 0) {
		fprintf(stderr, "Error: unrecognized JSON object name %s"
			" for material\n", root->string); 
		return;
	}
	/* get all material information */
	else {
		root = root->child;
		while(root != NULL) {
			json = root;
			/* get the first child object of the material */
			if(strncmp(json->string, "shaders", 7) == 0) {
				/* get shaders */
				json = json->child;
				do {
					if(strncmp(json->string, "vert", 4) == 0) {
						vertFile = json->valuestring;
					}
					else if(strncmp(json->string, "frag", 4) == 0) {
						fragFile = json->valuestring;
					}
					else if(strncmp(json->string, "geom", 4) == 0) {
						geomFile = json->valuestring;
					}
					json = json->next;
				}while(json != NULL);
			}
			else if(strncmp(json->string, "attributes", 10) == 0) {
				/* get attributes */
				nAttributes = cJSON_GetArraySize(json);
				attributes = (char**)malloc(nAttributes * sizeof(char*));
				for(i = 0; i < nAttributes; ++i) {
					cJSON* attr = cJSON_GetArrayItem(json, i);
					attributes[i] = (char*)malloc((strlen(attr->valuestring)+1) * 
						sizeof(char));
					strncpy(attributes[i], attr->valuestring, 
						strlen(attr->valuestring)+1);
				}
			}
			root = root->next;
		}
	}
	cJSON_Delete(root);
	cJSON_Delete(json);

	/* get/compile shaders - start with the vertex shader */
	HASH_FIND_PTR(loaded_vertex_shaders, vertFile, lup);
	if(lup == NULL) {
		/* no, load it */
		UtilReadFile(vertFile, &buffer);
		v = tv_material_compile_shader(buffer, GL_VERTEX_SHADER);
		/* insert into hash tables */
		lup = (TvMaterialShader*)tv_alloc(sizeof(TvMaterialShader));
		lup->name = vertFile;
		lup->id = v;
		HASH_ADD_PTR(loaded_vertex_shaders, name, lup);
		free(buffer);
	}
	else {
		/* yes, use saved ID */
		v = (GLuint)(lup->id);
	}

	/* get/compile fragment shader */
	HASH_FIND_PTR(loaded_fragment_shaders, file, lup);
	if(lup == NULL) {
		UtilReadFile(fragFile, &buffer);
		f = tv_material_compile_shader(buffer, GL_FRAGMENT_SHADER);
		lup = (TvMaterialShader*)tv_alloc(sizeof(TvMaterialShader));
		lup->name = vertFile;
		lup->id = f;
		HASH_ADD_PTR(loaded_fragment_shaders, name, lup);
		free(buffer);
	}
	else {
		f = (GLuint)(lup->id);
	}

	/* geometry shader is optional */
	if(geomFile == NULL) {
		g = 0;
	}
	else {
		/* get/compile geometry shader */
		HASH_FIND_PTR(loaded_geometry_shaders, file, lup);
		if(lup == NULL) {
			UtilReadFile(geomFile, &buffer);
			g = tv_material_compile_shader(buffer, GL_GEOMETRY_SHADER);
			lup = (TvMaterialShader*)tv_alloc(sizeof(TvMaterialShader));
			lup->name = geomFile;
			lup->id = g;
			HASH_ADD_PTR(loaded_geometry_shaders, name, lup);
			free(buffer);
		}
		else {
			g = (GLuint)(lup->id);
		}
	}

	/* compile the shader program */
	mat->name = (tvchar*)file;
	mat->program = tv_material_compile_program(v, f, g, attributes, nAttributes);

	tv_material_get_uniforms(mat->program, &mat->model_mat, &mat->view_mat, &mat->projection_mat);

	HASH_ADD_PTR(loaded_materials, name, mat);
}

#if 0
void tv_material_optimize(tv_material *material)
{
	tv_material_effect **effect;
	tv_material_effect_info *effect_info;
	GLuint vert_shader;
	GLuint frag_shader;
	GLuint geom_shader;

	tv_string *vert_src;
	tv_string *vert_prefix;
	tv_string *vert_main_prefix;
	tv_string *vert_main_suffix;
	tv_string *vert_suffix;

	tv_string *frag_src;
	tv_string *frag_prefix;
	tv_string *frag_main_prefix;
	tv_string *frag_main_suffix;
	tv_string *frag_suffix;

	tv_string *geom_src;
	tv_string *geom_prefix;
	tv_string *geom_main_prefix;
	tv_string *geom_main_suffix;
	tv_string *geom_suffix;

	utstring_new(vert_src);
	utstring_new(vert_prefix);
	utstring_new(vert_main_prefix);
	utstring_new(vert_main_suffix);
	utstring_new(vert_suffix);
	utstring_new(frag_src);
	utstring_new(frag_prefix);
	utstring_new(frag_main_prefix);
	utstring_new(frag_main_suffix);
	utstring_new(frag_suffix);
	utstring_new(geom_src);
	utstring_new(geom_prefix);
	utstring_new(geom_main_prefix);
	utstring_new(geom_main_suffix);
	utstring_new(geom_suffix);
	for(effect = (tv_material_effect**)utarray_front(material->effects);
		effect != NULL;
		effect = (tv_material_effect**)utarray_next(material->effects, effect)) {
			HASH_FIND_STR(effects_info_table, (*effect)->name, effect_info);
			utstring_printf(vert_prefix, effect_info->vertex_shader_prefix);
			utstring_printf(vert_main_prefix, effect_info->vertex_shader_main_prefix);
			utstring_printf(vert_main_suffix, effect_info->vertex_shader_main_suffix);
			utstring_printf(vert_suffix, effect_info->vertex_shader_suffix);
			utstring_printf(frag_prefix, effect_info->fragment_shader_prefix);
			utstring_printf(frag_main_prefix, effect_info->fragment_shader_main_prefix);
			utstring_printf(frag_main_suffix, effect_info->fragment_shader_main_suffix);
			utstring_printf(frag_suffix, effect_info->fragment_shader_suffix);
			utstring_printf(geom_prefix, effect_info->geometry_shader_prefix);
			utstring_printf(geom_main_prefix, effect_info->geometry_shader_main_prefix);
			utstring_printf(geom_main_suffix, effect_info->geometry_shader_main_suffix);
			utstring_printf(geom_suffix, effect_info->geometry_shader_suffix);
	}
	utstring_concat(vert_src, vert_prefix);
	utstring_concat(vert_src, vert_main_prefix);
	utstring_concat(vert_src, vert_main_suffix);
	utstring_concat(vert_src, vert_suffix);
	utstring_concat(frag_src, frag_prefix);
	utstring_concat(frag_src, frag_main_prefix);
	utstring_concat(frag_src, frag_main_suffix);
	utstring_concat(frag_src, frag_suffix);
	utstring_concat(geom_src, geom_prefix);
	utstring_concat(geom_src, geom_main_prefix);
	utstring_concat(geom_src, geom_main_suffix);
	utstring_concat(geom_src, geom_suffix);
	material->program = tv_material_compile_program(vert_shader, frag_shader,
								   geom_shader, tvchar **attributes, 
								   num_attributes);

}
#endif

void tv_material_get_uniforms(GLuint program, GLuint* model, GLuint* view, GLuint* projection)
{
	*model        = glGetUniformLocation(program, "Model");
	*view         = glGetUniformLocation(program, "View");
	*projection   = glGetUniformLocation(program, "Projection");
}
