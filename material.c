#include "material.h"
#include "tv_draw.h"

/* IDs of loaded programs (materials) */
static tv_material* loaded_materials;
/* translation table from material names -> IDs */
static TvMaterialShader* loaded_fragment_shaders;
static TvMaterialShader* loaded_vertex_shaders;
static TvMaterialShader* loaded_geometry_shaders;

static void tv_material_init_uniform_buffer_(tv_material *material);
static GLuint tv_material_compile_shader(const GLchar* shader, GLuint type);

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
	self->num_passes = 0;
END_COMPONENT_NEW(tv_material)

COMPONENT_START(tv_material)
END_COMPONENT_START

COMPONENT_UPDATE(tv_material)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(tv_material)
END_COMPONENT_DESTROY

#if 0
void tv_material_add_ubo(tv_material *material, const tvchar* name)
{
	/* retrieve the binding point for the new UBO */
	GLuint binding_point = material->num_ubos + 1;
	/* get the block index of the UBO by its name */
	GLuint block_index = glGetUniformBlockIndex(material->program, name);
	glUniformBlockBinding(material->program, block_index, binding_point);
	glGenBuffers(1, &material->ubos[material->num_ubos]);
	++material->num_ubos;
}

tvint tv_material_get_uniform_block(tv_material *material, const tvchar *name)
{
	tvuint i;
	tvuint len = strlen(name);
	/* linearly search for the UBO by name */
	for(i = 0; i < material->num_ubos; ++i) {
		if(strncmp(material->ubo_names[i], name, len) == 0) {
			return material->ubos[i];
		}
	}
	/* no UBO of the specified name */
	return -1;
}

void tv_material_set_ubo_attribute(tv_material *material, const tvchar* block, void *data, tvuint size)
{
	tvuint id = tv_material_get_uniform_block(material, block);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
}
#endif

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
		log = (char*)tv_alloc(sizeof(GLchar) * len);
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
		log = (char*)tv_alloc(sizeof(GLchar) * len);
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
	/* TODO: obviously this is a horrible hack.
	 * this function shouldn't exist anyway */
	return glGetUniformLocation(material->passes[0]->program, name);
}

/*****************************************************************************/
/* local functions */
tv_material_program tv_material_compile_shaders(tvchar* vert_file, tvchar* frag_file, tvchar* geom_file)
{
	tv_material_program program;
	TvMaterialShader* lup;
	GLuint v, f, g;
	tvchar* buffer;

	/* get/compile shaders - start with the vertex shader */
	HASH_FIND_PTR(loaded_vertex_shaders, vert_file, lup);
	if(lup == NULL) {
		/* no, load it */
		UtilReadFile(vert_file, &buffer);
		v = tv_material_compile_shader(buffer, GL_VERTEX_SHADER);
		/* insert into hash tables */
		lup = (TvMaterialShader*)tv_alloc(sizeof(TvMaterialShader));
		lup->name = vert_file;
		lup->id = v;
		HASH_ADD_PTR(loaded_vertex_shaders, name, lup);
		free(buffer);
	}
	else {
		/* yes, use saved ID */
		v = (GLuint)(lup->id);
	}

	/* get/compile fragment shader */
	HASH_FIND_PTR(loaded_fragment_shaders, frag_file, lup);
	if(lup == NULL) {
		UtilReadFile(frag_file, &buffer);
		f = tv_material_compile_shader(buffer, GL_FRAGMENT_SHADER);
		lup = (TvMaterialShader*)tv_alloc(sizeof(TvMaterialShader));
		lup->name = frag_file;
		lup->id = f;
		HASH_ADD_PTR(loaded_fragment_shaders, name, lup);
		free(buffer);
	}
	else {
		f = (GLuint)(lup->id);
	}

	/* geometry shader is optional */
	g = 0;
	if(geom_file != NULL) {
		/* get/compile geometry shader */
		HASH_FIND_PTR(loaded_geometry_shaders, geom_file, lup);
		if(lup == NULL) {
			UtilReadFile(geom_file, &buffer);
			g = tv_material_compile_shader(buffer, GL_GEOMETRY_SHADER);
			lup = (TvMaterialShader*)tv_alloc(sizeof(TvMaterialShader));
			lup->name = geom_file;
			lup->id = g;
			HASH_ADD_PTR(loaded_geometry_shaders, name, lup);
			free(buffer);
		}
		else {
			g = (GLuint)(lup->id);
		}
	}
	program.fragment = f;
	program.vertex = v;
	program.geometry = g;
	return program;
}

/**
 * Parse the shaders from the given JSON node and return it.
 */
tv_material_program tv_material_load_shaders(cJSON* json) 
{
	char* vert_file = NULL;
	char* frag_file = NULL;
	char* geom_file = NULL;

	/* get shaders */
	do {
		if(strncmp(json->string, "vert", 4) == 0) {
			vert_file = json->valuestring;
		}
		else if(strncmp(json->string, "frag", 4) == 0) {
			frag_file = json->valuestring;
		}
		else if(strncmp(json->string, "geom", 4) == 0) {
			geom_file = json->valuestring;
		}
		json = json->next;
	}while(json != NULL);
	return tv_material_compile_shaders(vert_file, frag_file, geom_file);
}

/**
 * Parse the attributes from the given JSON node and return them.
 */
