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

DEFINE(
    Model,
    ATTRIBUTES(
        struct Component_Material* mat;
        float** attributes;
        int* attributeTable;
        int numAttributes;
    
        GLuint* vboIDs;
        GLuint vao;
        GLuint primitive;
    
        unsigned numVertices;
        struct Component_Model* subgroups;

        void (*LoadPLY)(struct Component_Model* self, char* file);
        void (*BufferAttribute)(struct Component_Model* self, int attribute, float *data);
    ),
    FUNCTIONS(
        static int GetAttributeSize(int id) 
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

        float* GetAttributeBuffer(Component_Model* self, int attribute)
        {
            int i;
            for(i = 0; i < self->numAttributes; ++i) {
                if(self->attributeTable[i] == attribute) {
                    return self->attributes[i];
                }
            }
            return NULL;
        }

        static void AddAttribute(Component_Model* self, int attribute)
        {
            int i;
            int attrSize; 
            attrSize = GetAttributeSize(attribute);

            /* make sure the attribute is supported */
            if(attrSize < 0) {
                /* not supported */
                return;
            }

            /* check if attribute already exists, if it does - nothing to do */
            for(i = 0; i < self->numAttributes; ++i) {
                if(self->attributeTable[i] == attribute) {
                    return;
                }
            }

            /* attribute doesn't exist, create a new buffer for it */
            /* if there are no attributes, malloc */
            if(i == 0) {
                self->attributes = (float**)malloc(sizeof(float*));
                self->attributeTable = (int*)malloc(sizeof(int));
            }
            /* not the first attribute, do realloc */
            else {
                self->attributes = (float**)realloc(self->attributes, (i+1) * sizeof(float*));
                self->attributeTable = (int*)realloc(self->attributeTable, (i+1) * sizeof(int));
            }

            self->attributes[i] = (float*)malloc((self->numVertices) * attrSize * sizeof(float));
            self->attributeTable[i] = attribute;
            ++self->numAttributes;
        }

        static void LoadPLY(Component_Model* self, char* file)
        {
            int* faceBuff;
            int* faceSizeBuff;
            int curFace;

            char* pch;
            FILE* fp;
            int numFaces;

            int* attributesSize;

            int i;
            int j; 
            int k;
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

            self->numAttributes = 0;
            self->attributeTable = NULL;
            self->attributes = NULL;
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
                        self->attributeTable = (int*)realloc(self->attributeTable, (self->numAttributes+1) * sizeof(int));
                        attributesSize = (int*)realloc(attributesSize, (self->numAttributes+1) * sizeof(int));
                        self->attributeTable[self->numAttributes] = attributeID;
                        attributesSize[self->numAttributes] = atoi(strtok(NULL, " \t"));
                        ++self->numAttributes;
                    }
                }
                fgets(lineBuff, 2048, fp);
            }

            /* allocate temporary buffers for reading in all the attributes to */
            tmpAttributes = (float**)malloc(sizeof(float*) * self->numAttributes);
            for(i = 0; i < self->numAttributes; i++) {
                tmpAttributes[i] = (float*)malloc(sizeof(float*) * attributesSize[i] *
                        GetAttributeSize(self->attributeTable[i]));
            }

            /* read lists of each attribute */
            for(i = 0; i < self->numAttributes; ++i) {
                for(j = 0; j < attributesSize[i]; ++j) {
                    int attrSize = GetAttributeSize(self->attributeTable[i]);
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
            self->numVertices = 0;
            for(i = 0, curFace = 0; curFace < numFaces; ++curFace) {
                fgets(lineBuff, 2048, fp);
                faceSizeBuff[curFace] = atoi(strtok(lineBuff, " \t"));

                if(faceSizeBuff[curFace] == 3) {
                    faceBuff[i] = atoi(strtok(NULL, " \t"));
                    faceBuff[i+1] = atoi(strtok(NULL, " \t"));
                    faceBuff[i+2] = atoi(strtok(NULL, " \t"));
                    self->numVertices += 3;
                    i += 3;
                }
                else if(faceSizeBuff[curFace] == 4) {
                    faceBuff[i] = atoi(strtok(NULL, " \t"));
                    faceBuff[i+1] = atoi(strtok(NULL, " \t"));
                    faceBuff[i+2] = atoi(strtok(NULL, " \t"));
                    faceBuff[i+3] = atoi(strtok(NULL, " \t"));
                    self->numVertices += 6;
                    i += 4;
                }
            }

            /* allocate attribute buffers for model */
            self->attributes = (float**)malloc(sizeof(float*) * self->numAttributes);
            for(i = 0; i < self->numAttributes; ++i) {
                self->attributes[i] = (float*)malloc(sizeof(float) * self->numVertices *
                        GetAttributeSize(self->attributeTable[i]));
            }

            /* expand vertex/normal/color information from indexed face information */
            for(i = 0, j = 0, curFace = 0; curFace < numFaces; ++curFace) {
                /* if triangle, just expand each vertex */
                if(faceSizeBuff[curFace] == 3) {
                    for(k = 0; k < self->numAttributes; ++k) {
                        Model_CopyAttribute(self->attributes[k],i,
                                tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                        Model_CopyAttribute(self->attributes[k],i+1,
                                tmpAttributes[k],faceBuff[j+1], self->attributeTable[k]);
                        Model_CopyAttribute(self->attributes[k],i+2,
                                tmpAttributes[k],faceBuff[j+2], self->attributeTable[k]);
                    }
                    i += 3;
                    j += 3;
                }
                /* if quad, do 0,1,2 and 0,2,3 */
                else if(faceSizeBuff[curFace] == 4) {
                    for(k = 0; k < self->numAttributes; ++k) {
                        Model_CopyAttribute(self->attributes[k],i,
                                tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                        Model_CopyAttribute(self->attributes[k],i+1,
                                tmpAttributes[k],faceBuff[j+1], self->attributeTable[k]);
                        Model_CopyAttribute(self->attributes[k],i+2,
                                tmpAttributes[k],faceBuff[j+2], self->attributeTable[k]);

                        Model_CopyAttribute(self->attributes[k],i+3,
                                tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                        Model_CopyAttribute(self->attributes[k],i+4,
                                tmpAttributes[k],faceBuff[j+2], self->attributeTable[k]);
                        Model_CopyAttribute(self->attributes[k],i+5,
                                tmpAttributes[k],faceBuff[j+3], self->attributeTable[k]);
                    }
                    i += 6;
                    j += 4;
                }
                else {
                    fprintf(stderr, "Error: unsupported number of vertices per face\n");
                    break;
                }
            }
            self->primitive = GL_TRIANGLES;

            /* cleanup temporary buffers */
            for(i = 0; i < self->numAttributes; ++i) {
                free(tmpAttributes[i]);
            }
            free(tmpAttributes);
            free(attributesSize);
            free(faceBuff);
            free(faceSizeBuff);
        }

        static void BufferAttribute(Component_Model* self, int attribute, float* data)
        {
            int i;
            float* dst;
            int attrSize = GetAttributeSize(attribute);

            /* add the attribute if it isn't already */
            AddAttribute(self, attribute);

            /* get the destination for the copy */
            dst = GetAttributeBuffer(self, attribute);

            /* copy - say replace this with memcpy, I dare you */
            for(i = 0; i < self->numVertices * attrSize; ++i) {
                dst[i] = data[i];
            }
        }

        static void Start(Component_Model* self)
        {

        }

        static void Update(Component_Model* self) 
        {

        }

        static void Collide(Entity* e)
        {
            puts("model collision");
        }
    ),
    INIT(
        SET(LoadPLY, LoadPLY)
        SET(BufferAttribute, BufferAttribute)
    )
)
#endif
