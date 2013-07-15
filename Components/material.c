/*****************************************************************************/
/* material.c                                                                */
/* The material component                                                    */
/* Contains the definition of the material type and functions to load/manage */
/* materials. Platform dependent code (OpenGL) is used in this system.       */
/* Bryce Wilson                                                              */
/* created: June 18, 2013                                                    */
/*****************************************************************************/

#ifdef OPENGL_2
typedef struct tagTexture {
    /* the ID of the texture itself (as given by glGenTextures) */
    GLuint id;
    /* the location of the texture in the shader program it's attacked to */
    GLuint loc;
    /* the handle of the sampler used this texture (given by glGenSamplers). */
    GLuint sampler;
}Texture;

static GLuint CompileShader(const GLchar* shader, GLuint type);
/* function to compile program from given shaders */
static GLuint CompileProgram(GLuint vertShader, GLuint fragShader, GLuint geomShader, 
        char **attributes, int numAttributes);
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


/* compile a shader of the given type */
GLuint CompileShader(const GLchar* shader, GLuint type) 
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

/* compile a shader program from the given shaders and attributes */
GLuint CompileProgram(GLuint vertShader, GLuint fragShader, GLuint geomShader, 
    char **attributes, int numAttributes) 
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

COMPONENT Material {
    public char* file;

    public GLuint frag;
    public GLuint vert;
    public GLuint geom;
    public GLuint program;

    public GLuint modelMatrixID;
    public GLuint viewMatrixID;
    public GLuint projectionMatrixID;
    public Texture texture;

    void Load(char *file) 
    {
        gpointer lup;
        GLuint v;
        GLuint f;
        GLuint g;
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
            v = CompileShader(buffer, GL_VERTEX_SHADER);
            /* insert into hash tables */
            g_hash_table_insert(vertShaderNames, (gpointer)vertFile, (gpointer)v);
            free(buffer);
        }
        else {
            /* yes, use saved ID */
            v = (GLuint)lup;
        }
        self->vert = v;

        /* get/compile fragment shader */
        lup = (char*)g_hash_table_lookup(fragShaderNames, fragFile);
        if(lup == NULL) {
            UtilReadFile(fragFile, &buffer);
            f = CompileShader(buffer, GL_FRAGMENT_SHADER);
            g_hash_table_insert(fragShaderNames, (gpointer)fragFile, (gpointer)f);
            free(buffer);
        }
        else {
            f = (GLuint)lup;
        }
        self->frag = f;

        /* geometry shader is optional */
        if(geomFile == NULL) {
            g = 0;
        }
        else {
            /* get/compile geometry shader */
            lup = (char*)g_hash_table_lookup(geomShaderNames, geomFile);
            if(lup == NULL) {
                UtilReadFile(geomFile, &buffer);
                g = CompileShader(buffer, GL_GEOMETRY_SHADER);
                g_hash_table_insert(geomShaderNames, (gpointer)geomFile, (gpointer)g);
                free(buffer);
            }
            else {
                g = (GLuint)lup;
            }
            self->geom = g;
        }

        /* compile the shader program */
        self->program = CompileProgram(v, f, g, attributes, nAttributes);

        /* get matrix uniform locations */
        self->modelMatrixID        = glGetUniformLocation(self->program, "Model");
        self->viewMatrixID         = glGetUniformLocation(self->program, "View");
        self->projectionMatrixID   = glGetUniformLocation(self->program, "Projection");

        /* no textures for now */
        self->texture.id = 0;
    }


    /* Start and Update are part of all components */
    void Start() 
    {
        if(materials == NULL) materials = g_hash_table_new(g_int_hash, g_direct_equal);
        if(fragShaders == NULL) fragShaders = g_hash_table_new(g_int_hash, g_int_equal);
        if(vertShaders == NULL) vertShaders = g_hash_table_new(g_int_hash, g_int_equal);
        if(geomShaders == NULL) geomShaders = g_hash_table_new(g_int_hash, g_int_equal);
        if(fragShaderNames == NULL) fragShaderNames = g_hash_table_new(g_str_hash, g_int_equal);
        if(vertShaderNames == NULL) vertShaderNames = g_hash_table_new(g_int_hash, g_int_equal);
        if(geomShaderNames == NULL) geomShaderNames = g_hash_table_new(g_str_hash, g_int_equal);

        if(self->file != NULL) {
            self->Load(self, self->file);
        }
    }
    void Update() 
    {

    }

    void Collide(Entity* e)
    {
        puts("material collision");
    }
}
#elif defined SW_RENDER
    /* TODO: software renderer material component */
#else 
#error "no rendering method selected."
#endif