tvchar** tv_material_load_attributes(cJSON* json, tvuint* /* OUT */ num)
{
	tvuint i, num_attributes = 0;
	tvchar** attributes;

	/* get attributes */
	num_attributes = cJSON_GetArraySize(json);
	attributes = (char**)tv_alloc(num_attributes * sizeof(char*));

	for(i = 0; i < num_attributes; ++i) {
		cJSON* attr = cJSON_GetArrayItem(json, i);
		/* allocate room for the attribute name */
		attributes[i] = (char*)tv_alloc((strlen(attr->valuestring)+1) * 
			sizeof(char));
		/* copy the name of the attribute */
		strncpy(attributes[i], attr->valuestring, 
			strlen(attr->valuestring)+1);
	}
	*num = num_attributes;
	return attributes;
}

/**
 * Parse one pass from the given JSON node and return it.
 */
tv_material_pass* tv_material_load_pass(cJSON* json) 
{
	tv_material_program shaders;
	tvchar** attributes;
	tvuint num_attributes = 0;
	tv_material_pass* pass = NULL;

	/* check if the pass has already been loaded */
	//tv_material_pass* pass = tv
	if(pass == NULL) {
		pass = (tv_material_pass*)tv_alloc(sizeof(tv_material_pass));
	}

	/* load the pass */
	do {
		if(strncmp(json->string, "shaders", 7) == 0) {
			/* load the shaders */
			shaders = tv_material_load_shaders(json->child);
		}
		else if(strncmp(json->string, "attributes", 10) == 0) {
			/* load the attributes */
			attributes = tv_material_load_attributes(json, &num_attributes);
		}
		json = json->next;
	} while(json != NULL);

	/* compile the shader program and get the camera uniforms for it. */
	pass->program = tv_material_compile_program(shaders.vertex, shaders.fragment, shaders.geometry, attributes, num_attributes);
	pass->attributes = attributes;
	pass->num_attributes = num_attributes;
	tv_material_get_uniforms(pass->program, &pass->modelview_mat, &pass->projection_mat);

	/* add the pass to the pass table and return it */
	return pass;
}

void tv_material_load(tv_material *mat, const char* file)
{       
	tvchar* text;
	cJSON* root = NULL;
	cJSON* json = NULL;
	tv_material *lup_mat = NULL;

	/* check if the material is already loaded, if it is copy it and return. */
	HASH_FIND_PTR(loaded_materials, file, lup_mat);
	if(lup_mat) {
		/* TOOD: will only work if materials are never deleted */
		mat = lup_mat;
		return;
	}
	/* if no material was given, create a new material */
	if(mat == NULL) {
		mat = tv_material_new();
	}

	mat->num_passes = 0;

	/* material is not loaded, load file and parse it */
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
		tv_warning("unrecognized JSON object name %s"
			" for material\n", root->string); 
	}
	else {
		/* read the material file */
		root = root->child;
		while(root != NULL) {
			if(strncmp(root->string, "passes", sizeof("passes")) == 0) {
				/* read all the material's passes */
				for(json = root->child; json != NULL; json = json->next) {
					tv_material_pass* pass = tv_material_load_pass(json->child);
					tv_material_add_pass(mat, pass);
				}
			}
			/* unrecognized node - print warning */
			else {
				tv_warning("Unrecognized JSON object name %s "
					"for material\n", root->string);
			}
			/* move to the next JSON object */
			root = root->next;
		}
		/* add the material to the table of loaded materials to prevent reloading */
		mat->name = (tvchar*)file;
		HASH_ADD_PTR(loaded_materials, name, mat);
	}
	/* cleanup */
	free(text);
	cJSON_Delete(root);
	cJSON_Delete(json);
}

void METHOD(tv_material, add_pass, tv_material_pass *pass)
{
	if(self->num_passes >= TV_MATERIAL_MAX_PASSES) {
		tv_warning("maximum number of material passes exceeded.");
		return;
	}
	self->passes[self->num_passes] = pass;
	++self->num_passes;
}

void METHOD(tv_material, use_pass, tvuint pass_idx)
{
	tv_material_pass* pass;
	/* make sure pass index is valid */
	if(self->num_passes <= pass_idx) {
		return;
	}

	pass = self->passes[pass_idx];
	/* use the shader program */
	glUseProgram(pass->program);

	/* update camera if needed */
	glUniformMatrix4fv(pass->modelview_mat, 1, GL_FALSE, 
		tv_mat4x4_to_array(&main_cam->modelview_mat));
	glUniformMatrix4fv(pass->projection_mat, 1, GL_FALSE, 
		tv_mat4x4_to_array(&main_cam->projection_mat));
	/* TODO: update all uniforms */
}

void METHOD(tv_material, do_pass, tvuint pass_index, tv_model* model) 
{
	tv_material_use_pass(self, pass_index);
	glBindVertexArray(model->vao);
	/* render arrays if there are no indices */
	if(utarray_len(model->indices) > 0) {
		tv_draw_elements(model->primitive, utarray_len(model->indices),
			GL_UNSIGNED_SHORT, 0);
	}
	/* if there are indices, render elements */
	else {
		tv_draw_arrays(model->primitive, 0, utarray_len(model->vertices));
	}
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

void tv_material_get_uniforms(GLuint program, GLuint* modelview, GLuint* projection)
{
	printf("%d\n", glGetError());
	*projection   = glGetUniformLocation(program, "Projection");
	*modelview    = glGetUniformLocation(program, "ModelView");
	printf("%d\n", glGetError());
	printf("%d %d", *modelview, *projection);
}
