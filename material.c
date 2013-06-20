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

int Material_Init()
{
    materials = g_hash_table_new(g_int_hash, g_direct_equal);
    fragShaders = g_hash_table_new(g_int_hash, g_int_equal);
    fragShaderNames = g_hash_table_new(g_str_hash, g_int_equal);
    vertShaders = g_hash_table_new(g_int_hash, g_int_equal);
    vertShaderNames = g_hash_table_new(g_str_hash, g_int_equal);
    geomShaders = g_hash_table_new(g_int_hash, g_int_equal);
    geomShaderNames = g_hash_table_new(g_str_hash, g_int_equal);
    return 0;
}

Material* Material_Load(const char* matFile)
{ 
    Material* m;
    gpointer lup;
    GLuint v, f, g;
    int i;

    cJSON* root;
    cJSON* json;

    int nAttributes;
    char** attributes;
    char* text;
    char* buffer;
    char* vertFile = NULL;
    char* fragFile = NULL;
    char* geomFile = NULL;


    UtilReadFile(matFile, &text);
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
                        puts(vertFile);
                    }
                    else if(strncmp(json->string, "frag", 4) == 0) {
                        fragFile = json->valuestring;
                        puts(fragFile);
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
                    puts(attr->valuestring);
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

    m = (Material*)malloc(sizeof(Material));

    /* get/compile shaders - start with the vertex shader */
    lup = (char*)g_hash_table_lookup(vertShaderNames, vertFile);
    if(lup == NULL) {
        /* no, load it */
        UtilReadFile(vertFile, &buffer);
        v = Material_CompileShader(buffer, GL_VERTEX_SHADER);
        /* insert into hash tables */
        g_hash_table_insert(vertShaderNames, (gpointer)vertFile, (gpointer)v);
        free(buffer);
    }
    else {
        /* yes, use saved ID */
        v = (GLuint)lup;
    }
    m->vert = v;

    /* get/compile fragment shader */
    lup = (char*)g_hash_table_lookup(fragShaderNames, fragFile);
    if(lup == NULL) {
        UtilReadFile(fragFile, &buffer);
        f = Material_CompileShader(buffer, GL_FRAGMENT_SHADER);
        g_hash_table_insert(fragShaderNames, (gpointer)fragFile, (gpointer)f);
        free(buffer);
    }
    else {
        f = (GLuint)lup;
    }
    m->frag = f;

    /* geometry shader is optional */
    if(geomFile == NULL) {
        g = 0;
    }
    else {
        /* get/compile geometry shader */
        lup = (char*)g_hash_table_lookup(geomShaderNames, geomFile);
        if(lup == NULL) {
            UtilReadFile(geomFile, &buffer);
            g = Material_CompileShader(buffer, GL_GEOMETRY_SHADER);
            g_hash_table_insert(geomShaderNames, (gpointer)geomFile, (gpointer)g);
            free(buffer);
        }
        else {
            g = (GLuint)lup;
        }
        m->geom = g;
    }

    /* compile the shader program */
    m->program = Material_CompileProgram(v, f, g, attributes, nAttributes);

    /* get matrix uniform locations */
    m->modelMatrixID        = glGetUniformLocation(m->program, "Model");
    m->viewMatrixID         = glGetUniformLocation(m->program, "View");
    m->projectionMatrixID   = glGetUniformLocation(m->program, "Projection");

    /* no textures for now */
    m->texture.id = 0;

    return m;
}

GLuint Material_CompileShader(const GLchar* shader, GLuint type) 
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
        log = malloc(sizeof(GLchar) * len);
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

GLuint Material_CompileProgram(GLuint vertShader, GLuint fragShader, GLuint geomShader,
      char **attributes, int numAttributes)
{
    int i;
    int success;
    char* log;
    int len;
    GLuint temp;
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
        log = malloc(sizeof(GLchar) * len);
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

void Material_AddTexture(Material* m, int w, int h, Texel* data)
{
    GLuint tex;

    /* generate a texture handle for this texture */
    glGenTextures(1, &tex);
    m->texture.id = tex;
    /* get the location of the sampler for the texture in the shader program*/
    m->texture.loc = glGetUniformLocation(m->program, "tex");

    ++m->numTextures;

    /* bind the texture, set its data, and set up its parameters. */
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
            GL_UNSIGNED_BYTE, (GLvoid*)data);
}

void Material_SetTexture(Material* m, Texture* t)
{
    m->textures.id = t->id;
    m->textures.loc = glGetUniformLocation(m->program, "tex");
}

