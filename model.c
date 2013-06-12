#include "model.h" 
void SetVertex(Vertex n1, Vertex n2);
void SetNormal(Vertex n1, Vertex n2);
void SetColor(Color c1, Color c2);

Model *ModelNew(int numVertices)
{
    puts("making new model");
    Model* m = malloc(sizeof(Model));

    /* create base buffers */
    m->attributes = (float**)malloc(sizeof(float*));
    m->attributes[0] = (float*)malloc(numVertices * (sizeof(float) * ATTRIBUTE_VERTEX_SIZE));
    if(m->attributes == NULL) {
        fprintf(stderr, "Error allocating vertex buffer for new model\n");
    }
    
    m->numFaces = 0; 
    m->numVertices = 0;
    m->numAttributes = 0;
    m->subgroups = NULL;

    return m;
}

void ModelFree(Model* m)
{
    int i;
    for(i = 0; i < m->numAttributes; ++i) {
        free(m->attributes[i]);
    }
    free(m->attributeTable);
    free(m->vboIDs);
}

void ModelAddTriangle(Model *m, Vertex v1, Vertex v2, Vertex v3)
{
    /*
    SetVertex(m->vertices[m->numFaces*3], v1);
    SetVertex(m->vertices[m->numFaces*3+1], v2);
    SetVertex(m->vertices[m->numFaces*3+2], v3);
    m->numFaces++;
    */
}

void ModelAddVertex(Model *m, Vertex v)
{
    float* vertexBuff = ModelGetAttributeBuffer(m, ATTRIBUTE_VERTEX);
    if(vertexBuff) {
        ModelSetAttribute(vertexBuff, m->numVertices, v, 0, ATTRIBUTE_VERTEX);
    }
    m->numVertices++;
}

void ModelAddColor(Model *m, Color c)
{
    float* colorBuff = ModelGetAttributeBuffer(m, ATTRIBUTE_COLOR);
    if(colorBuff) {
        ModelSetAttribute(colorBuff, m->numVertices, c, 0, ATTRIBUTE_COLOR);
    }
}

float* ModelGetAttributeBuffer(Model* m, int attribute)
{
    int i;
    for(i = 0; i < m->numAttributes; ++i) {
        if(m->attributeTable[i] == attribute) {
            return m->attributes[i];
        }
    }
    return NULL;
}

void ModelAddAttribute(Model* m, int attribute)
{
    int attrSize; 
    int i;

    /* make sure the attribute doesn't already exist */
    for(i = 0; i < m->numAttributes; ++i) {
        if(m->attributeTable[i] == attribute) {
            return;
        }
    }
    attrSize = ModelGetAttributeSize(attribute);
    /* make sure the attribute is supported */
    if(attrSize < 0) {
        return;
    }

    m->attributes = (float**)realloc(m->attributes, (m->numAttributes+1) * sizeof(float));
    m->attributeTable = (int*)realloc(m->attributeTable, (m->numAttributes+1) * sizeof(int));
    ++m->numAttributes;
    m->attributeTable[m->numAttributes] = attribute;
    m->attributes[m->numAttributes] = (float*)malloc(m->numVertices * attrSize * sizeof(float));
}

