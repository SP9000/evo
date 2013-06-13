#include "modelgen.h"

Model* GenText(char *text, float w, float h)
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

    Model* m = ModelNew(1000);
    /* add all vertices contained in the given string */
    float x = 3.0f;
    float y = 20.0f;
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
            ModelAddAttribute(m, ATTRIBUTE_COLOR, color);
            ModelAddAttribute(m, ATTRIBUTE_VERTEX, v);

            /* (x2, y2) */
            v[0] = (font[idx][i+2] + x) * scale;
            v[1] = (font[idx][i+3] + y) * scale;
            ModelAddAttribute(m, ATTRIBUTE_COLOR, color);
            ModelAddAttribute(m, ATTRIBUTE_VERTEX, v);
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
    DrawOptimizeModel(m);

    return m;
}

Model* GenRect(float x, float y, float z, float w, float h, Color c)
{
    float scale = 10;
    Vertex v;

    Model* m = ModelNew(4);
    v[2] = z;

    v[0] = x * scale;
    v[1] = y * scale;
    ModelAddAttribute(m, ATTRIBUTE_COLOR, c);
    ModelAddAttribute(m, ATTRIBUTE_VERTEX, v);
    v[0] = x * scale + w * scale;
    v[1] = y * scale;
    ModelAddAttribute(m, ATTRIBUTE_COLOR, c);
    ModelAddAttribute(m, ATTRIBUTE_VERTEX, v);
    v[0] = x * scale + w * scale;
    v[1] = y * scale + h * scale;
    ModelAddAttribute(m, ATTRIBUTE_COLOR, c);
    ModelAddAttribute(m, ATTRIBUTE_VERTEX, v);
    v[0] = x * scale;
    v[1] = y * scale + h * scale;
    ModelAddAttribute(m, ATTRIBUTE_COLOR, c);
    ModelAddAttribute(m, ATTRIBUTE_VERTEX, v);

    m->primitive = GL_QUADS;
    DrawOptimizeModel(m);
    return m;
}

