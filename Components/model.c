/*****************************************************************************/
/* model.h                                                                   */
/* The model component.                                                      */
/* Contains definitions for the model and the types it uses as well as       */
/* functions to create/load/manage models. Models are used for nearly all    */
/* objects rendered in the engine including text and other GUI functionaliy. */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: April, 2013                                                      */
/*****************************************************************************/
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

/**
 * Model structure.
 * This is holds the data for the data for the position, normal, and color for
 * each vertex in the model.
 */
COMPONENT(Model,
    /* the file to load the model properties from */
    PUBLIC char* file;

    /* the transform component associated with this model. */
    struct Component_Transform* transform;
    /* the material associated with this model. */
    struct Component_Material* mat;
    /* buffers for each attribute of the model */
    float** attributes;
    /* a table of MODEL_ATTRIBUTE_* ID's to tell the contents of attributes */
    int* attributeTable;
    /* the # of unique per-vertex attributes */
    int numAttributes;

    /* VBO ID's for rendering with OpenGL-2.0+. */
    GLuint* vboIDs;
    GLuint vao;

    /* The type of primitive to render as. e.g. GL_LINES, GL_TRIANGLES... */
    GLuint primitive;
    /* The offset of the next unassigned vertex. */
    unsigned int numVertices;
    /* Submodels of this model. For example: a leg may be a subgroup of body */
    struct tagModel* subgroups;

    void (*LoadPLY)(struct Component_Model* self, char* file);
    void (*BufferAttribute)(struct Component_Model* self, int attribute, float *data);
    int  (*GetAttributeSize)(int id);
    void (*AddAttribute)(struct Component_Model* self, int attribute);
    void (*SetAttribute)(struct Component_Model* self, int attribute, int offset, float* val);
    void (*Free)(struct Component_Model* self);
)


