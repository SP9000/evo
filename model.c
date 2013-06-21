#include "model.h" 

Model *Model_New()
{
    Model* m = malloc(sizeof(Model));

    m->numFaces = 0; 
    m->numVertices = 0;
    m->numAttributes = 0;
    m->subgroups = NULL;

    return m;
}

void Model_Free(Model* m)
{
    int i;
    for(i = 0; i < m->numAttributes; ++i) {
        free(m->attributes[i]);
    }
    free(m->attributeTable);
    free(m->vboIDs);
}

float* Model_GetAttributeBuffer(Model* m, int attribute)
{
    int i;
    for(i = 0; i < m->numAttributes; ++i) {
        if(m->attributeTable[i] == attribute) {
            return m->attributes[i];
        }
    }
    return NULL;
}

void Model_AddAttribute(Model* m, int attribute)
{
    int i;
    int attrSize; 
    attrSize = Model_GetAttributeSize(attribute);

    /* make sure the attribute is supported */
    if(attrSize < 0) {
        /* not supported */
        return;
    }

    /* check if attribute already exists, if it does - nothing to do */
    for(i = 0; i < m->numAttributes; ++i) {
        if(m->attributeTable[i] == attribute) {
            return;
        }
    }

    /* attribute doesn't exist, create a new buffer for it */
    m->attributes = (float**)realloc(m->attributes, (i+1) * sizeof(float*));
    m->attributes[i] = (float*)malloc((m->numVertices) * attrSize * sizeof(float));
    m->attributeTable = (int*)realloc(m->attributeTable, (i+1) * sizeof(int));
    m->attributeTable[i] = attribute;
    ++m->numAttributes;
}


void Model_LoadPLY(Model *m, char *file)
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
            int attributeID = MODEL_ATTRIBUTE_NONE;

            pch = strtok(NULL, " \t");

            /* vertices */
            if(strncmp(pch, "vertex", 6) == 0) {
                attributeSize = MODEL_ATTRIBUTE_VERTEX_SIZE;
                attributeID = MODEL_ATTRIBUTE_VERTEX;
            }
            /* colors */
            else if(strncmp(pch, "color", 5) == 0) {
                attributeSize = MODEL_ATTRIBUTE_COLOR_SIZE;
                attributeID = MODEL_ATTRIBUTE_COLOR;
            }
            /* set number of faces */
            else if(strncmp(pch, "face", 4) == 0) {
                numFaces = atoi(strtok(NULL, " \t"));
            }

            /* reallocate the attribute table and attributes buffers */
            if(attributeID != MODEL_ATTRIBUTE_NONE) {
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
                Model_GetAttributeSize(m->attributeTable[i]));
    }

    /* read lists of each attribute */
    for(i = 0; i < m->numAttributes; ++i) {
        for(j = 0; j < attributesSize[i]; ++j) {
            int attrSize = Model_GetAttributeSize(m->attributeTable[i]);
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
                Model_GetAttributeSize(m->attributeTable[i]));
    }

    /* expand vertex/normal/color information from indexed face information */
    for(i = 0, j = 0, curFace = 0; curFace < numFaces; ++curFace) {
        /* if triangle, just expand each vertex */
        if(faceSizeBuff[curFace] == 3) {
            for(k = 0; k < m->numAttributes; ++k) {
                Model_CopyAttribute(m->attributes[k],i,
                        tmpAttributes[k],faceBuff[j], m->attributeTable[k]);
                Model_CopyAttribute(m->attributes[k],i+1,
                        tmpAttributes[k],faceBuff[j+1], m->attributeTable[k]);
                Model_CopyAttribute(m->attributes[k],i+2,
                        tmpAttributes[k],faceBuff[j+2], m->attributeTable[k]);
            }
            i += 3;
            j += 3;
        }
        /* if quad, do 0,1,2 and 0,2,3 */
        else if(faceSizeBuff[curFace] == 4) {
            for(k = 0; k < m->numAttributes; ++k) {
                Model_CopyAttribute(m->attributes[k],i,
                        tmpAttributes[k],faceBuff[j], m->attributeTable[k]);
                Model_CopyAttribute(m->attributes[k],i+1,
                        tmpAttributes[k],faceBuff[j+1], m->attributeTable[k]);
                Model_CopyAttribute(m->attributes[k],i+2,
                        tmpAttributes[k],faceBuff[j+2], m->attributeTable[k]);

                Model_CopyAttribute(m->attributes[k],i+3,
                        tmpAttributes[k],faceBuff[j], m->attributeTable[k]);
                Model_CopyAttribute(m->attributes[k],i+4,
                        tmpAttributes[k],faceBuff[j+2], m->attributeTable[k]);
                Model_CopyAttribute(m->attributes[k],i+5,
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

void Model_CopyAttribute(float* dst, int dstOffset, float* src, int srcOffset, int type)
{
    int i;
    int size = Model_GetAttributeSize(type);
    srcOffset *= size;
    dstOffset *= size;

    if(srcOffset < 0) {
        return;
    }
    for(i = 0; i < size; ++i) {
        dst[dstOffset+i] = src[srcOffset+i];
    }
}

void Model_SetAttribute(Model* m, int attribute, int offset, float* val)
{
    int attrSize; 
    int i;
    int exists = -1;

    attrSize = Model_GetAttributeSize(attribute);

    /* make sure the attribute is supported */
    if(attrSize < 0) {
        /* not supported */
        return;
    }

    /* make sure attribute already exists, if it does i=offset in table */
    for(i = 0; i < m->numAttributes; ++i) {
        if(m->attributeTable[i] == attribute) {
            exists = i;
            break;
        }
    }
    /* if the exists, set the value */
    if(exists >= 0) {
        /* add the attribute */
        Model_CopyAttribute(m->attributes[i],offset, val,0, attribute);
    }
}

void Model_BufferAttribute(Model* m, int attribute, float* data)
{
    int i;
    float* dst;
    /* add the attribute if it isn't already */
    Model_AddAttribute(m, attribute);

    /* get the destination for the copy */
    dst = Model_GetAttributeBuffer(m, attribute);

    /* copy - say replace this with memcpy, I dare you */
    for(i = 0; i < m->numVertices * Model_GetAttributeSize(attribute); ++i) {
        dst[i] = data[i];
    }
}

void Model_SetMaterial(Model* m, Material* mat)
{
    m->mat = *mat;
}

/*****************************************************************************/
/*                           local functions                                 */
/*****************************************************************************/
int Model_GetAttributeSize(int id) 
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