void ModelLoadPLY(Model *m, char *file)
{
    Vertex *vertexBuff;
    Normal *normalBuff;
    Color *colorBuff;
    int *faceBuff;
    int *faceSizeBuff;
    int curFace;

    size_t lineBuffSize;
    char *pch;
    FILE *fp;
    int numFaces, numVertices, numColors;

    int* attributesSize;

    int i, j, k;
    char lineBuff[2048];

    float** tmpAttributes;
    
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

    m->numAttributes = 0;
    m->attributeTable = NULL;
    m->attributes = NULL;
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
            int attributeSize;
            int attributeID = ATTRIBUTE_NONE;

            pch = strtok(NULL, " \t");

            /* vertices */
            if(strncmp(pch, "vertex", 6) == 0) {
                attributeSize = ATTRIBUTE_VERTEX_SIZE;
                attributeID = ATTRIBUTE_VERTEX;
            }
            /* colors */
            else if(strncmp(pch, "color", 5) == 0) {
                attributeSize = ATTRIBUTE_COLOR_SIZE;
                attributeID = ATTRIBUTE_COLOR;
            }
            /* set number of faces */
            else if(strncmp(pch, "face", 4) == 0) {
                numFaces = atoi(strtok(NULL, " \t"));
            }

            /* reallocate the attribute table and attributes buffers */
            if(attributeID != ATTRIBUTE_NONE) {
                m->attributeTable = (int*)realloc(m->attributeTable, (m->numAttributes+1) * sizeof(int));
                attributesSize = (int*)realloc(attributesSize, (m->numAttributes+1) * sizeof(int));
                m->attributeTable[m->numAttributes] = attributeID;
                attributesSize[m->numAttributes] = atoi(strtok(NULL, " \t"));
                ++m->numAttributes;
            }
        }
        fgets(lineBuff, 2048, fp);
    }

    /* allocate temporary buffers for reading in all the attributes to */
    tmpAttributes = (float**)malloc(sizeof(float*) * m->numAttributes);
    for(i = 0; i < m->numAttributes; i++) {
        tmpAttributes[i] = (float*)malloc(sizeof(float*) * attributesSize[i] *
                ModelGetAttributeSize(m->attributeTable[i]));
    }

    /* read lists of each attribute */
    for(i = 0; i < m->numAttributes; ++i) {
        for(j = 0; j < attributesSize[i]; ++j) {
            int attrSize = ModelGetAttributeSize(m->attributeTable[i]);
            fgets(lineBuff, 2048, fp);
            for(k = 0; k < attrSize; ++k) {
                tmpAttributes[i][j*attrSize+k] = (float)atof(strtok(NULL, " \t"));
            }
        }
    }
    puts("SRS");

    faceSizeBuff = (int*)malloc(numFaces * sizeof(int));
    faceBuff = (int*)malloc(numFaces * 4 * sizeof(int));

    /* read face list */
    m->numVertices = 0;
    for(i = 0, curFace = 0; curFace < numFaces; ++curFace) {
        fgets(lineBuff, 2048, fp);
        faceSizeBuff[curFace] = atoi(strtok(lineBuff, " \t"));

        if(faceSizeBuff[curFace] == 3) {
            faceBuff[i] = atoi(strtok(NULL, " \t"));
            faceBuff[i+1] = atoi(strtok(NULL, " \t"));
            faceBuff[i+2] = atoi(strtok(NULL, " \t"));
            m->numVertices += 3;
            i += 3;
        }
        else if(faceSizeBuff[curFace] == 4) {
            faceBuff[i] = atoi(strtok(NULL, " \t"));
            faceBuff[i+1] = atoi(strtok(NULL, " \t"));
            faceBuff[i+2] = atoi(strtok(NULL, " \t"));
            faceBuff[i+3] = atoi(strtok(NULL, " \t"));
            m->numVertices += 6;
            i += 4;
        }
    }

    /* allocate attribute buffers for model */
    m->attributes = (float**)malloc(sizeof(float*) * m->numAttributes);
    for(i = 0; i < m->numAttributes; ++i) {
        m->attributes[i] = (float*)malloc(sizeof(float) * m->numVertices *
                ModelGetAttributeSize(m->attributeTable[i]));
    }

    /* expand vertex/normal/color information from indexed face information */
    for(i = 0, j = 0, curFace = 0; curFace < numFaces; ++curFace) {
        /* if triangle, just expand each vertex */
        if(faceSizeBuff[curFace] == 3) {
            for(k = 0; k < m->numAttributes; ++k) {
                ModelSetAttribute(m->attributes[k],i,
                        tmpAttributes[k],faceBuff[j], m->attributeTable[k]);
                ModelSetAttribute(m->attributes[k],i+1,
                        tmpAttributes[k],faceBuff[j+1], m->attributeTable[k]);
                ModelSetAttribute(m->attributes[k],i+2,
                        tmpAttributes[k],faceBuff[j+2], m->attributeTable[k]);
            }
            i += 3;
            j += 3;
        }
        /* if quad, do 0,1,2 and 0,2,3 */
        else if(faceSizeBuff[curFace] == 4) {
            for(k = 0; k < m->numAttributes; ++k) {
                ModelSetAttribute(m->attributes[k],i,
                        tmpAttributes[k],faceBuff[j], m->attributeTable[k]);
                ModelSetAttribute(m->attributes[k],i+1,
                        tmpAttributes[k],faceBuff[j+1], m->attributeTable[k]);
                ModelSetAttribute(m->attributes[k],i+2,
                        tmpAttributes[k],faceBuff[j+2], m->attributeTable[k]);

                ModelSetAttribute(m->attributes[k],i+3,
                        tmpAttributes[k],faceBuff[j], m->attributeTable[k]);
                ModelSetAttribute(m->attributes[k],i+4,
                        tmpAttributes[k],faceBuff[j+2], m->attributeTable[k]);
                ModelSetAttribute(m->attributes[k],i+5,
                        tmpAttributes[k],faceBuff[j+3], m->attributeTable[k]);
            }
            i += 6;
            j += 4;
        }
        else {
            fprintf(stderr, "Error: unsupported number of vertices per face\n");
            break;
        }
    }
    m->primitive = GL_TRIANGLES;

    /* cleanup temporary buffers */
    for(i = 0; i < m->numAttributes; ++i) {
        free(tmpAttributes[i]);
    }
    free(tmpAttributes);
    free(attributesSize);
    free(faceBuff);
    free(faceSizeBuff);
}

void ModelAddTriangle2(Model *m, float x1,float y1, float x2,float y2, float x3,float y3)
{
    
}

void SetVertex(Vertex v1, Vertex v2) 
{
    v1[0] = v2[0];
    v1[1] = v2[1];
    v1[2] = v2[2];
}

void SetNormal(Vertex n1, Vertex n2) 
{
    n1[0] = n2[0];
    n1[1] = n2[1];
    n1[2] = n2[2];
}

void SetColor(Color c1, Color c2)
{
    c1[0] = c2[0];
    c1[1] = c2[1];
    c1[2] = c2[2];
    c1[3] = c2[3];
}

void ModelSetAttribute(float* dst, int dstOffset, float* src, int srcOffset, int type)
{
    int i;
    srcOffset *= ModelGetAttributeSize(type);
    dstOffset *= ModelGetAttributeSize(type);

    if(srcOffset < 0) {
        return;
    }
    for(i = 0; i < ModelGetAttributeSize(type); ++i) {
        dst[srcOffset+i] = src[dstOffset+i];
    }
}

int ModelGetAttributeSize(int id) 
{
    switch(id) {
    case ATTRIBUTE_VERTEX:
        return ATTRIBUTE_VERTEX_SIZE;
    case ATTRIBUTE_COLOR:
        return ATTRIBUTE_COLOR_SIZE;
    case ATTRIBUTE_NORMAL:
        return ATTRIBUTE_NORMAL_SIZE;
    case ATTRIBUTE_NONE:
        return 0;
    default:
        return -1;
    }
}

