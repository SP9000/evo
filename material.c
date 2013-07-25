#include "material.h"

/* IDs of loaded programs (materials) */
static GHashTable* materials;
/* IDs of all loaded shaders */
static GHashTable* fragShaders;
static GHashTable* vertShaders;
static GHashTable* geomShaders;
/* translation table from material names -> IDs */
static GHashTable* fragShaderNames;
static GHashTable* vertShaderNames;
static GHashTable* geomShaderNames;

GLuint tv_material_compile_shader(const GLchar* shader, GLuint type)
{
	GLuint s;
	GLsizei len;
	int success;
	GLchar* log;

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

GLuint tv_material_compile_program(GLuint vertShader, GLuint fragShader,
								   GLuint geomShader, char **attributes, 
								   int numAttributes)
{
	int i;
	int success;
	char* log;
	int len;
	GLuint program;

	program = glCreateProgram();
	glAttachShader(program, vertShader);
	if(geomShader != 0) {
		glAttachShader(program, geomShader);
	}
	glAttachShader(program, fragShader);

	for(i = 0; i < numAttributes; i++) {
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

GLuint tv_material_load(char* file, GLuint* program, GLuint* vert, GLuint* frag, GLuint* geom)
{       
	gpointer lup;
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

	UtilReadFile(file, &text);
	root = cJSON_Parse(text);

	/* check for errors, return NULL if encountered */
	if(!root) {
		fprintf(stderr, "Error: JSON parse error before: [%s]\n", 
			cJSON_GetErrorPtr());
		return -1;
	}

	root = root->child;
	/* make sure "material" object exists */
	if(strncmp(root->string, "material", 8) != 0) {
		fprintf(stderr, "Error: unrecognized JSON object name %s"
			" for material\n", root->string); 
		return -1;
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
	lup = (char*)g_hash_table_lookup(vertShaderNames, vertFile);
	if(lup == NULL) {
		/* no, load it */
		UtilReadFile(vertFile, &buffer);
		v = tv_material_compile_shader(buffer, GL_VERTEX_SHADER);
		/* insert into hash tables */
		g_hash_table_insert(vertShaderNames, (gpointer)vertFile, (gpointer)v);
		free(buffer);
	}
	else {
		/* yes, use saved ID */
		v = (GLuint)lup;
	}
	*vert = v;

	/* get/compile fragment shader */
	lup = (char*)g_hash_table_lookup(fragShaderNames, fragFile);
	if(lup == NULL) {
		UtilReadFile(fragFile, &buffer);
		f = tv_material_compile_shader(buffer, GL_FRAGMENT_SHADER);
		g_hash_table_insert(fragShaderNames, (gpointer)fragFile, (gpointer)f);
		free(buffer);
	}
	else {
		f = (GLuint)lup;
	}
	*frag = f;

	/* geometry shader is optional */
	if(geomFile == NULL) {
		g = 0;
	}
	else {
		/* get/compile geometry shader */
		lup = (char*)g_hash_table_lookup(geomShaderNames, geomFile);
		if(lup == NULL) {
			UtilReadFile(geomFile, &buffer);
			g = tv_material_compile_shader(buffer, GL_GEOMETRY_SHADER);
			g_hash_table_insert(geomShaderNames, (gpointer)geomFile, (gpointer)g);
			free(buffer);
		}
		else {
			g = (GLuint)lup;
		}
		*geom = g;
	}

	/* compile the shader program */
	*program = tv_material_compile_program(v, f, g, attributes, nAttributes);
	
	/* success */
	return 0;
}

void tv_material_get_uniforms(GLuint program, GLuint* model, GLuint* view, GLuint* projection)
{
	*model        = glGetUniformLocation(program, "Model");
	*view         = glGetUniformLocation(program, "View");
	*projection   = glGetUniformLocation(program, "Projection");
}
