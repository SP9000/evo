#include "terrain.h"

typedef struct terrain_vertex {
	tv_vector4 pos;
	tv_vector4 color;
	tv_vector4 flags; /* the plane of this vertex, unused, unused, unused. */
}terrain_vertex;

COMPONENT_NEW(app_terrain, tv_component)
	tv_model_vertex vertex = {2, {TV_MODEL_PROPERTY_FLOAT, TV_MODEL_PROPERTY_FLOAT}, {4,4}};
	tv_vector2 dim = {100,100};
	tv_model_attribute vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 4, 0},
		{TV_MODEL_PROPERTY_FLOAT, 4, 4*sizeof(tvfloat)},
		{TV_MODEL_PROPERTY_FLOAT, 4, 8*sizeof(tvfloat)}};	
	self->model = tv_modelgen_quad(dim, vertex);
END_COMPONENT_NEW(app_terrain)

COMPONENT_START(app_terrain)
END_COMPONENT_START

COMPONENT_UPDATE(app_terrain)

END_COMPONENT_UPDATE
