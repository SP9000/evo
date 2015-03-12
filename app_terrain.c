#include "app_terrain.h"
/*##############################################################################
#################################Climate########################################
##############################################################################*/
COMPONENT_NEW(terrain_Climate, tv_component)
END_COMPONENT_NEW(terrain_Climate)

COMPONENT_START(terrain_Climate)
END_COMPONENT_START

COMPONENT_UPDATE(terrain_Climate)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(terrain_Climate)
END_COMPONENT_DESTROY

void METHOD(terrain_Climate, generate, tvuint numSamples)
{

}
void METHOD(terrain_Climate, updateSample, tvuint numSamples)
{

}
void METHOD(terrain_Climate, updateSamples, tvuint numSamples)
{

}
/*##############################################################################
#################################Soil###########################################
##############################################################################*/
COMPONENT_NEW(terrain_Soil, tv_component)
END_COMPONENT_NEW(terrain_Soil)

COMPONENT_START(terrain_Soil)
END_COMPONENT_START

COMPONENT_UPDATE(terrain_Soil)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(terrain_Soil)
END_COMPONENT_DESTROY

void METHOD(terrain_Soil, generate, tvuint numSamples)
{

}
void METHOD(terrain_Soil, updateSample, tvuint numSamples)
{

}
void METHOD(terrain_Soil, updateSamples, tvuint numSamples)
{

}
/*##############################################################################
#################################Flora##########################################
##############################################################################*/
COMPONENT_NEW(terrain_Flora, tv_component)
END_COMPONENT_NEW(terrain_Flora)

COMPONENT_START(terrain_Flora)
END_COMPONENT_START

COMPONENT_UPDATE(terrain_Flora)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(terrain_Flora)
END_COMPONENT_DESTROY
void METHOD(terrain_Flora, generate, tvuint numSamples)
{

}
void METHOD(terrain_Flora, updateSample, tvuint numSamples)
{

}
void METHOD(terrain_Flora, updateSamples, tvuint numSamples)
{

}
/*##############################################################################
################################Ground##########################################
##############################################################################*/
COMPONENT_NEW(terrain_Ground, tv_component)
END_COMPONENT_NEW(terrain_Ground)

COMPONENT_START(terrain_Ground)
END_COMPONENT_START

COMPONENT_UPDATE(terrain_Ground)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(terrain_Ground)
END_COMPONENT_DESTROY

/** 
 * The vertex used in the generated ground meshes. 
 */ 
typedef struct {
	tv_vector4 color;
}tv_ground_vertex;

//void /* private */ METHDO(terrain_Ground, gen_interpolated
/*******************************************************************************
 * set_soil 
 * Sets the pointer to the soil that is used to represent this ground.  A call
 * to generate is still necessary in order to regenerate the geometry 
 * associated with this soil.
 ******************************************************************************/
void METHOD(terrain_Ground, set_soil, terrain_Ground* new_soil)
{

}
/*******************************************************************************
 * generate_mesh
 * Using samples from the soil, generate a new mesh for the ground.
 ******************************************************************************/
void METHOD(terrain_Ground, generate_mesh, terrain_Ground* new_soil)
{
	SoilSample **ss;
	tvfloat x_pos;	/* the x-coordinate that the running mesh generator is at */
	tvfloat x_step = 1.0f;
	/* the vertex format for the ground */
	tv_model_attribute ground_vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 4, 0},
		{TV_MODEL_PROPERTY_FLOAT, 4, 4*sizeof(tvfloat)}};

	/* if mesh already exists, destroy it before creating a new one. */
	if(self->mesh) {
		DESTROY(self->mesh);
	}
	/* create the new mesh */
	self->mesh = tv_model_new();

	/* place the vertices for the new mesh */
	for(x_pos = 0.0f, ss = (SoilSample**)utarray_front(self->soil->samples); 
		ss != NULL;
		x_pos += x_step, ss = (SoilSample**)utarray_next(self->soil->samples, ss))  {
			SoilSample* s = (*ss);
			switch(s->type) {
			case TERRAIN_SOILTYPE_RICH:
				break;
			case TERRAIN_SOILTYPE_ROCKY:
				break;
			case TERRAIN_SOILTYPE_SAND:
				break;
			default:
				break;
			}
	}
}
/*##############################################################################
#################################Sky############################################
##############################################################################*/
COMPONENT_NEW(terrain_Sky, tv_component)
END_COMPONENT_NEW(terrain_Sky)

COMPONENT_START(terrain_Sky)
END_COMPONENT_START

COMPONENT_UPDATE(terrain_Sky)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(terrain_Sky)
END_COMPONENT_DESTROY

/*##############################################################################
##############################Ground Details####################################
##############################################################################*/
COMPONENT_NEW(terrain_ground_details, tv_component)
	tv_model* mesh;	/**< a mesh generated based on soil properties */
	terrain_Soil* soil;	/**< the soil to base the details off of. */
END_COMPONENT_NEW(terrain_ground_details)

COMPONENT_START(terrain_ground_details)
END_COMPONENT_START

COMPONENT_UPDATE(terrain_ground_details)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(terrain_ground_details)
END_COMPONENT_DESTROY

void METHOD(terrain_ground_details, generate, tvuint lod)
{
	SoilSample **ss;
	tvfloat x_pos;	/* the x-coordinate that the running mesh generator is at */
	tvfloat x_step = 1.0f;
	/* the vertex format for the ground */
	tv_model_attribute ground_vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 4, 0},
		{TV_MODEL_PROPERTY_FLOAT, 4, 4*sizeof(tvfloat)}};

	/* if mesh already exists, destroy it before creating a new one. */
#if 0
	if(self->mesh) {
		DESTROY(self->mesh);
	}
	/* create the new mesh */
	self->mesh = tv_model_new();

	/* place the vertices for the new mesh */
	for(x_pos = 0.0f, ss = (SoilSample**)utarray_front(self->soil->samples); 
		ss != NULL;
		x_pos += x_step, ss = (SoilSample**)utarray_next(self->soil->samples, ss))  {
			SoilSample* s = (*ss);
			switch(s->type) {
			case TERRAIN_SOILTYPE_RICH:
				break;
			case TERRAIN_SOILTYPE_ROCKY:
				break;
			case TERRAIN_SOILTYPE_SAND:
				break;
			default:
				break;
			}
	}
#endif
}
