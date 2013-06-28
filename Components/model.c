#ifndef COMPONENT_MODEL
#define COMPONENT_MODEL

/* attribute types for the model */
#define MODEL_ATTRIBUTE_NONE   0
#define MODEL_ATTRIBUTE_VERTEX 1
#define MODEL_ATTRIBUTE_COLOR  2
#define MODEL_ATTRIBUTE_NORMAL 3
#define MODEL_ATTRIBUTE_TEXCO  4

/* the # of floats each attribute uses */
#define MODEL_ATTRIBUTE_VERTEX_SIZE 3
#define MODEL_ATTRIBUTE_COLOR_SIZE  4
#define MODEL_ATTRIBUTE_NORMAL_SIZE 3
#define MODEL_ATTRIBUTE_TEXCO_SIZE  2

#include "../component.h"

DEFINE_COMPONENT(Model,
    ATTRIBUTE(struct Component_Material* mat)
    ATTRIBUTE(float** attributes)
    ATTRIBUTE(int* attributeTable)
    ATTRIBUTE(int numAttributes)
    
    ATTRIBUTE(GLuint* vboIDs)
    ATTRIBUTE(GLuint vao)
    ATTRIBUTE(GLuint primitive)
    
    ATTRIBUTE(unsigned numVertices)
    ATTRIBUTE(struct Component_Model* subgroups),

    /* FUNCTION(LoadPLY), */
    /* FUNCTION(BufferAttribute) */
)


#ifdef BUILD
static int GetAttributeSize(int id);

void Start(Component_Model* c)
{

}

void Update(Component_Model* c) 
{

}

void Collide(Entity* e)
{
    puts("model collision");
}

