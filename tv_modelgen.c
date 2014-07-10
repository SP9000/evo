#include "tv_modelgen.h"


typedef struct modelgen_vertex {
	tv_vector4 pos;
	tv_vector4 color;
}modelgen_vertex;

/*****************************************************************************/
void write_header(FILE* fp)
{
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
}
void write_property(FILE* fp, char* type, char* name)
{
    fprintf(fp, "property %s %s\n", type, name);
}
void write_end_header(FILE* fp)
{
    fprintf(fp, "end_header\n");
}
void write_element(FILE* fp, char* name, int size)
{
    fprintf(fp, "element %s %d\n", name, size);
    if(strncmp(name, "vertex", 7) == 0) {
        write_property(fp, "float", "x");
        write_property(fp, "float", "y");
        write_property(fp, "float", "z");
    }
    else if(strncmp(name, "color", 6) == 0) {
        write_property(fp, "float", "r");
        write_property(fp, "float", "g");
        write_property(fp, "float", "b");
        write_property(fp, "float", "a");
    }
}
FILE* open_file(char* dir, char* file)
{
    FILE* fp;
    char path[512];
    strcpy(path, dir);
    strcat(path, file);
    fp = fopen(path, "w");
    if(!fp) {
        fprintf(stderr, "Error: couldn't open file %s for writing\n", path);
    }
    return fp;
}

/*****************************************************************************/
void ModelGen_Charset()
{
    int i;
    int j;

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
    float font[26][5*4] = {
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

    for(i = 0; i < 26; ++i) {
        FILE* fp;
        char dir[] = "StdAssets/Models/Charset/";
        char file[31];

        strcpy(file, " .ply");
        file[0] = 'A'+i;
        if(!(fp = open_file(dir, file))) {
            return;
        }
        write_header(fp);
        write_element(fp, "vertex", charSizes[i]*2);
        write_element(fp, "face", charSizes[i]);
        write_end_header(fp);

        for(j = 0; j < charSizes[i]*2; ++j) {
            fprintf(fp, "%f %f 0.0\n", font[i][j*2], font[i][j*2+1]);
        }
        for(j = 0; j < charSizes[i]; ++j) {
            fprintf(fp, "2 %d %d\n", j*2, j*2+1);
        }
        fclose(fp);
    }
}

/*****************************************************************************/
void ModelGen_GUI()
{
    FILE* fp;
    char dir[] = "StdAssets/Models/GUI/";

    /* Button.ply */
    if(!(fp = open_file(dir, "Button.ply"))) {
        return;
    }
    write_element(fp, "vertex", 4);
    write_element(fp, "face", 1);
    write_end_header(fp);
    fprintf(fp, "%f %f %f\n%f %f %f\n%f %f %f\n%f %f %f\n", 
            0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, 0.0f,1.0f,0.0f);
    fprintf(fp, "4 %d %d %d %d", 0, 1, 2, 3);
    fclose(fp);
}

tv_model *tv_modelgen_quad(tv_vector2 dimensions, tv_model_vertex format)
{
	tvfloat quad_vertices[] = {
		0,0,0,
		1,0,0,
		1,1,0,
		1,1,0,
		0,1,0,
		0,0,0
	};
	tv_model_attribute vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 4, 0},
		{TV_MODEL_PROPERTY_FLOAT, 4, 4*sizeof(tvfloat)}};	
	modelgen_vertex my_vertex;
	tv_model *model = tv_model_new();
	tvuint i;

	tv_model_vertex_format(model, 2, vertex_properties);

	for(i = 0; i < 6; ++i) {
		my_vertex.pos.x = quad_vertices[i*3] * dimensions.x;
		my_vertex.pos.y = quad_vertices[i*3+1] * dimensions.y;
		my_vertex.pos.z = 0.0f;
		my_vertex.pos.w = 1.0f;
		my_vertex.color.x = 1.0f;
		my_vertex.color.y = 1.0f;
		my_vertex.color.z = 1.0f;
		my_vertex.color.w = 0.5F;
		tv_model_append_vertex(model, &my_vertex);
	}
	model->primitive = GL_TRIANGLES;
	tv_model_optimize(model, TRUE, FALSE);
	return model;
}

tv_model *tv_modelgen_tetrahedron(tv_vector3 dimensions, tv_model_vertex format)
{
	tvfloat cube_vertices[] = {
		// Front face
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		// Back face
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		// Top face
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		// Bottom face
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		// Right face
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		// Left face
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f
	};
	tvuint cube_indices[] = {
		0,  1,  2,      0,  2,  3,    // front
		4,  5,  6,      4,  6,  7,    // back
		8,  9,  10,     8,  10, 11,   // top
		12, 13, 14,     12, 14, 15,   // bottom
		16, 17, 18,     16, 18, 19,   // right
		20, 21, 22,     20, 22, 23    // left
	};
	tv_model_attribute vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 4, 0},
		{TV_MODEL_PROPERTY_FLOAT, 4, 4*sizeof(tvfloat)}};	
	modelgen_vertex my_vertex;
	tv_model *model = tv_model_new();
	tvuint i;

	tv_model_vertex_format(model, 2, vertex_properties);
	for(i = 0; i < 24*3; i += 3) {
		my_vertex.pos.x = cube_vertices[i] * dimensions.x;
		my_vertex.pos.y = cube_vertices[i+1] * dimensions.y;
		my_vertex.pos.z = cube_vertices[i+2] * dimensions.z;
		my_vertex.pos.w = 1.0f;
		my_vertex.color.x = 1.0f;
		my_vertex.color.y = 1.0f;
		my_vertex.color.z = 1.0f;
		my_vertex.color.w = 0.5f;
		tv_model_append_vertex(model, &my_vertex);
	}
	for(i = 0; i < 36; ++i) {
		tv_model_append_indices1(model, cube_indices[i]);
	}
	model->primitive = GL_TRIANGLES;
	tv_model_optimize(model, TRUE, TRUE);
	return model;
}
