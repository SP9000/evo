#include "draw.h"

/************************* Rendering variables *******************************/
/* Screen surface. */
SDL_Surface *screen;

/* the global GUI camera */
tv_camera *tv_camera_gui;


/* The draw target for the pre-post-pass rendering */
static TvDrawTarget* activeTarget;

/* The model that is used for post-processing effects (a simple rect) */
static tv_model* postPassRect;
 
/* components for drawing textured quads */
static tv_model* tex_quad;
static tv_material* tex_mat;

int tv_draw_init()
{
	/* textured rect */
	float uvs[] = {0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f};
    float vtxs[] = {0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1,1,0, 0.0f,1.0f,0.0f};

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
        glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
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
	/* create and initialize the default cameras */
    main_cam = tv_camera_new();
	main_cam->pos.x = 0;
	main_cam->pos.y = 0;
	main_cam->pos.z = -10.0f;

	main_cam->scale.x = 1;
	main_cam->scale.y = 1;
	main_cam->scale.z = 1;

	main_cam->rot.x = 0;
	main_cam->rot.y = 0;
	main_cam->rot.z = 0;
	tv_camera_perspective(main_cam, 30.0f, (float)screen->w / (float)screen->h, .01f, 100.0f);
    /* position the camera */
	tv_mat4x4_load_identity(main_cam->view_mat);
	tv_mat4x4_translate(main_cam->view_mat, -main_cam->pos.x, 
            -main_cam->pos.y, main_cam->pos.z);

	tv_camera_gui = tv_camera_new();
	tv_camera_orthographic(tv_camera_gui, 0.0f, 1.0f, 0.0f, 1.0f, 0.01f, 100.0f);

	//tex_quad = tv_model_new();
	//tex_quad->num_vertices = 4;
    //tv_model_buffer_attribute(tex_quad, MODEL_ATTRIBUTE_VERTEX, vtxs);
	//tv_model_buffer_attribute(tex_quad, MODEL_ATTRIBUTE_TEXCO, uvs);

	//tv_material_load(tex_mat, "tex.mat");
	//tv_material_get_uniforms(tex_mat->program, 
	//	&tex_mat->model_mat, &tex_mat->view_mat, &tex_mat->projection_mat);
	//tex_quad->primitive = GL_QUADS;
	//tv_model_optimize(tex_quad);

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

void tv_draw_quit()
{
    /* TODO */
    return;
    glDeleteTextures(1, &activeTarget->texID);
    glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffersEXT(1, &activeTarget->fbID);
}

void tv_draw_start_frame()
{
    /* clear GL buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

	tv_mat4x4_push(main_cam->view_mat);
	tv_mat4x4_scale(main_cam->view_mat, main_cam->scale.x, main_cam->scale.y, main_cam->scale.z);
	tv_mat4x4_rotate(main_cam->view_mat, main_cam->rot.x, 1.0f, 0.0f, 0.0f);
	tv_mat4x4_rotate(main_cam->view_mat, main_cam->rot.y, 0.0f, 1.0f, 0.0f);
	tv_mat4x4_rotate(main_cam->view_mat, main_cam->rot.z, 0.0f, 0.0f, 1.0f);
	tv_mat4x4_translate(main_cam->view_mat, main_cam->pos.x, main_cam->pos.y, main_cam->pos.z);

	/* update camera ubo (camera matrices) */
	//glBindBufferRange(GL_UNIFORM_BUFFER, main_cam->uniform_binding_index,
	//	main_cam->ubo, 0, sizeof(TvMat4x4) * 2);
}

void tv_draw_resize_screen(int w, int h)
{
    TvCameraNode* it;
    screen = SDL_SetVideoMode(w, h, 32, SDL_OPENGL | SDL_RESIZABLE);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	for(it = tv_scene_get_cameras(); it != NULL; it = it->next) {
        tv_camera* c = it->cam;
        if(c->ortho) {
            /*
            c->Orthographic(c, 0.0f, c->w, ->h, 
                    0.0f, 0.01f, 100.0f);
            */
        }
        else {
			tv_camera_perspective(c, c->properties.perspective.fov, 
                    (float)w / (float)h,
					c->properties.perspective.nearZ, c->properties.perspective.farZ);
        }
    }
}

void tv_draw_finish_frame()
{
	tv_mat4x4_pop(main_cam->view_mat);
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

void tv_draw_scene()
{
}

void tv_draw_gui()
{
	/* TODO */
    glDisable(GL_DEPTH_TEST);
    /* set up GUI "camera" */

    glEnable(GL_SCISSOR_TEST);
    //Scene_ForeachWidget(Draw_Widget);
    glDisable(GL_SCISSOR_TEST);
}

void tv_draw_texture(TvTexture tex, tv_rect* rect)
{
	GLint loc;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    /* save state */
    tv_mat4x4_push(main_cam->view_mat);
	tv_mat4x4_load_identity(main_cam->view_mat);
	tv_mat4x4_translate(main_cam->view_mat, -rect->x, -rect->y, -1.0f);
    main_cam->view_mat[0] *= rect->w;
    main_cam->view_mat[5] *= rect->h;

    /* use the model's material's shader */
    glUseProgram(tex_mat->program);

	glUniformMatrix4fv(tex_mat->model_mat, 1, GL_FALSE, 
		main_cam->model_mat);
	glUniformMatrix4fv(tex_mat->view_mat, 1, GL_FALSE, 
		main_cam->view_mat);
	glUniformMatrix4fv(tex_mat->projection_mat, 1, GL_FALSE, 
		main_cam->projection_mat);

    /* bind the texture */
    loc = glGetUniformLocation(tex_mat->program, "tex");
    glActiveTexture(GL_TEXTURE0 + 0);
    glUniform1i(loc, 0); 
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glEnable(GL_TEXTURE_2D);

    /* bind attribute array and draw */
    glBindVertexArray(tex_quad->vao);
    tv_draw_arrays(GL_QUADS, 0, 4);
    glBindVertexArray(0);

    /* restore */
	tv_mat4x4_pop(main_cam->view_mat);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

TvDrawTarget* tv_draw_new_target(int w, int h)
{
    GLenum status;
    TvDrawTarget* target = (TvDrawTarget*)malloc(sizeof(TvDrawTarget));

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

void tv_draw_set_target(TvDrawTarget* target)
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

TvTexture tv_draw_target_to_texture(TvDrawTarget* target)
{
    TvTexture t;
    t.id = target->texID;
    return t;
}

void tv_draw_elements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	tv_stats_notify_primitives_rendered(count);
	glDrawElements(mode, count, type, indices);
}

void tv_draw_arrays(GLenum mode, GLint first, GLsizei count) 
{
	tv_stats_notify_primitives_rendered(count);
	glDrawArrays(mode, first, count);
}