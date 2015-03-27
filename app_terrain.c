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
	tv_vector3 pos;
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
	tv_model_vertex ground_vertex_properties = TV_MODEL_VERTEX_FORMAT_PC;

	/* if mesh already exists, destroy it before creating a new one. */
	if(self->mesh) {
		DESTROY(self->mesh);
	}
	/* create the new mesh */
	self->mesh = tv_model_new();
	//TODO:
	//tv_model_vertex_format(self->mesh, 2, TV_MODEL_VERTEX_FORMAT_PC);

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

void METHOD(terrain_Sky, generate, terrain_Climate* climate) 
{
	tvuint i;
	/* generate the sky model based upon the climate information */
}

/*##############################################################################
##############################Ground Details####################################
##############################################################################*/
const tvuint GRASS_SUBMESH = 0;	/* the submesh index of the grass details */
const tvuint ROCKS_SUBMESH = 1;	/* the submesh index of the grass details */
COMPONENT_NEW(terrain_ground_details, tv_component)
	self->mesh = NULL;
	self->grass_blade = NULL;
END_COMPONENT_NEW(terrain_ground_details)

COMPONENT_START(terrain_ground_details)
	/* get the mesh */
	GET(mesh, tv_model);
	/* generate the mesh(es) */
	CALL(terrain_ground_details, generate, 0);
END_COMPONENT_START

COMPONENT_UPDATE(terrain_ground_details)
END_COMPONENT_UPDATE

COMPONENT_DESTROY(terrain_ground_details)
END_COMPONENT_DESTROY

/*******************************************************************************
 * create_grass_mesh
 * Creates a grass details mesh based upon the given ground.
 ******************************************************************************/
 void create_grass_mesh(terrain_ground_details* gd)
{
	tvfloat x, z;
	terrain_Soil* soil = gd->soil;
	const tvfloat GRASS_MESH_WIDTH = 20.0f;	/* the width of the mesh to generate */
	const tvfloat GRASS_MESH_HEIGHT = 1.0f;	/* the height of the mesh to generate */
	const tvfloat GRASS_MESH_X_DENSITY = 1.00f;	/* the ~units between blades of grass in the x-plane. */
	const tvfloat GRASS_MESH_Z_DENSITY = 2.00f;	/* the ~units between blades of grass in the z-plane. */

	tv_model *grass_model;
	SoilSample *s_left, *s_right;

	/* load the grass model */
	//tv_model_load_ply(grass_model, "grass.ply");

	/* TODO: delete - this temporary workaround allows model appending */
	tv_model_load_ply(gd->mesh, "C:\\Users\\Bryce\\Documents\\GitHub\\evo\\StdAssets\\Models\\quad.ply");
	grass_model = gd->grass_blade;

	/* set the L and R soil samples to base the generation off */
	/* TODO:
	s_left = (SoilSample*)utarray_front(soil->samples);
	s_right = (SoilSample*)utarray_next(soil->samples, s_left);
	*/

	/* generate a large mesh of a TON of blades of grass- the mesh must be 
	 * large enough to fill the screen, but it should not be much larger (for
	 * the sake of avoiding unecessary culling work for GL) */
	for(x = 0.0f; x < GRASS_MESH_WIDTH; x += GRASS_MESH_X_DENSITY) {
		for(z = 0.0f; z < GRASS_MESH_HEIGHT; z += GRASS_MESH_Z_DENSITY) {
			/* get the coordinates for the grass' x and z coordinates */
			tv_vector3 offset;
			tvfloat grass_x = x; ///+ (rand() % 100) * GRASS_MESH_X_DENSITY;
			tvfloat grass_z = z; // + (rand() % 100) * GRASS_MESH_Z_DENSITY;
			offset.x = grass_x;
			offset.y = 0.0f;
			offset.z = grass_z;
			/* TODO: adjust the vertex-colors a bit */

			/* apply the offset-translation to the grass model */
			tv_model_apply_translate(grass_model, offset, 0);

			/* add a blade of grass to the model. */
			tv_model_append_model(gd->mesh, grass_model);

			/* revert the translation */
			offset.x = -offset.x;
			offset.z = -offset.z;
			tv_model_apply_translate(grass_model, offset, 0);
			/* move to the next sample */
			/* TODO:
			s_left = s_right;
			s_right = (SoilSample*)utarray_next(soil->samples, s_right);
			*/

			/* if we've reached the last sample, we're screwed. TODO: */
			/* TODO: 
			if(s_right == NULL) {
				break;
			}
			*/
		}
	}
	tv_model_optimize(gd->mesh, TRUE, TRUE);
}

void METHOD(terrain_ground_details, set_grass_mesh, tv_model* mesh)
{
	self->grass_blade = mesh;
}

void METHOD(terrain_ground_details, generate, tvuint lod)
{
	SoilSample **ss;
	tvfloat x_pos;	/* the x-coordinate that the running mesh generator is at */
	tvfloat x_step = 1.0f;
	/* the vertex format for the ground */
	tv_model_attribute ground_vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 4, 0},
		{TV_MODEL_PROPERTY_FLOAT, 4, 4*sizeof(tvfloat)}};

	/* create the grass mesh */
	create_grass_mesh(self);

	/* create the rock mesh */
	//TODO:

	/* if mesh already exists, destroy it before creating a new one. */
#if 0
	if(self->mesh) {
		DESTROY(self->mesh);
	}

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
