/*****************************************************************************/
/* material.c                                                                */
/* The material component                                                    */
/* Contains the definition of the material type and functions to load/manage */
/* materials. Platform dependent code (OpenGL) is used in this system.       */
/* Bryce Wilson                                                              */
/* created: June 18, 2013                                                    */
/*****************************************************************************/
#include "../cJSON.h"

COMPONENT Material {
    public char* file;

	/* the engine material, which this component is a wrapper for */
	getset TvMaterial* material

    void Load(char* file) 
    {
		self->material = tv_material_load(file);
    }

    /* Start and Update are part of all components */
    void Start() 
    {
        if(self->file != NULL) {
            self->Load(self, self->file);
        }
    }
    void Update() 
    {

    }

    void Collide(Entity* e)
    {
        puts("material collision");
    }
}


