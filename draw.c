#include "draw.h"

/* TODO: the way uniforms are set is not remotely reliable I think... gon'
 * have to rework that big time */
/************************* Rendering variables *******************************/
/* The draw target for the pre-post-pass rendering */
static DrawTarget* activeTarget;

/* The model that is used for post-processing effects (a simple rect) */
static Model* postPassRect;
 
/* Screen surface. */
static SDL_Surface *screen;

/* The current camera. */
static Camera* activeCam;
static Camera sceneCam;
static Camera guiCam;


/************************** helper functions *********************************/
void DrawWidgetRecursive(gpointer data, gpointer user_data);

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
    screen = SDL_SetVideoMode(960, 540, 32, SDL_OPENGL);
    
    /* Initialize OpenGL. */
    if(glewInit() == GLEW_OK) {
        printf("OpenGL extensions availiable\n");
        printf("Shader version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        
        /* Initialize the camera. */
        sceneCam.pos[0] = 0.0f; sceneCam.pos[1] = 0.0f; sceneCam.pos[2] = -4.0f;
        sceneCam.rot[0] = 0.0f; sceneCam.rot[1] = 0.0f; sceneCam.rot[2] = 0.0f;
        sceneCam.fov = 60.0f;
        sceneCam.aspect = (float)screen->w / (float)screen->h;
        sceneCam.near = 0.1f;
        sceneCam.far = 100.0f;
        
        /* Initialize matrices */
        Mat4x4LoadIdentity(sceneCam.modelMat);
        Mat4x4LoadIdentity(sceneCam.viewMat);
        Mat4x4LoadIdentity(sceneCam.projectionMat);
        Mat4x4PerspMat(sceneCam.projectionMat, sceneCam.fov, sceneCam.aspect, sceneCam.near, sceneCam.far);

        /* Initialize GUI matrices */
        Mat4x4LoadIdentity(guiCam.modelMat);
        Mat4x4LoadIdentity(guiCam.viewMat);
        Mat4x4LoadIdentity(guiCam.projectionMat);
        Mat4x4OrthoMat(guiCam.projectionMat, 0.0f, (float)screen->w, (float)screen->h, 0.0f, 0.01f, 100.0f);
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
    glLineWidth(2);

    return 0;

    /* TODO: post-processing */
    /* create the post pass rectangle model */
    float postPassRectUV[] = {0.0f, 0.0f, 
                            1.0f, 0.0f,
                            1.0f, 1.0f,
                            0.0f, 1.0f};

    postPassRect = GenRect(-320,-320,640,480,10);
    Model_BufferAttribute(postPassRect, MODEL_ATTRIBUTE_TEXCO, postPassRectUV);
    /* set the material for the post pass rect. */
    Material* pprMat = Material_Load("tex.mat");
    Texture t = Draw_TargetToTexture(activeTarget);
    Material_SetTexture(pprMat, &t);
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
    Mat4x4LoadIdentity(sceneCam.viewMat);
    Mat4x4Translate(sceneCam.viewMat, -sceneCam.pos[0], -sceneCam.pos[1], sceneCam.pos[2]);
    activeCam = &sceneCam;
}

void Draw_FinishFrame()
{
    glDisable(GL_DEPTH_TEST);

    glUseProgram(postPassRect->mat.program);
    Draw_SetTarget(NULL);
    glEnable(GL_TEXTURE_2D);
    /* do the post pass rendering - draw to back buffer*/
    glClear(GL_COLOR_BUFFER_BIT);

    /* Use the GUI orthographic matrices */
    Mat4x4LoadIdentity(guiCam.viewMat);
    Mat4x4Translate(guiCam.viewMat, 0, 0, -1);
    activeCam = &guiCam;
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();

    //Draw_Model(postPassRect);
    /* render next frame to texture again. */
    glDisable(GL_TEXTURE_2D);
    Draw_SetTarget(activeTarget);
}

void Draw_Scene()
{
    Scene_Foreach(Draw_Model);
}

void Draw_GUI()
{
    glDisable(GL_DEPTH_TEST);

    /* set up GUI "camera" */
    Mat4x4LoadIdentity(guiCam.viewMat);
    Mat4x4Translate(guiCam.viewMat, 0, 0, -1);

    activeCam = &guiCam;
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

    /* set matrices */
    glUniformMatrix4fv(m->mat.modelMatrixID, 1, GL_FALSE, activeCam->modelMat);
    glUniformMatrix4fv(m->mat.viewMatrixID, 1, GL_FALSE, activeCam->viewMat);
    glUniformMatrix4fv(m->mat.projectionMatrixID, 1, GL_FALSE, activeCam->projectionMat);

    /* bind any samplers (textures) the material uses */
    if(m->mat.texture.id != 0) {
#if 0
        glUniform1i(m->mat.texture.loc, 0); /* TODO: use glProgramUniform in material.c */
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, m->mat.texture.id);
        glBindSampler(0, m->mat.texture.sampler); 
#endif
    }

    /* Draw the model. */
    glDrawArrays(m->primitive, 0, m->numVertices);

    /* Unbind. */
    glBindVertexArray(0);
}

void Draw_MoveCamera(float x, float y, float z)
{
    sceneCam.pos[0] += x;
    sceneCam.pos[1] += y;
    sceneCam.pos[2] += z;
}

DrawTarget* Draw_NewTarget(int w, int h)
{
    GLenum status;
    DrawTarget* target = (DrawTarget*)malloc(sizeof(DrawTarget));

    glGenFramebuffersEXT(1, &(target->fbID));
    glGenTextures(1, &(target->texID));
    glGenRenderbuffersEXT(1, &(target->depthID));

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target->fbID);

    /* create a texture to render to */
    glBindTexture(GL_TEXTURE_2D, target->texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screen->w, screen->h, 0, GL_RGBA, 
            GL_INT, NULL);

    /* attach texture to FBO */
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
            GL_TEXTURE_2D, target->texID, 0);
    
    goto wat;
    /* create and attach depth buffer */
    glBindRenderbufferEXT(GL_RENDERBUFFER, target->depthID);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
        GL_DEPTH_COMPONENT24, screen->w, screen->h);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
        GL_RENDERBUFFER_EXT, target->depthID);
wat:

    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "Warning: draw target not properly supported.\n");
    }
    return target;
}

void Draw_SetTarget(DrawTarget* target)
{
    if(target == NULL) {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, activeTarget->texID);
        glUniform1i(glGetUniformLocation(postPassRect->mat.program, "tex"), 0);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, activeTarget->fbID);
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
void DrawWidgetRecursive(gpointer data, gpointer user_data)
{
    Widget* w = (Widget*)data;

    /* draw the widget background */
    Draw_Model(w->background);

    /* draw the widget contents */
    Draw_Model(w->contents);

    glScissor(w->rect.x * screen->w, w->rect.y * screen->h, 
            w->rect.w * screen->w, w->rect.h * screen->h);
    g_slist_foreach(w->children, DrawWidgetRecursive, NULL);
}
