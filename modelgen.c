#if 0
#include "modelgen.h"

Model* ModelGen_Text(char *text, float w, float h)
{
    int i;
    int modelOff;
    Model* m;

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
    m = Model_New();
    for(i = 0; i < strlen(text); ++i) {
        m->numVertices += charSizes[i]*2;
    }
    Model_AddAttribute(m, MODEL_ATTRIBUTE_VERTEX);
    Model_AddAttribute(m, MODEL_ATTRIBUTE_COLOR);

    /* add all vertices contained in the given string */
    float x = 3.0f;
    float y = 20.0f;
    float scale = 3;
    modelOff = 0;
    while(*text) {
        if(*text != ' ') {
            int idx = (*text) - 'A';
            Vertex v;
            v[2] = .9f;

            /* add all vertices that compose this character */
            for(i = 0; i < charSizes[idx]*4; i += 4) {
                /* (x1, y1) */
                v[0] = (font[idx][i] + x) * scale;   
                v[1] = (font[idx][i+1] + y) * scale;
                Model_SetAttribute(m, MODEL_ATTRIBUTE_COLOR, modelOff, color);
                Model_SetAttribute(m, MODEL_ATTRIBUTE_VERTEX, modelOff, v);

                /* (x2, y2) */
                v[0] = (font[idx][i+2] + x) * scale; 
                v[1] = (font[idx][i+3] + y) * scale; 
                Model_SetAttribute(m, MODEL_ATTRIBUTE_COLOR, modelOff+1, color);
                Model_SetAttribute(m, MODEL_ATTRIBUTE_VERTEX, modelOff+1, v);
                
                modelOff += 2;
            }
        }
        x += scale;
        /* keep text inside bounds given */
        if(x >= w) {
            y -= scale;
            x = 0.0f;
        }
        /* break if no more room left for text */
        if(y >= h) {
            break;
        }
        text++;
    }
    m->primitive = GL_LINES;
    Draw_OptimizeModel(m);
    return m;
}

Model* ModelGen_Rect(float x, float y, float z, float w, float h)
{
    Model* m;
    float scale = 1.0f;
    float sx = x*scale;
    float sy = y*scale;
    float sw = w*scale;
    float sh = h*scale;

    float v[] = {sx,    sy, z,
                 sx+sw, sy, z,
                 sx+sw, sy+sh, z,
                 sx,    sy+sh, z};
    /* create model and buffer vertex data */
    m = Model_New();
    m->numVertices = 4;
    Model_BufferAttribute(m, MODEL_ATTRIBUTE_VERTEX, v);
    m->primitive = GL_QUADS;
    return m;
}

Model* ModelGen_Box(float x, float y, float z, float w, float h, float d)
{
    Model* m;
    float v[] = {x,y,z, x+w,y,z, x+w,y+h,z, x,y+h,z,         /* front face  */
                 x,y,z, x,y,z+d, x,y+h,z+d, x,y+h,z,         /* left face   */
                 x,y,z+d, x+w,y,z+d, x+w,y+h,z+d, x,y+h,z+d, /* back face   */
                 x+w,y,z, x+w,y,z+d, x+w,y+h,z+d, x+w,y+h,z, /* right face  */
                 x,y+h,z, x+w,y+h,z, x+w,y+h,z+d, x,y+h,z+d, /* top face    */
                 x,y,z, x+w,y,z, x+w,y,z+d, x,y,z+d          /* bottom face */
    };  
    m = Model_New();
    m->numVertices = 24;
    m->primitive = GL_QUADS;
    Model_BufferAttribute(m, MODEL_ATTRIBUTE_VERTEX, v);
    return m;
}

/*
Model* ModelGen_Circle(float radius, int resolution)
{
    float theta;
    int i;
    float dTheta = resolution / (2 * M_PI);
    for(i = 0, theta = 0.0f; i < resolution; ++i, theta += 
}

*/
#endif
