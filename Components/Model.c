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

COMPONENT Model {
    /* the file to load the model properties from */
    public char* file;

    /* the transform component associated with this model. */
    getset Component_Transform* transform;

	/* yo dawg. This is the model struct from the actual engine. */
	TvModel* model;

    /**
     * Add an attribute of the given type to this model.
     * This function allocates a buffer of numVertices size for the given attribute.
     * to determine where to place the other attributes.
     * @param val the value to append to the given attribute buffer.
     */
    public void AddAttribute(int attribute)
    {
		void tv_model_add_attribute(self->model, attribute);
    }

    /* helper function to set an attribute at a given offset */
    void CopyAttribute(float* dst, int dstOffset, float* src, int srcOffset, int type)
    {
    }

    /**
     * Set the specified attribute of this model to the specified value.
     * @param attribute the type of the attribute to set.
     * @param offset the vertex-offset of the attribute to set.
     * @param val the value to set the attribute to.
     */
    public void SetAttribute(int attribute, int offset, float* val) 
    {
    }

    /**
      * Load a .ply model from file.
      * @param file the file to load the model from.
      */
    public void LoadPLY(char* file)
    {
		self->model = tv_model_load_ply(file);
    }

    /**
     * Add the buffer of the given attribute type to this model.
     * @param attribute the type of the attribute to add.
     * @param data the data to buffer (an array of floats ATTRIBUTE_XX_SIZE *
     *   numVertices) floats long.
     */
    public void BufferAttribute(int attribute, float* data)
    {
		tv_model_buffer_attribute(self->model, attribute, data);
	}

    /**
     * Get the axis-aligned bounding box for this model.
     * @return the axis-aligned bounding box for this model.
     */
    public AABB GetAABB()
    {
		return tv_model_get_aabb(self->model);
    }

    /**
     * Generate handles to VBOs for the given model.
     * You must call this function before calling DrawModel.
     * @param m the model to generate VBO ID's for.
     * @param attributes flags for each attribute to generate for.
     */
    public void Optimize() {
		tv_model_optimize(self->model);
    }

    /**
     * Free all the resources used by this model.
     */
    public void Free()
    {
		tv_model_free(self->model);
    }

    void Start()
    {
        self->transform = Component_GetAs(Transform);

		self->model = tv_model_new();
        if(self->file != NULL) {
            self->LoadPLY(self, self->file);
            self->Optimize(self);
        }
    }

    void Update() 
    {

    }

    void Collide(Entity* e)
    {
        puts("model collision");
    }
}

