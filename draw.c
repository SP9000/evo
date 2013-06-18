#include "draw.h"

/************************* Rendering variables *******************************/
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


/************************** helper functions *********************************/
void DrawWidgetRecursive(gpointer data, gpointer user_data);

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
        
        /* compile shader programs */
        MaterialMain = MaterialLoad("test.mat");
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
        GUIModelMatID = glGetUniformLocation(MaterialMain->program, "Model");
        GUIViewMatID = glGetUniformLocation(MaterialMain->program, "View");
        GUIProjectionMatID = glGetUniformLocation(MaterialMain->program, "Projection");
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
    glEnable(GL_DEPTH_TEST);

    glUseProgram(MaterialMain->program);

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
    Widget* w;

    glDisable(GL_DEPTH_TEST);
    /* use the GUI shader */
    //glUseProgram(MaterialGUISelect->program);

    /* set up GUI "camera" */
    Mat4x4LoadIdentity(GUIViewMat);
    Mat4x4Translate(GUIViewMat, 0, 0, -1);
    glUniformMatrix4fv(GUIViewMatID, 1, 0, GUIViewMat);
    /* do GUI */
    glUniformMatrix4fv(GUIModelMatID, 1, 0, GUIModelMat);
    glUniformMatrix4fv(GUIProjectionMatID, 1, 0, GUIProjectionMat);

    glEnable(GL_SCISSOR_TEST);
    DrawWidgetRecursive((gpointer)GUILayoutGetRootWidget(), NULL);
    glDisable(GL_SCISSOR_TEST);
}

void DrawWidgetRecursive(gpointer data, gpointer user_data)
{
    Widget* w = (Widget*)data;
    DrawModel(w->background);
    DrawModel(w->contents);
    glScissor(w->rect.x * screen->w, w->rect.y * screen->h, 
            w->rect.w * screen->w, w->rect.h * screen->h);
    g_slist_foreach(w->children, DrawWidgetRecursive, NULL);
}

void DrawOptimizeModel(Model* m) {
    int i;
    
    /* create a VAO for the model */
    glGenVertexArrays(1, &m->vao);
    glBindVertexArray(m->vao);

    /* buffer all the attributes of the model into VBO's */
    m->vboIDs = (GLuint*)malloc(m->numAttributes * sizeof(GLuint));
    glGenBuffers(m->numAttributes, m->vboIDs);
    for(i = 0; i < m->numAttributes; ++i) {
        int attrSize = ModelGetAttributeSize(m->attributeTable[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m->vboIDs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attrSize * m->numVertices,
                m->attributes[i], GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)i, attrSize, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(i);
    }
    /* Unbind. */
    glBindVertexArray(0);
}

void DrawModel(Model *m)
{
    /* Bind the models' vertex attribute object. */
    glBindVertexArray(m->vao);
    
    glUseProgram(MaterialMain->program);
    //glUseProgram(m->mat.program);

    /* Draw the model. */
    glDrawArrays(m->primitive, 0, m->numVertices);
    
    /* Unbind. */
    glBindVertexArray(0);
}

void DrawMoveCamera(float x, float y, float z)
{
    cam.pos[0] += x;
    cam.pos[1] += y;
    cam.pos[2] += z;
}

DrawTarget* DrawNewTarget(int w, int h)
{
    GLenum status;
    DrawTarget* target = (DrawTarget*)malloc(sizeof(DrawTarget));
    /* create a texture to render to */
    glGenTextures(1, &target->texID);
    glBindTexture(GL_TEXTURE_2D, target->texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
            GL_UNSIGNED_BYTE, NULL);

    glGenFramebuffers(1, &target->fbID);
    glBindFramebuffer(GL_FRAMEBUFFER, target->fbID);

    /* attach texture to FBO */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
            GL_TEXTURE_2D, target->fbID, 0);

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "Warning: draw target not properly supported.\n");
    }
    return target;
}

void DrawSetTarget(DrawTarget* target)
{
    if(target != NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, target->fbID);
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

