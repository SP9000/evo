#include "draw.h"

/* TODO: the way uniforms are set is not remotely reliable I think... gon'
 * have to rework that big time */
/************************* Rendering variables *******************************/
/* Matrices for the shaders */
static Mat4x4 sceneModelMat;
static Mat4x4 sceneProjectionMat;
static Mat4x4 sceneViewMat;
/* ID's for the matrix uniforms */
static GLuint sceneModelMatID;
static GLuint sceneProjectionMatID;
static GLuint sceneViewMatID;

/* Matrices for GUI */
static Mat4x4 GUIsceneModelMat;
static Mat4x4 GUIsceneProjectionMat;
static Mat4x4 GUIsceneViewMat;
/* ID's for the GUI matrix uniforms */
static GLuint GUIsceneModelMatID;
static GLuint GUIsceneProjectionMatID;
static GLuint GUIsceneViewMatID;

/* The draw target for the pre-post-pass rendering */
static DrawTarget* activeTarget;

/* The model that is used for post-processing effects (a simple rect) */
static Model* postPassRect;
 
/* Screen surface. */
static SDL_Surface *screen;

/* The current camera. */
static Camera cam;


/************************** helper functions *********************************/
void DrawWidgetRecursive(gpointer data, gpointer user_data);
void DrawSceneModel(Model* m);

/*****************************************************************************/
/* Rendering subsystem                                                       */
/*****************************************************************************/
int Draw_Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error: Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    /* Turn on double buffering. */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);   
    screen = SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
    
    /* Initialize OpenGL. */
    if(glewInit() == GLEW_OK) {
        int success; 

        printf("OpenGL extensions availiable\n");
        printf("Shader version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        glClearColor(1.0, 1.0, 1.0, 0.0);
        
        /* Initialize the camera. */
        cam.pos[0] = 0.0f; cam.pos[1] = 0.0f; cam.pos[2] = -4.0f;
        cam.rot[0] = 0.0f; cam.rot[1] = 0.0f; cam.rot[2] = 0.0f;
        cam.fov = 60.0f;
        cam.aspect = 640.0f / 480.0f;
        cam.near = 0.1f;
        cam.far = 100.0f;
        
        /* Initialize matrices */
        Mat4x4LoadIdentity(sceneModelMat);
        Mat4x4LoadIdentity(sceneViewMat);
        Mat4x4LoadIdentity(sceneProjectionMat);
        Mat4x4PerspMat(sceneProjectionMat, cam.fov, cam.aspect, cam.near, cam.far);

        /* Initialize GUI matrices */
        Mat4x4LoadIdentity(GUIsceneModelMat);
        Mat4x4LoadIdentity(GUIsceneViewMat);
        Mat4x4LoadIdentity(GUIsceneProjectionMat);
        Mat4x4OrthoMat(GUIsceneProjectionMat, 0.0f, (float)screen->w, (float)screen->h, 0.0f, 0.01f, 100.0f);
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


    /* create and use render to texture target. */
    activeTarget = Draw_NewTarget();
    Draw_SetTarget(activeTarget);

    /* create the post pass rectangle model */
    float postPassRectUV[] = {0.0f, 0.0f, 
                            1.0f, 0.0f,
                            1.0f, 1.0f,
                            0.0f, 1.0f};
    postPassRect = GenRect(0,0,640,480,30);
    Model_BufferAttribute(postPassRect, MODEL_ATTRIBUTE_TEXCO, postPassRectUV);

    /* set the material for the post pass rect. */
    Material* pprMat = Material_Load("tex.mat");
    Texture t = Draw_TargetToTexture(activeTarget);
    Material_SetTexture(pprMat, &t);

    return 0;
}

void Draw_Quit()
{
    /* TODO */
    return;
    glDeleteTextures(1, &activeTarget->texID);
    glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffersEXT(1, &activeTarget->fbID);
}

void Draw_StartFrame()
{
    /* clear GL buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    /* position the camera */
    Mat4x4LoadIdentity(sceneViewMat);
    Mat4x4Translate(sceneViewMat, -cam.pos[0], -cam.pos[1], cam.pos[2]);
    glUniformMatrix4fv(sceneViewMatID, 1, 0, sceneViewMat);
    /* use scene projection matrix */
    glUniformMatrix4fv(sceneModelMatID, 1, 0, sceneModelMat);
    glUniformMatrix4fv(sceneViewMatID, 1, 0, sceneViewMat);
    glUniformMatrix4fv(sceneProjectionMatID, 1, 0, sceneProjectionMat);
}

void Draw_FinishFrame()
{
    /* do the post pass rendering - draw to back buffer*/
    glClear(GL_COLOR_BUFFER_BIT);

    /* Use the GUI orthographic matrices */
    Mat4x4LoadIdentity(GUIsceneViewMat);
    Mat4x4Translate(GUIsceneViewMat, 0, 0, -1);
    glUniformMatrix4fv(postPassRect->mat.viewMatrixID, 1, 0, GUIsceneViewMat);
    glUniformMatrix4fv(postPassRect->mat.modelMatrixID, 1, 0, GUIsceneModelMat);
    glUniformMatrix4fv(postPassRect->mat.projectionMatrixID, 1, 0, GUIsceneProjectionMat);

    Draw_SetTarget(NULL);
    Draw_Model(postPassRect);

    /* render next frame to texture again. */
    Draw_SetTarget(activeTarget);
}

void Draw_Scene()
{
    Scene_Foreach(DrawSceneModel);
}

void Draw_GUI()
{
    glDisable(GL_DEPTH_TEST);

    /* set up GUI "camera" */
    Mat4x4LoadIdentity(GUIsceneViewMat);
    Mat4x4Translate(GUIsceneViewMat, 0, 0, -1);

    glEnable(GL_SCISSOR_TEST);
    DrawWidgetRecursive((gpointer)GUILayout_GetRootWidget(), NULL);
    glDisable(GL_SCISSOR_TEST);
}

void Draw_OptimizeModel(Model* m) {
    int i;
    
    /* create a VAO for the model */
    glGenVertexArrays(1, &m->vao);
    glBindVertexArray(m->vao);

    /* buffer all the attributes of the model into VBO's */
    m->vboIDs = (GLuint*)malloc(m->numAttributes * sizeof(GLuint));
    glGenBuffers(m->numAttributes, m->vboIDs);
    for(i = 0; i < m->numAttributes; ++i) {
        int attrSize = Model_GetAttributeSize(m->attributeTable[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m->vboIDs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attrSize * m->numVertices,
                m->attributes[i], GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)i, attrSize, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(i);
    }
    /* Unbind. */
    glBindVertexArray(0);
}

void Draw_Model(Model *m)
{
    /* Bind the models' vertex attribute object. */
    glBindVertexArray(m->vao);
    
    /* use the model's material's shader */
    glUseProgram(m->mat.program);

    /* bind any samplers (textures) the material uses */
    if(m->mat.texture.id != 0) {
        glUniform1i(m->mat.texture.loc, 0); /* TODO: use glProgramUniform in material.c */
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, m->mat.texture.id);
        glBindSampler(0, m->mat.texture.sampler); 
    }

    /* Draw the model. */
    glDrawArrays(m->primitive, 0, m->numVertices);

    /* Unbind. */
    glBindVertexArray(0);
}

void Draw_MoveCamera(float x, float y, float z)
{
    cam.pos[0] += x;
    cam.pos[1] += y;
    cam.pos[2] += z;
}

DrawTarget* Draw_NewTarget(int w, int h)
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

    glGenFramebuffersEXT(1, &target->fbID);
    glBindFramebufferEXT(GL_FRAMEBUFFER, target->fbID);

    /* attach texture to FBO */
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
            GL_TEXTURE_2D, target->fbID, 0);

    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "Warning: draw target not properly supported.\n");
    }
    return target;
}

