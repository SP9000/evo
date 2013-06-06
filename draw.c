#include "draw.h"

const GLchar *selectShader =
"#version 130\n"
"in vec3 in_Position;\n"
"out vec4 ex_Color;\n"
"uniform mat4x4 Model, View, Projection;\n"
"void main()\n"
"{\n"
"   gl_Position = Projection * View * Model * vec4(in_Position,1.0);\n"
"   ex_Color = vec4(0.0, 1.0, 0.0, 0.5);"
"}";

/* Matrices for the shaders */
static Mat4x4 ModelMat;
static Mat4x4 ProjectionMat;
static Mat4x4 ViewMat;
/* ID's for the matrix uniforms */
static GLuint ModelMatID;
static GLuint ProjectionMatID;
static GLuint ViewMatID;

/* Matrices for GUI */
static Mat4x4 GUIModelMat;
static Mat4x4 GUIProjectionMat;
static Mat4x4 GUIViewMat;
/* ID's for the GUI matrix uniforms */
static GLuint GUIModelMatID;
static GLuint GUIProjectionMatID;
static GLuint GUIViewMatID;
 
/* Screen surface. */
static SDL_Surface *screen;

/* The current camera. */
static Camera cam;

/* The current vertex and fragment shaders. */
static Material* MaterialMain;
static Material* MaterialGUISelect;

/* IDs of loaded programs (materials) */
static GHashTable* materials;
/* IDs of all loaded shaders */
static GHashTable* fragShaders;
static GHashTable* vertShaders;
/* translation table from material names -> IDs */
static GHashTable* fragShaderNames;
static GHashTable* vertShaderNames;


/* Material loading */
int MaterialInit();
Material* MaterialLoad(const char* vertFile, const char* fragFile, char** attributes, int numAttributes);
Material* GetMaterial(int id);
GLuint CompileProgram(GLuint vertShader, GLuint fragShader, char **attributes, int numAttributes);
int ReadFile(const char* filename, char** buffer);
void AddMaterial(int id, Material *mat);
GLuint CompileShader(const GLchar* shader, GLuint type);

/* The current level. */
// static Map world;


