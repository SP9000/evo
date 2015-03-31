#include "widgetmodels.h"
#define FONT_SCALE 0.01f
#define FONT_LETTER_GAP 0.5f

typedef struct {
	tv_vector3 pos;
	tv_vector4 color;
}gui_vertex;

tv_model *tv_gui_model_text(tvchar *text, tvuint w, tv_vector4 color)
{
	gui_vertex vertex;
	tvint i, j;

	/* # of lines in each character */
    tvint char_sizes[] = {
		4, /* . */
		1, /* / */
		4, /* 0 */
		1, /* 1 */
		4, /* 2 */
		4, /* 3 */
		3, /* 4 */
		5, /* 5 */
		5, /* 6 */
		2, /* 7 */
		5, /* 8 */
		5, /* 9 */
		2, /* : */
		2, /* ; */
		2, /* < */
		2, /* = */
		2, /* > */
		5, /* ? */
		1, /* @ TODO */
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
    tvfloat font[26+19][5*4] = {
	{-0.1f,-0.9f,0.1f,-0.9f, 0.1f,-0.9f,0.1f,-1, 0.1f,-1,-0.1f,-1, -0.1f,-1,-0.1f,-0.9f},	/* . */
	{1,1,-1,-1},														/* / */
    {-1,1,1,1, -1,1,-1,-1, -1,-1,1,-1, 1,1,1,-1},                       /* 0 */
	{0,1,0,-1},															/* 1 */
	{-1,0,0,1, 0,1,1,0, 1,0,-1,-1, -1,-1,1,-1},							/* 2 */
	{-1,1,1,1, -1,0,1,0, -1,-1,1,-1, 1,1,1,-1},							/* 3 */
	{-1,1,-1,0, -1,0,1,0, 1,1,1,-1},									/* 4 */
	{1,1,-1,1, -1,1,-1,0, -1,0,1,0, 1,0,1,-1, 1,-1,-1,-1},				/* 5 */
	{1,1,-1,1, -1,1,-1,-1, -1,-1,1,-1, 1,-1,1,0, 1,0,-1,0},				/* 6 */
	{-1,1,1,1, 1,1,-1,-1},												/* 7 */
	{-1,-1,-1,1, -1,1,1,1, 1,1,1,-1, 1,-1,-1,-1, -1,0,1,0},				/* 8 */
	{-1,0,-1,1, -1,1,1,1, 1,1,1,-1, 1,-1,-1,-1, -1,0,1,0},				/* 9 */
	{0,0.75f,0,0.25f, 0,-0.75f,0,0.25f},								/* : */
	{0,0.75f,0,0.25f, 0,-0.75f,-0.25f,0.25f},							/* ; */
	{1,1,-1,0, -1,0,1,-1},												/* < */
	{-1,0.5f,1,0.5f, -1,-0.5f,1,-0.5f},									/* = */
	{-1,1,1,0, 1,0,-1,-1},												/* > */
	{-1,1,1,1, 1,1,1,0, 1,0,0,0, 0,0,0,-0.25f, 0,-0.75f,0,-1},			/* ? */
	{-1,-1,1,1},														/* @ TODO */
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
	{1,1,-1,0.5f, -1,0.5f,1,-0.5f, 1,-0.5f,-1,-1},						/* S */
    {-1,1,1,1, 0,1,0,-1},                                               /* T */
    {-1,1,-1,-1, -1,-1,1,-1, 1,1,1,-1},                                 /* U */
    {-1,1,0,-1, 0,-1,1,1},                                              /* V */
    {-1,1,-1,-1, -1,-1,0,0, 0,0,1,-1, 1,-1,1,1},                        /* W */
    {-1,1,1,-1, -1,-1,1,1},                                             /* X */
    {-1,1,0,0, 1,1,0,0, 0,0,0,-1},                                      /* Y */
    {-1,1,1,1, 1,1,-1,-1, -1,-1, 1,-1}                                  /* Z */
    };
	tv_model_attribute vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 3, 0},
		{TV_MODEL_PROPERTY_FLOAT, 4, 3*sizeof(TV_MODEL_PROPERTY_FLOAT)}
	};
	tv_model *model = tv_model_new();
	tvint row = 0;
	tvint col = 0;
	vertex.color = color;
	vertex.pos.z = 0.0f;

	tv_model_vertex_format(model, 2, vertex_properties);
	for(i = 0; i < (tvint)strlen(text); ++i, ++col) {
		tvuint character;
		/* check control codes */
		if(text[i] == ' ') {
			continue;
		}
		else if(text[i] == '\n') {
			++row;
			/* set to -1 because it will be incremeneted */
			col = -1;
			continue;
		}
		else if(i && col > (tvint)w) {
			++row;
			col = 0;
		}
		character = text[i] - '.';
		for(j = 0; j < char_sizes[character]; ++j) {
			vertex.pos.x = (tvfloat)col * (2.0f + FONT_LETTER_GAP) * FONT_SCALE + (1.0f + font[character][j*4]) * FONT_SCALE;
			vertex.pos.y = (tvfloat)row * (2.0f + FONT_LETTER_GAP) * FONT_SCALE + (font[character][j*4+1]) * -FONT_SCALE + FONT_SCALE;
			tv_model_append_vertex(model, &vertex);
			
			vertex.pos.x = (tvfloat)col * (2.0f + FONT_LETTER_GAP) * FONT_SCALE + (1.0f + font[character][j*4+2]) * FONT_SCALE;
			vertex.pos.y = (tvfloat)row * (2.0f + FONT_LETTER_GAP) * FONT_SCALE + (font[character][j*4+3]) * -FONT_SCALE + FONT_SCALE;
			tv_model_append_vertex(model, &vertex);
		}
	}
	model->primitive = GL_LINES;
	tv_model_optimize(model, TRUE, TRUE);
	return model;
}

tv_model *tv_gui_model_border(tvfloat w, tvfloat h, tv_vector4 color)
{
	gui_vertex vertex;
	tv_model_attribute vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 3, 0},
		{TV_MODEL_PROPERTY_FLOAT, 3, 3*sizeof(TV_MODEL_PROPERTY_FLOAT)}
	};
	tv_model *model = tv_model_new();

    vertex.pos.x = 0.0f;
    vertex.pos.y = 0.0f;
    vertex.pos.z = 0.0f;
	vertex.color = color;

	tv_model_vertex_format(model, 2, vertex_properties);

	tv_model_append_vertex(model, &vertex);
	vertex.pos.x += w;
	tv_model_append_vertex(model, &vertex);
	vertex.pos.y += h;
	tv_model_append_vertex(model, &vertex);
	vertex.pos.x -= w;
	tv_model_append_vertex(model, &vertex);

	model->primitive = GL_LINE_LOOP;
	tv_model_optimize(model, TRUE, TRUE);

	return model;
}
