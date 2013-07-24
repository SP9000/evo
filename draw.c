#include "draw.h"

/************************* Rendering variables *******************************/
/* Screen surface. */
SDL_Surface *screen;
Component_Camera* main_cam;

/* The draw target for the pre-post-pass rendering */
static DrawTarget* activeTarget;

/* The model that is used for post-processing effects (a simple rect) */
static Component_Model* postPassRect;
 
/* The current camera. */
static Component_Camera* activeCam;
static Component_Camera* guiCam;
static GSList* cameras;

/* components for drawing textured quads */
static Component_Model* tex_quad;
static Component_Material* tex_mat;

int Draw_Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error: Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    /* Turn on double buffering. */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);   
    screen = SDL_SetVideoMode(960, 540, 32, SDL_OPENGL | SDL_RESIZABLE);
    
    /* Initialize OpenGL. */
    if(glewInit() == GLEW_OK) {
        printf("OpenGL extensions availiable\n");
        printf("Shader version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    }
    /* Initialize legacy OpenGL for older hardware. */
    else {
        printf("OpenGL extensions not supported, going old-skool\n");
        printf("But seriously, this is GOING to be a miserable failure\n");
        glEnable(GL_TEXTURE_2D);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glViewport(0, 0, 640, 480);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    main_cam = NULL;

    /* create a rectangle for drawing quads */
    float uvs[] = {0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f};
    float vtxs[] = {0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1,1,0, 0.0f,1.0f,0.0f};
    tex_quad = Component_Model_New(NULL);
    tex_quad->Start(tex_quad);
    tex_quad->numVertices = 4;
    tex_quad->BufferAttribute(tex_quad, MODEL_ATTRIBUTE_VERTEX, vtxs);
    tex_quad->BufferAttribute(tex_quad, MODEL_ATTRIBUTE_TEXCO, uvs);

    tex_mat = Component_Material_New("tex.mat");
    tex_mat->Start(tex_mat);
    tex_quad->primitive = GL_QUADS;
    tex_quad->Optimize(tex_quad);

    /* wider lines */
    glLineWidth(2);
    return 0;

    /* TODO: post-processing */
    /* create the post pass rectangle model */
    /*
    float postPassRectUV[] = {0.0f, 0.0f, 
                            1.0f, 0.0f,
                            1.0f, 1.0f,
                            0.0f, 1.0f};

    postPassRect = ModelGen_Rect(-320,-320,640,480,10);
    Model_BufferAttribute(postPassRect, MODEL_ATTRIBUTE_TEXCO, postPassRectUV);
    */
    /* set the material for the post pass rect. */
    /*
    Material* pprMat = Material_Load("tex.mat");
    Texture t = Draw_TargetToTexture(activeTarget);
    Material_SetTexture(pprMat, &t);
    */
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

}

void Draw_ResizeScreen(int w, int h)
{
    GSList* it;
    screen = SDL_SetVideoMode(w, h, 32, SDL_OPENGL | SDL_RESIZABLE);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    for(it = cameras; it != NULL; it = g_slist_next(it)) {
        Component_Camera* c = (Component_Camera*)it->data;
        if(c->orthographic) {
            /*
            c->Orthographic(c, 0.0f, c->w, ->h, 
                    0.0f, 0.01f, 100.0f);
            */
        }
        else {
            c->Perspective(c, c->fov, 
                    (float)w / (float)h,
                    c->nearZ, c->farZ);
        }
    }
}

void Draw_FinishFrame()
{
#if 0
    glDisable(GL_DEPTH_TEST);

    glUseProgram(postPassRect->mat->program);
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
#endif
}

void Draw_Scene()
{
    GSList* it;
    for(it = cameras; it != NULL; it = g_slist_next(it)) {
        int i;
        Component_Camera* cam = (Component_Camera*)it->data;
        /* position the camera */
        Mat4x4LoadIdentity(&cam->viewMat);
        Mat4x4Translate(&cam->viewMat, -cam->transform->pos.x, 
                -cam->transform->pos.y, cam->transform->pos.z);
        main_cam = cam;
        for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
            if((1 << i) & cam->render_layers) {
                GSList* jt;
                for(jt = scene_layers[i]; jt != NULL; jt = g_slist_next(jt)) {
                    Component_Renderer* r = (Component_Renderer*)jt->data;
                    r->Render(r);
                }
            }
        }
    }
}

void Draw_GUI()
{
    Component_Camera* saveCam;

    glDisable(GL_DEPTH_TEST);

    /* set up GUI "camera" */
    Mat4x4LoadIdentity(&guiCam->viewMat);
    Mat4x4Translate(&guiCam->viewMat, 0, 0, -1);

    saveCam = activeCam;
    activeCam = guiCam;

    glEnable(GL_SCISSOR_TEST);
    //Scene_ForeachWidget(Draw_Widget);
    glDisable(GL_SCISSOR_TEST);

    activeCam = saveCam;
}


void Draw_Model(Component_Model* m)
{
#if 0
    /* translate */
    Mat4x4Push(activeCam->viewMat);
    Mat4x4Translate(activeCam->viewMat, -m->transform->pos.x, -m->transform->pos.y, m->transform->pos.z);
    activeCam->viewMat[0] *= m->transform->scale.x;
    activeCam->viewMat[5] *= m->transform->scale.y;
    activeCam->viewMat[10] *= m->transform->scale.z;

    /* Bind the models' vertex attribute object. */
    glBindVertexArray(m->vao);

    /* use the model's material's shader */
    glUseProgram(m->mat->program);

    /* set matrices */
    glUniformMatrix4fv(m->mat->modelMatrixID, 1, GL_FALSE, activeCam->modelMat);
    glUniformMatrix4fv(m->mat->viewMatrixID, 1, GL_FALSE, activeCam->viewMat);
    glUniformMatrix4fv(m->mat->projectionMatrixID, 1, GL_FALSE, activeCam->projectionMat);

    /* bind any samplers (textures) the material uses */
    if(m->mat->texture.id != 0) {
#if 0
        glUniform1i(m->mat->texture.loc, 0); /* TODO: use glProgramUniform in material.c */
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, m->mat->texture.id);
        glBindSampler(0, m->mat->texture.sampler); 
#endif
    }

    /* Draw the model. */
    glDrawArrays(m->primitive, 0, m->numVertices);

    /* Unbind. */
    glBindVertexArray(0);

    /* restore camera */
    Mat4x4Pop(activeCam->viewMat);
#endif
}