void Draw_SetTarget(DrawTarget* target)
{
    activeTarget = target;
    if(target != NULL) {
        glBindFramebufferEXT(GL_FRAMEBUFFER, target->fbID);
    }
    else {
        glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
    }
}

Texture Draw_TargetToTexture(DrawTarget* target)
{
    Texture t;
    t.id = target->texID;
    return t;
}

/*****************************************************************************/
/*                           local functions                                 */
/*****************************************************************************/
void DrawSceneModel(Model* m)
{
    glUniformMatrix4fv(m->mat.viewMatrixID, 1, 0, sceneViewMat);
    glUniformMatrix4fv(m->mat.modelMatrixID, 1, 0, sceneModelMat);
    glUniformMatrix4fv(m->mat.projectionMatrixID, 1, 0, sceneProjectionMat);
    Draw_Model(m);
}

void DrawWidgetRecursive(gpointer data, gpointer user_data)
{
    Widget* w = (Widget*)data;
    /* draw the widget background */
    glUniformMatrix4fv(w->background->mat.viewMatrixID, 1, 0, GUIsceneViewMat);
    glUniformMatrix4fv(w->background->mat.modelMatrixID, 1, 0, GUIsceneModelMat);
    glUniformMatrix4fv(w->background->mat.projectionMatrixID, 1, 0, GUIsceneProjectionMat);
    Draw_Model(w->background);

    /* draw the widget contents */
    glUniformMatrix4fv(w->contents->mat.viewMatrixID, 1, 0, GUIsceneViewMat);
    glUniformMatrix4fv(w->contents->mat.modelMatrixID, 1, 0, GUIsceneModelMat);
    glUniformMatrix4fv(w->contents->mat.projectionMatrixID, 1, 0, GUIsceneProjectionMat);
    Draw_Model(w->contents);

    glScissor(w->rect.x * screen->w, w->rect.y * screen->h, 
            w->rect.w * screen->w, w->rect.h * screen->h);
    g_slist_foreach(w->children, DrawWidgetRecursive, NULL);
}
