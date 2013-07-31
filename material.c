#include "material.h"

/* IDs of loaded programs (materials) */
static TvMaterial* loaded_materials;
/* translation table from material names -> IDs */
static TvMaterialShader* loaded_fragment_shaders;
static TvMaterialShader* loaded_vertex_shaders;
static TvMaterialShader* loaded_geometry_shaders;

int tv_material_init()
{
	return 0;
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
		exit(EXIT_FAILURE);
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
		exit(EXIT_FAILURE);
	}
	return program;

}

TvMaterial* tv_material_load(char* file)
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
	TvMaterial* mat = NULL;

	cJSON* root = NULL;
	cJSON* json = NULL;

	HASH_FIND_PTR(loaded_materials, file, mat);
	if(mat) {
		return mat;
	}

	UtilReadFile(file, &text);
	root = cJSON_Parse(text);

	/* check for errors, return NULL if encountered */
	if(!root) {
		fprintf(stderr, "Error: JSON parse error before: [%s]\n", 
			cJSON_GetErrorPtr());
		return NULL;
	}

	root = root->child;
	/* make sure "material" object exists */
	if(strncmp(root->string, "material", 8) != 0) {
		fprintf(stderr, "Error: unrecognized JSON object name %s"
			" for material\n", root->string); 
		return NULL;
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
	mat = (TvMaterial*)malloc(sizeof(TvMaterial));
	mat->name = file;
	mat->program = tv_material_compile_program(v, f, g, attributes, nAttributes);
	HASH_ADD_PTR(loaded_materials, name, mat);
	return mat;
}

void tv_material_get_uniforms(GLuint program, GLuint* model, GLuint* view, GLuint* projection)
{
	*model        = glGetUniformLocation(program, "Model");
	*view         = glGetUniformLocation(program, "View");
	*projection   = glGetUniformLocation(program, "Projection");
}