void Draw_Widget(Component_Widget* w)
{
    GSList* it;
    glScissor(w->rect.x * screen->w, w->rect.y * screen->h, 
            w->rect.w * screen->w, w->rect.h * screen->h);
}

void Draw_MoveCamera(float x, float y, float z)
{
    /* 
     * TODO:
    activeCam->transform->pos.x += x;
    activeCam->transform->pos.y += y;
    activeCam->transform->pos.z += z;
    */
}

void Draw_Texture(Texture tex, Rect* rect)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    /* save state */
    Mat4x4Push(&main_cam->viewMat);
    Mat4x4LoadIdentity(&main_cam->viewMat);
    Mat4x4Translate(&main_cam->viewMat, -rect->x, -rect->y, -1.0f);
    main_cam->viewMat.a00*= rect->w;
    main_cam->viewMat.a11 *= rect->h;

    /* use the model's material's shader */
    glUseProgram(tex_mat->program);

    glUniformMatrix4fv(tex_mat->modelMatrixID, 1, GL_FALSE, 
            Mat4x4Pack(&main_cam->modelMat));
    glUniformMatrix4fv(tex_mat->viewMatrixID, 1, GL_FALSE, 
            Mat4x4Pack(&main_cam->viewMat));
    glUniformMatrix4fv(tex_mat->projectionMatrixID, 1, GL_FALSE, 
            Mat4x4Pack(&main_cam->projectionMat));

    /* bind the texture */
    GLint loc = glGetUniformLocation(tex_mat->program, "tex");
    glActiveTexture(GL_TEXTURE0 + 0);
    glUniform1i(loc, 0); 
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glEnable(GL_TEXTURE_2D);

    /* bind attribute array and draw */
    glBindVertexArray(tex_quad->vao);
    glDrawArrays(GL_QUADS, 0, 4);
    glBindVertexArray(0);

    /* restore */
    Mat4x4Pop(&main_cam->viewMat);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
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
    
    /* create and attach depth buffer */
    glBindRenderbufferEXT(GL_RENDERBUFFER, target->depthID);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
        GL_DEPTH_COMPONENT24, screen->w, screen->h);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
        GL_RENDERBUFFER_EXT, target->depthID);

    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "Warning: draw target not properly supported.\n");
    }
    return target;
}

void Draw_AddCamera(Component_Camera* cam)
{
    cameras = g_slist_append(cameras, (gpointer)cam);
}


void Draw_SetTarget(DrawTarget* target)
{
    if(target == NULL) {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, activeTarget->texID);
        //glUniform1i(glGetUniformLocation(postPassRect->mat->program, "tex"), 0);
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