void LoadPLY(Component_Model* c, void* data)
{
    int* faceBuff;
    int* faceSizeBuff;
    int curFace;

    char* pch;
    FILE* fp;
    int numFaces;

    int* attributesSize;

    int i, j, k;
    char lineBuff[2048];

    float** tmpAttributes;

    char* file = (char*)data;
    /* open the file and verify it is a .ply file */
    fp = NULL;
    fp = fopen(file, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error: could not open %s\n", file);
    }
    fgets(lineBuff, 2048, fp);
    if(strncmp(lineBuff, "ply", 3) != 0) {
        fprintf(stderr, "Error: file %s is not a .ply file\n", file);
    }

    c->numAttributes = 0;
    c->attributeTable = NULL;
    c->attributes = NULL;
    attributesSize = NULL;

    fgets(lineBuff, 2048, fp);

    /* read the file header - alloc the buffers to store the model data */
    while(strncmp(lineBuff, "end_header", 10) != 0) {
        /* begin parsing line */
        pch = strtok(lineBuff, " \t");

        if(strncmp(pch, "comment", 7) == 0) {
            /* do nothing */
        }
        else if(strncmp(pch, "element", 7) == 0) {
            int attributeID = MODEL_ATTRIBUTE_NONE;

            pch = strtok(NULL, " \t");

            /* vertices */
            if(strncmp(pch, "vertex", 6) == 0) {
                attributeID = MODEL_ATTRIBUTE_VERTEX;
            }
            /* colors */
            else if(strncmp(pch, "color", 5) == 0) {
                attributeID = MODEL_ATTRIBUTE_COLOR;
            }
            /* set number of faces */
            else if(strncmp(pch, "face", 4) == 0) {
                numFaces = atoi(strtok(NULL, " \t"));
            }

            /* reallocate the attribute table and attributes buffers */
            if(attributeID != MODEL_ATTRIBUTE_NONE) {
                c->attributeTable = (int*)realloc(c->attributeTable, (c->numAttributes+1) * sizeof(int));
                attributesSize = (int*)realloc(attributesSize, (c->numAttributes+1) * sizeof(int));
                c->attributeTable[c->numAttributes] = attributeID;
                attributesSize[c->numAttributes] = atoi(strtok(NULL, " \t"));
                ++c->numAttributes;
            }
        }
        fgets(lineBuff, 2048, fp);
    }

    /* allocate temporary buffers for reading in all the attributes to */
    tmpAttributes = (float**)malloc(sizeof(float*) * c->numAttributes);
    for(i = 0; i < c->numAttributes; i++) {
        tmpAttributes[i] = (float*)malloc(sizeof(float*) * attributesSize[i] *
                GetAttributeSize(c->attributeTable[i]));
    }

    /* read lists of each attribute */
    for(i = 0; i < c->numAttributes; ++i) {
        for(j = 0; j < attributesSize[i]; ++j) {
            int attrSize = GetAttributeSize(c->attributeTable[i]);
            fgets(lineBuff, 2048, fp);
            tmpAttributes[i][j*attrSize] = (float)atof(strtok(lineBuff, " \t"));
            for(k = 1; k < attrSize; ++k) {
                tmpAttributes[i][j*attrSize+k] = (float)atof(strtok(NULL, " \t"));
            }
        }
    }

    /* allocate face buffers */
    faceSizeBuff = (int*)malloc(numFaces * sizeof(int));
    faceBuff = (int*)malloc(numFaces * 4 * sizeof(int));

    /* read face list */
    c->numVertices = 0;
    for(i = 0, curFace = 0; curFace < numFaces; ++curFace) {
        fgets(lineBuff, 2048, fp);
        faceSizeBuff[curFace] = atoi(strtok(lineBuff, " \t"));

        if(faceSizeBuff[curFace] == 3) {
            faceBuff[i] = atoi(strtok(NULL, " \t"));
            faceBuff[i+1] = atoi(strtok(NULL, " \t"));
            faceBuff[i+2] = atoi(strtok(NULL, " \t"));
            c->numVertices += 3;
            i += 3;
        }
        else if(faceSizeBuff[curFace] == 4) {
            faceBuff[i] = atoi(strtok(NULL, " \t"));
            faceBuff[i+1] = atoi(strtok(NULL, " \t"));
            faceBuff[i+2] = atoi(strtok(NULL, " \t"));
            faceBuff[i+3] = atoi(strtok(NULL, " \t"));
            c->numVertices += 6;
            i += 4;
        }
    }

    /* allocate attribute buffers for model */
    c->attributes = (float**)malloc(sizeof(float*) * c->numAttributes);
    for(i = 0; i < c->numAttributes; ++i) {
        c->attributes[i] = (float*)malloc(sizeof(float) * c->numVertices *
                GetAttributeSize(c->attributeTable[i]));
    }

    /* expand vertex/normal/color information from indexed face information */
    for(i = 0, j = 0, curFace = 0; curFace < numFaces; ++curFace) {
        /* if triangle, just expand each vertex */
        if(faceSizeBuff[curFace] == 3) {
            for(k = 0; k < c->numAttributes; ++k) {
                Model_CopyAttribute(c->attributes[k],i,
                        tmpAttributes[k],faceBuff[j], c->attributeTable[k]);
                Model_CopyAttribute(c->attributes[k],i+1,
                        tmpAttributes[k],faceBuff[j+1], c->attributeTable[k]);
                Model_CopyAttribute(c->attributes[k],i+2,
                        tmpAttributes[k],faceBuff[j+2], c->attributeTable[k]);
            }
            i += 3;
            j += 3;
        }
        /* if quad, do 0,1,2 and 0,2,3 */
        else if(faceSizeBuff[curFace] == 4) {
            for(k = 0; k < c->numAttributes; ++k) {
                Model_CopyAttribute(c->attributes[k],i,
                        tmpAttributes[k],faceBuff[j], c->attributeTable[k]);
                Model_CopyAttribute(c->attributes[k],i+1,
                        tmpAttributes[k],faceBuff[j+1], c->attributeTable[k]);
                Model_CopyAttribute(c->attributes[k],i+2,
                        tmpAttributes[k],faceBuff[j+2], c->attributeTable[k]);

                Model_CopyAttribute(c->attributes[k],i+3,
                        tmpAttributes[k],faceBuff[j], c->attributeTable[k]);
                Model_CopyAttribute(c->attributes[k],i+4,
                        tmpAttributes[k],faceBuff[j+2], c->attributeTable[k]);
                Model_CopyAttribute(c->attributes[k],i+5,
                        tmpAttributes[k],faceBuff[j+3], c->attributeTable[k]);
            }
            i += 6;
            j += 4;
        }
        else {
            fprintf(stderr, "Error: unsupported number of vertices per face\n");
            break;
        }
    }
    c->primitive = GL_TRIANGLES;

    /* cleanup temporary buffers */
    for(i = 0; i < c->numAttributes; ++i) {
        free(tmpAttributes[i]);
    }
    free(tmpAttributes);
    free(attributesSize);
    free(faceBuff);
    free(faceSizeBuff);
}

void BufferAttribute(Component_Model* c, void* data)
{
    int i;
    float* dst;
    int attrSize = GetAttributeSize(attribute);

    /* add the attribute if it isn't already */
    Model_AddAttribute(m, attribute);

    /* get the destination for the copy */
    dst = GetAttributeBuffer(m, attribute);

    /* copy - say replace this with memcpy, I dare you */
    for(i = 0; i < m->numVertices * attrSize; ++i) {
        dst[i] = data[i];
    }
}

int GetAttributeSize(int id) 
{
    switch(id) {
    case MODEL_ATTRIBUTE_VERTEX:
        return MODEL_ATTRIBUTE_VERTEX_SIZE;
    case MODEL_ATTRIBUTE_COLOR:
        return MODEL_ATTRIBUTE_COLOR_SIZE;
    case MODEL_ATTRIBUTE_NORMAL:
        return MODEL_ATTRIBUTE_NORMAL_SIZE;
    case MODEL_ATTRIBUTE_TEXCO:
        return MODEL_ATTRIBUTE_TEXCO_SIZE;
    case MODEL_ATTRIBUTE_NONE:
        return 0;
    default:
        return -1;
    }
}

#endif
#endif
