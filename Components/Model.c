/*****************************************************************************/
/* Model.c                                                                   */
/* The model component.                                                      */
/* Contains definitions for the model and the types it uses as well as       */
/* functions to create/load/manage models. Models are used for nearly all    */
/* objects rendered in the engine including text and other GUI functionaliy. */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: April, 2013                                                      */
/*****************************************************************************/
/* the # of floats each attribute uses */
#define MODEL_ATTRIBUTE_VERTEX_SIZE 3
#define MODEL_ATTRIBUTE_COLOR_SIZE  4
#define MODEL_ATTRIBUTE_NORMAL_SIZE 3
#define MODEL_ATTRIBUTE_TEXCO_SIZE  2

#include "../draw.h"

/* attribute types for the model */
enum {
    MODEL_ATTRIBUTE_NONE,
    MODEL_ATTRIBUTE_VERTEX,
    MODEL_ATTRIBUTE_COLOR,
    MODEL_ATTRIBUTE_NORMAL,
    MODEL_ATTRIBUTE_TEXCO   
};

COMPONENT Model {
    /* the file to load the model properties from */
    public char* file;

    /* the transform component associated with this model. */
    public Component_Transform* transform;
    /* the material associated with this model. */
    public Component_Material* mat;
    /* buffers for each attribute of the model */
    public float** attributes;
    /* a table of MODEL_ATTRIBUTE_* ID's to tell the contents of attributes */
    public int* attributeTable;
    /* the # of unique per-vertex attributes */
    public int numAttributes;

    /* VBO ID's for rendering with OpenGL-2.0+. */
    public GLuint* vboIDs;
    public GLuint vao;

    /* The type of primitive to render as. e.g. GL_LINES, GL_TRIANGLES... */
    public GLuint primitive;
    /* The offset of the next unassigned vertex. */
    public unsigned numVertices;
    /* Submodels of this model. For example: a leg may be a subgroup of body */
    public Component_Model* subgroups;

    /**
     * Get the number of floats the attribute of the given ID uses.
     * @param id the ID of the attribute to get the number of floats used by.
     * @return the number of floats the given attribute uses or negative if unknown
     *  type.
     */
    public int GetAttributeSize(int id) 
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
     * Get the address of the specified attribute's buffer in this model.
     * @param attribute the attribute who's buffer is to be located.
     * @return the location of the buffer for the given attribute or NULL if the
     *  attribute doesn't exist in the given model.
     */
    float* GetAttributeBuffer(int attribute)
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
     * Add an attribute of the given type to this model.
     * This function allocates a buffer of numVertices size for the given attribute.
     * to determine where to place the other attributes.
     * @param val the value to append to the given attribute buffer.
     */
    void AddAttribute(int attribute)
    {
        int i;
        int attrSize; 
        attrSize = self->GetAttributeSize(self, attribute);

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
    void CopyAttribute(float* dst, int dstOffset, float* src, int srcOffset, int type)
    {
        int i;
        int size = GetAttributeSize(self, type);
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
     * Set the specified attribute of this model to the specified value.
     * @param attribute the type of the attribute to set.
     * @param offset the vertex-offset of the attribute to set.
     * @param val the value to set the attribute to.
     */
    void SetAttribute(int attribute, int offset, float* val) 
    {
        int attrSize; 
        int i;
        int exists = -1;

        attrSize = self->GetAttributeSize(self, attribute);

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
            CopyAttribute(self, self->attributes[i],offset, val,0, attribute);
        }
    }

    /**
      * Load a .ply model from file.
      * @param file the file to load the model from.
      */
    void LoadPLY(char* file)
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
                    self->GetAttributeSize(self, self->attributeTable[i]));
        }

        /* read lists of each attribute */
        for(i = 0; i < self->numAttributes; ++i) {
            for(j = 0; j < attributesSize[i]; ++j) {
                int attrSize = self->GetAttributeSize(self, self->attributeTable[i]);
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
            if(faceSizeBuff[curFace] == 2) {
                faceBuff[i] = atoi(strtok(NULL, " \t"));
                faceBuff[i+1] = atoi(strtok(NULL, " \t"));
                self->numVertices += 2;
                i += 2;
            }
            else if(faceSizeBuff[curFace] == 3) {
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
                    self->GetAttributeSize(self, self->attributeTable[i]));
        }

        /* default to triangles */
        self->primitive = GL_TRIANGLES;

        /* expand vertex/normal/color information from indexed face information */
        for(i = 0, j = 0, curFace = 0; curFace < numFaces; ++curFace) {
            /* if triangle, just expand each vertex */
            if(faceSizeBuff[curFace] == 3) {
                for(k = 0; k < self->numAttributes; ++k) {
                    CopyAttribute(self, self->attributes[k],i,
                            tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                    CopyAttribute(self, self->attributes[k],i+1,
                            tmpAttributes[k],faceBuff[j+1], self->attributeTable[k]);
                    CopyAttribute(self, self->attributes[k],i+2,
                            tmpAttributes[k],faceBuff[j+2], self->attributeTable[k]);
                }
                i += 3;
                j += 3;
            }
            /* if quad, do 0,1,2 and 0,2,3 */
            else if(faceSizeBuff[curFace] == 4) {
                for(k = 0; k < self->numAttributes; ++k) {
                    CopyAttribute(self, self->attributes[k],i,
                            tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                    CopyAttribute(self, self->attributes[k],i+1,
                            tmpAttributes[k],faceBuff[j+1], self->attributeTable[k]);
                    CopyAttribute(self, self->attributes[k],i+2,
                            tmpAttributes[k],faceBuff[j+2], self->attributeTable[k]);

                    CopyAttribute(self, self->attributes[k],i+3,
                            tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                    CopyAttribute(self, self->attributes[k],i+4,
                            tmpAttributes[k],faceBuff[j+2], self->attributeTable[k]);
                    CopyAttribute(self, self->attributes[k],i+5,
                            tmpAttributes[k],faceBuff[j+3], self->attributeTable[k]);
                }
                i += 6;
                j += 4;
            }
            /* line, simply expand */
            else if(faceSizeBuff[curFace] == 2) {
                for(k = 0; k < self->numAttributes; ++k) {
                    CopyAttribute(self, self->attributes[k],i,
                            tmpAttributes[k],faceBuff[j], self->attributeTable[k]);
                    CopyAttribute(self, self->attributes[k],i+1,
                            tmpAttributes[k],faceBuff[j+1], self->attributeTable[k]);
                    self->primitive = GL_LINES;
                }
                i += 2;
                j += 2;
            }
            else {
                fprintf(stderr, "Error: unsupported number of vertices "
                       " per face (%d)\n", faceSizeBuff[curFace]);
                break;
            }
        }

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
     * Add the buffer of the given attribute type to this model.
     * @param attribute the type of the attribute to add.
     * @param data the data to buffer (an array of floats ATTRIBUTE_XX_SIZE *
     *   numVertices) floats long.
     */
    void BufferAttribute(int attribute, float* data)
    {
        int i;
        float* dst;
        int attrSize = self->GetAttributeSize(self, attribute);

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
     * Get the axis-aligned bounding box for this model.
     * @return the axis-aligned bounding box for this model.
     */
    public AABB GetAABB()
    {
        int i;
        float min_x, min_y, min_z;
        float max_x, max_y, max_z;
        float* vertices = self->GetAttributeBuffer(self, MODEL_ATTRIBUTE_VERTEX);
        for(i = 0; i < self->numAttributes; i += 3) {
            if(vertices[i] < min_x) {
                min_x = vertices[i];
            }
            if(vertices[i] > max_x) {
                max_x = vertices[i];
            }
            if(vertices[i+1] < min_y) {
                min_y = vertices[i];
            }
            if(vertices[i+1] > max_y) {
                max_y = vertices[i+1];
            }
            if(vertices[i+2] < min_z) {
                min_z = vertices[i];
            }
            if(vertices[i+2] > max_z) {
                max_z = vertices[i+2];
            }
        }
        return (AABB){max_x-min_x, max_y-min_y, max_z-min_z};
    }

    /**
     * Free all the resources used by this model.
     */
    public void Free()
    {
        int i;
        for(i = 0; i < self->numAttributes; ++i) {
            free(self->attributes[i]);
        }
        free(self->attributeTable);
        free(self->vboIDs);
    }

    void Start()
    {
        self->mat = Component_GetAs(Material);
        self->transform = Component_GetAs(Transform);

        self->numVertices = 0;
        self->numAttributes = 0;
        self->subgroups = NULL;

        if(self->file != NULL) {
            self->LoadPLY(self, self->file);
        }
        Draw_OptimizeModel(self);
    }

    void Update() 
    {

    }

    void Collide(Entity* e)
    {
        puts("model collision");
    }
}