/*****************************************************************************/
/* Rendering subsystem                                                       */
/*****************************************************************************/
int DrawInit()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error: Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }
    puts("Initializing draw");

    /* Turn on double buffering. */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);   
    screen = SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
    
    /* Initialize OpenGL. */
    if(glewInit() == GLEW_OK) {
        int success; 

        printf("OpenGL extensions availiable\n");
        printf("Shader version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        glClearColor(1.0, 1.0, 1.0, 0.0);
        

        /* Initialize material subsystem */
        if(MaterialInit() != 0) {
            fprintf(stderr, "Error: could not initialize the material subsytem\n");
        }

        /* compile shader programs */
        char attr1[] = "in_Position"; char attr2[] = "in_Color"; char attr3[] = "in_Normal";
        char *attrs[3] = {attr1, attr2, attr3};
        char guiAttr1[] = "in_Position"; char guiAttr2[] = "in_Color"; 
        char *guiAttrs[2] = {guiAttr1, guiAttr2};
        MaterialMain = MaterialLoad("test.vert", "test.frag", attrs, 3);
        MaterialGUISelect = MaterialLoad("gui.vert", "gui.frag", guiAttrs, 2);
        puts("loaded materials successfully");

        glUseProgram(MaterialMain->program);
        /* Initialize the camera. */
        cam.pos[0] = 0.0f; cam.pos[1] = 0.0f; cam.pos[2] = -4.0f;
        cam.rot[0] = 0.0f; cam.rot[1] = 0.0f; cam.rot[2] = 0.0f;
        cam.fov = 60.0f;
        cam.aspect = 640.0f / 480.0f;
        cam.near = 0.1f;
        cam.far = 100.0f;
        
        /* Initialize matrices */
        Mat4x4LoadIdentity(ModelMat);
        Mat4x4LoadIdentity(ViewMat);
        Mat4x4LoadIdentity(ProjectionMat);
        Mat4x4PerspMat(ProjectionMat, cam.fov, cam.aspect, cam.near, cam.far);
        /* get uniforms for all model/view/projection matrices */
        ModelMatID = glGetUniformLocation(MaterialMain->program, "Model");
        ViewMatID = glGetUniformLocation(MaterialMain->program, "View");
        ProjectionMatID = glGetUniformLocation(MaterialMain->program, "Projection");

        /* Initialize GUI matrices */
        Mat4x4LoadIdentity(GUIModelMat);
        Mat4x4LoadIdentity(GUIViewMat);
        Mat4x4LoadIdentity(GUIProjectionMat);
        Mat4x4OrthoMat(GUIProjectionMat, 0.0f, (float)screen->w, (float)screen->h, 0.0f, 0.01f, 100.0f);
        /* get uniforms for GUI matrices */
        GUIModelMatID = glGetUniformLocation(MaterialGUISelect->program, "Model");
        GUIViewMatID = glGetUniformLocation(MaterialGUISelect->program, "View");
        GUIProjectionMatID = glGetUniformLocation(MaterialGUISelect->program, "Projection");
    }
    /* Initialize legacy OpenGL for older hardware. */
    else {
        printf("OpenGL extensions not supported, going old-skool\n");
        printf("But seriously, this is probably going to be a miserable failure\n");
        glEnable(GL_TEXTURE_2D);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glViewport(0, 0, 640, 480);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    /* wider lines */
    glLineWidth(3);

    /* Done. */
    puts("Draw initialization complete");
    return 0;
}

void DrawQuit()
{
        glDetachShader(MaterialMain->program, MaterialMain->vert);
        glDetachShader(MaterialMain->program, MaterialMain->frag);
        glDeleteShader(MaterialMain->vert);
        glDeleteShader(MaterialMain->frag);
}

void DrawStartFrame()
{
    /* clear GL buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* position the camera */
    Mat4x4LoadIdentity(ViewMat);
    Mat4x4Translate(ViewMat, -cam.pos[0], -cam.pos[1], cam.pos[2]);
    glUniformMatrix4fv(ViewMatID, 1, 0, ViewMat);
    /* use scene projection matrix */
    glUniformMatrix4fv(ModelMatID, 1, 0, ModelMat);
    glUniformMatrix4fv(ViewMatID, 1, 0, ViewMat);
    glUniformMatrix4fv(ProjectionMatID, 1, 0, ProjectionMat);

}

void DrawGUI()
{
    /* set up GUI "camera" */
    Mat4x4LoadIdentity(GUIViewMat);
    Mat4x4Translate(GUIViewMat, 0, 0, -1);
    glUniformMatrix4fv(GUIViewMatID, 1, 0, GUIViewMat);
    /* do GUI */
    glUniformMatrix4fv(GUIModelMatID, 1, 0, GUIModelMat);
    glUniformMatrix4fv(GUIProjectionMatID, 1, 0, GUIProjectionMat);
}

void DrawModel(Model *m)
{
    /* If there is no vertex and color buffers, generate them. */
    if(m->vertexVBOID == 0 || m->colorVBOID == 0 || m->normalVBOID == 0) {
        puts("generating VBO's");
        glGenVertexArrays(1, &m->vao);
        glBindVertexArray(m->vao);

        /* Buffer the vertex data. */
        glGenBuffers(1, &m->vertexVBOID);
        glBindBuffer(GL_ARRAY_BUFFER, m->vertexVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->numVertices,
                m->vertices, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        /* Buffer the color data. */
        glGenBuffers(1, &m->colorVBOID);
        glBindBuffer(GL_ARRAY_BUFFER, m->colorVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * m->numVertices,
                m->colors, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        /* Buffer normal data. */
        glGenBuffers(1, &m->normalVBOID);
        glBindBuffer(GL_ARRAY_BUFFER, m->normalVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->numVertices,
                m->normals, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);
    }

    if(m->numVertices == 0) {
        fprintf(stderr, "Error: attempted to render a model with 0 vertices\n");
        return;
    }

    /* Bind the models vertex attribute object. */
    glBindVertexArray(m->vao);

    /* Draw the model. */
    glDrawArrays(GL_TRIANGLES, 0, m->numVertices);
    
    /* Unbind. */
    glBindVertexArray(0);
}

void DrawMoveCamera(float x, float y, float z)
{
    cam.pos[0] += x;
    cam.pos[1] += y;
    cam.pos[2] += z;
}

/*****************************************************************************/
/*  GUI subsystem                                                            */
/*****************************************************************************/
Model* GUIDrawText(Rect *box, char *text, Model* m)
{
    /* # of lines in each character */
    int charSizes[] = {
        3, /* A */
        5, /* B */
        3, /* D */
        3, /* C */
        4, /* E */
        3, /* F */
        5, /* G */
        3, /* H */
        3, /* I */
        4, /* J */
        3, /* K */
        2, /* L */
        4, /* M */
        3, /* N */
        4, /* O */
        3, /* P */
        5, /* Q */
        4, /* R */
        3, /* S */
        2, /* T */
        3, /* U */
        2, /* V */
        4, /* W */
        2, /* X */
        3, /* Y */
        3, /* Z */
    };
    /* lines for each character */
    GLfloat font[26][5*4] = {
    {-1,-1,0,1, 0,1,1,-1, -.5f,0,.5f,0},                                /* A */
    {-1,1,-1,-1, -1,1,1,.5f, 1,.5f,-1,0, -1,0,1,-.5f, 1,-.5f,-1,-1},    /* B */
    {-1,1,1,1, -1,1,-1,-1, -1,-1,1,-1},                                 /* C */
    {-1,1,-1,-1, -1,1,1,0, 1,0,-1,-1},                                  /* D */
    {-1,1,1,1, -1,1,-1,-1, -1,-1,1,-1, -1,0, 1,0},                      /* E */
    {-1,1,1,1, -1,1,-1,-1, -1,0, 1,0},                                  /* F */
    {-1,1,1,1, -1,1,-1,-1, -1,-1,1,-1, 1,-1,1,0, 1,0,0,0},              /* G */
    {-1,1,-1,-1, 1,1,1,-1, -1,0,1,0},                                   /* H */
    {-1,1,1,1, 0,1,0,-1, -1,-1,1,-1},                                   /* I */
    {-1,1,1,1, 0,1,0,-1, -1,-1,0,-1, -1,-1,-1,0},                       /* J */
    {-1,1,-1,-1, -1,0,1,1, -1,0,1,-1},                                  /* K */
    {-1,1,-1,-1, -1,-1,1,-1},                                           /* L */
    {-1,1,-1,-1, -1,1,0,0, 0,0,1,1, 1,1,1,-1},                          /* M */
    {-1,1,-1,-1, -1,1,1,-1, 1,1,1,-1},                                  /* N */
    {-1,1,1,1, -1,1,-1,-1, -1,-1,1,-1, 1,1,1,-1},                       /* O */
    {-1,1,-1,-1, -1,1,1,.5f, 1,.5f,-1,0},                               /* P */
    {-1,1,1,1, -1,1,-1,-1, -1,-1,1,-1, 1,1,1,-1, 0,0,1,-1},             /* Q */
    {-1,1,-1,-1, -1,1,1,.5f, 1,.5f,-1,0, -1,0,1,-1},                    /* R */
    {-1,1,1,1, -1,1,1-1, 1,-1,-1,-1},                                   /* S */
    {-1,1,1,1, 0,1,0,-1},                                               /* T */
    {-1,1,-1,-1, -1,-1,1,-1, 1,1,1,-1},                                 /* U */
    {-1,1,0,-1, 0,-1,1,1},                                              /* V */
    {-1,1,-1,-1, -1,-1,0,0, 0,0,1,-1, 1,-1,1,1},                        /* W */
    {-1,1,1,-1, -1,-1,1,1},                                             /* X */
    {-1,1,0,0, 1,1,0,0, 0,0,0,-1},                                      /* Y */
    {-1,1,1,1, 1,1,-1,-1, -1,-1, 1,-1}                                  /* Z */
    };

    Color color = {0.0f, 0.0f, 1.0f, 1.0f};

    if(m == NULL) {
        m = ModelNew(1000);
        /* add all vertices contained in the given string */
        float x = 3.0f;
        float y = 4.0f;
        float scale = 5;
        while(*text) {
            int i;
            int idx = (*text) - 'A';
            Vertex v;
            v[2] = .9f;

            /* add all vertices that compose this character */
            for(i = 0; i < charSizes[idx]*4; i += 4) {
                /* (x1, y1) */
                v[0] = (font[idx][i] + x) * scale;  
                v[1] = (font[idx][i+1] + y) * scale;
                ModelAddColor(m, color);
                ModelAddVertex(m, v);

                /* (x2, y2) */
                v[0] = (font[idx][i+2] + x) * scale;
                v[1] = (font[idx][i+3] + y) * scale;
                ModelAddColor(m, color);
                ModelAddVertex(m, v);
            }
            x += scale;
            text++;
        }
    }

    /* buffer data if necessary */
    if(m->vertexVBOID == 0 || m->colorVBOID == 0) {
        puts("generating VBO's");
        glGenVertexArrays(1, &m->vao);
        glBindVertexArray(m->vao);

        /* Buffer the vertex data. */
        glGenBuffers(1, &m->vertexVBOID);
        glBindBuffer(GL_ARRAY_BUFFER, m->vertexVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->numVertices,
                m->vertices, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        /* Buffer the color data. */
        glGenBuffers(1, &m->colorVBOID);
        glBindBuffer(GL_ARRAY_BUFFER, m->colorVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * m->numVertices,
                m->colors, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
    }

    /* Bind the models vertex attribute object. */
    glBindVertexArray(m->vao);

    /* Draw the text. */
    glDrawArrays(GL_LINES, 0, m->numVertices);

    /* Unbind. */
    glBindVertexArray(0);
    return m;
}

/*****************************************************************************/
/*  Shader/Material loading subsystem                                        */
/*****************************************************************************/
int MaterialInit()
{
    materials = g_hash_table_new(g_int_hash, g_direct_equal);
    fragShaders = g_hash_table_new(g_int_hash, g_int_equal);
    fragShaderNames = g_hash_table_new(g_str_hash, g_int_equal);
    vertShaders = g_hash_table_new(g_int_hash, g_int_equal);
    vertShaderNames = g_hash_table_new(g_str_hash, g_int_equal);
    return 0;
}

Material* GetMaterial(int id)
{
    return (Material*)g_hash_table_lookup(materials, (gconstpointer)id);
}

void AddMaterial(int id, Material *mat)
{
    g_hash_table_insert(materials, (gpointer)id, (gpointer)mat);
}

Material* MaterialLoad(const char* vertFile, const char* fragFile, char** attributes, int numAttributes)
{
    Material* m;

    char* frag;
    char* vert;
    GLuint v, f;
    gpointer lup;

    m = (Material*)malloc(sizeof(Material));

    /* has this shader already been loaded? */
    lup = (char*)g_hash_table_lookup(vertShaderNames, vertFile);
    if(lup == NULL) {
        /* no, load it */
        ReadFile(vertFile, &vert);
        v = CompileShader(vert, GL_VERTEX_SHADER);
        /* insert into hash tables */
        g_hash_table_insert(vertShaderNames, (gpointer)vertFile, (gpointer)v);
    }
    else {
        /* yes, use saved ID */
        v = (GLuint)lup;
    }
    m->vert = v;

    lup = (char*)g_hash_table_lookup(fragShaderNames, fragFile);
    if(lup == NULL) {
        ReadFile(fragFile, &frag);
        f = CompileShader(frag, GL_FRAGMENT_SHADER);
        g_hash_table_insert(fragShaderNames, (gpointer)fragFile, (gpointer)f);
    }
    else {
        f = (GLuint)lup;
    }
    m->frag = f;

    m->program = CompileProgram(v, f, attributes, numAttributes);
    return m;
}

int ReadFile(const char* filename, char** buffer)
{
    FILE* fp = NULL;
    char* data = NULL;
    long lSize;

    fp = fopen(filename, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error: could not open file %s for reading\n", filename);
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);
    data = (char*)malloc(sizeof(char) * (lSize+1));
    if(data == NULL) {
        fclose(fp);
        fprintf(stderr, "Failed to allocate buffer for material load\n");
        return -2;
    }
    if(lSize != fread(data, sizeof(char), lSize, fp)) {
        fclose(fp);
        free(data);
        fprintf(stderr, "Failed to read material file %s\n", filename);
        return -3;
    }
    data[lSize] = '\0';
    *buffer = data;
    fclose(fp);
}

GLuint CompileShader(const GLchar* shader, GLuint type) 
{
    GLuint s;
    GLsizei len;
    int success;

    s = glCreateShader(type);
    glShaderSource(s, 1, &shader, NULL);
    glCompileShader(s);
    glGetShaderiv(s, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
        char *log = malloc(sizeof(char) * len);
        glGetShaderInfoLog(s, len, &len, log);
        printf("Shader failed to compile. Log:\n%s", log);
        free(log);
        if(type == GL_VERTEX_SHADER) {
            fprintf(stderr, "Error: vertex shader was not compiled successfully.\n");
        }
        else if(type == GL_FRAGMENT_SHADER) {
            fprintf(stderr, "Error: fragment shader was not compiled successfully.\n");
        }
        else {
            fprintf(stderr, "Error: shader was not compiled successfully.\n");
        }
        exit(EXIT_FAILURE);
    }
    return s;
}

GLuint CompileProgram(GLuint vertShader, GLuint fragShader, char **attributes, int numAttributes)
{
    int i;
    GLuint program;

    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);

    for(i = 0; i < numAttributes; i++) {
        printf("%s: %d\n", attributes[i], i);
        glBindAttribLocation(program, i, attributes[i]);
    }
    glLinkProgram(program);
    return program;
}