#ifdef BUILD
    /**
     * Get the number of floats the attribute of the given ID uses.
     * @param id the ID of the attribute to get the number of floats used by.
     * @return the number of floats the given attribute uses or negative if unknown
     *  type.
     */
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

    /**
     * Get the address of the specified attribute's buffer in the given model.
     * @param m the mode to find the buffer in.
     * @param attribute the attribute who's buffer is to be located.
     * @return the location of the buffer for the given attribute or NULL if the
     *  attribute doesn't exist in the given model.
     */
    static float* GetAttributeBuffer(Component_Model* self, int attribute)
    {
        int i;
        for(i = 0; i < self->numAttributes; ++i) {
            if(self->attributeTable[i] == attribute) {
                return self->attributes[i];
            }
        }
        return NULL;
    }



    /**
     * Add an attribute of the the given type to the model.
     * This function allocates a buffer of numVertices size for the given attribute.
     * to determine where to place the other attributes.
     * @param m the model to add the attribute to.
     * @param val the value to append to the given attribute buffer.
     */
    static void AddAttribute(Component_Model* self, int attribute)
    {
        int i;
        int attrSize; 
        attrSize = self->GetAttributeSize(attribute);

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

    /* helper function to set an attribute at a given offset */
    static void CopyAttribute(float* dst, int dstOffset, float* src, int srcOffset, int type)
    {
        int i;
        int size = GetAttributeSize(type);
        srcOffset *= size;
        dstOffset *= size;

        if(srcOffset < 0) {
            return;
        }
        for(i = 0; i < size; ++i) {
            dst[dstOffset+i] = src[srcOffset+i];
        }
    }

    /**
     * Set the specified attribute of the given model to the specified value.
     * @param m the model to set the attribute of.
     * @param attribute the type of the attribute to set.
     * @param offset the vertex-offset of the attribute to set.
     * @param val the value to set the attribute to.
     */
    static void SetAttribute(Component_Model* self, int attribute, int offset, float* val) 
    {
        int attrSize; 
        int i;
        int exists = -1;

        attrSize = self->GetAttributeSize(attribute);

        /* make sure the attribute is supported */
        if(attrSize < 0) {
            /* not supported */
            return;
        }

        /* make sure attribute already exists, if it does i=offset in table */
        for(i = 0; i < self->numAttributes; ++i) {
            if(self->attributeTable[i] == attribute) {
                exists = i;
                break;
            }
        }
        /* if the exists, set the value */
        if(exists >= 0) {
            /* add the attribute */
            CopyAttribute(self->attributes[i],offset, val,0, attribute);
        }
    }

    /**
      * Load a .ply model from file.
      * @param m the model to load into.
      * @param file the file to load the model from.
      */
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
                    self->GetAttributeSize(self->attributeTable[i]));
        }

        /* read lists of each attribute */
        for(i = 0; i < self->numAttributes; ++i) {
            for(j = 0; j < attributesSize[i]; ++j) {
                int attrSize = self->GetAttributeSize(self->attributeTable[i]);
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
                    self->GetAttributeSize(self->attributeTable[i]));
        }

        /* expand vertex/normal/color information from indexed face information */
        for(i = 0, j = 0, curFace = 0; curFace < numFaces; ++curFace) {
            /* if triangle, just expand each vertex */
            if(faceSizeBuff[curFace] == 3) {
                for(k = 0; k < self->numAttributes; ++k) {
                    CopyAttribute(self->attributes[k],i,
                            tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                    CopyAttribute(self->attributes[k],i+1,
                            tmpAttributes[k],faceBuff[j+1], self->attributeTable[k]);
                    CopyAttribute(self->attributes[k],i+2,
                            tmpAttributes[k],faceBuff[j+2], self->attributeTable[k]);
                }
                i += 3;
                j += 3;
            }
            /* if quad, do 0,1,2 and 0,2,3 */
            else if(faceSizeBuff[curFace] == 4) {
                for(k = 0; k < self->numAttributes; ++k) {
                    CopyAttribute(self->attributes[k],i,
                            tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                    CopyAttribute(self->attributes[k],i+1,
                            tmpAttributes[k],faceBuff[j+1], self->attributeTable[k]);
                    CopyAttribute(self->attributes[k],i+2,
                            tmpAttributes[k],faceBuff[j+2], self->attributeTable[k]);

                    CopyAttribute(self->attributes[k],i+3,
                            tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                    CopyAttribute(self->attributes[k],i+4,
                            tmpAttributes[k],faceBuff[j+2], self->attributeTable[k]);
                    CopyAttribute(self->attributes[k],i+5,
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

    /**
     * Add the buffer of the given attribute type to the model.
     * @param m the model to add the attribute to.
     * @param attribute the type of the attribute to add.
     * @param data the data to buffer (an array of floats ATTRIBUTE_XX_SIZE *
     *   numVertices) floats long.
     */
    static void BufferAttribute(Component_Model* self, int attribute, float* data)
    {
        int i;
        float* dst;
        int attrSize = self->GetAttributeSize(attribute);

        /* add the attribute if it isn't already */
        AddAttribute(self, attribute);

        /* get the destination for the copy */
        dst = GetAttributeBuffer(self, attribute);

        /* copy - say replace this with memcpy, I dare you */
        for(i = 0; i < self->numVertices * attrSize; ++i) {
            dst[i] = data[i];
        }
    }

    /**
     * Free all the resources used by the given model.
     * @param self the model to free the resources of.
     */
    static void Free(Component_Model* self)
    {
        int i;
        for(i = 0; i < self->numAttributes; ++i) {
            free(self->attributes[i]);
        }
        free(self->attributeTable);
        free(self->vboIDs);
    }

    static void Start(Component_Model* self)
    {
        self->mat = Component_GetAs(Material);
        self->transform = Component_GetAs(Transform);
        Draw_OptimizeModel(self);

        self->LoadPLY = LoadPLY;
        self->BufferAttribute = BufferAttribute;
        self->AddAttribute = AddAttribute;
        self->SetAttribute = SetAttribute;
        self->Free = Free;
        self->GetAttributeSize = GetAttributeSize;

        self->numVertices = 0;
        self->numAttributes = 0;
        self->subgroups = NULL;

        if(self->file != NULL) {
            self->LoadPLY(self, self->file);
        }
    }

    static void Update(Component_Model* self) 
    {

    }

    static void Collide(Entity* e)
    {
        puts("model collision");
    }
#endif
#endif
