#include "terrain.h"

typedef struct terrain_vertex {
	tv_vector3 pos;
	tv_vector4 color;
}terrain_vertex;

tv_vector3 cube_uv = {0,0,0};
tvfloat cube_tri_strip_vertices[] = {
	-1,-1,1, 1,-1,1, -1,1,1, 1,1,1,
    // Right face
    1,-1,1, 1,-1,-1, 1,1,1, 1,1,-1,
    // Back face
    1,-1,-1, -1,-1,-1, 1,1,-1, -1,1,-1,
    // Left face
    -1,-1,-1, -1,-1,1, -1,1,-1, -1,1,1,
    // Bottom face
    -1,-1,-1, 1,-1,-1, -1,-1,1, 1,-1,1,
    // Top face
    -1,1,1, 1,1,1, -1,1,-1, 1,1,-1
};
tvfloat cube_vertices[] = {
    // Front face
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    // Back face
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    // Top face
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    // Bottom face
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    // Right face
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    // Left face
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f
};
tvuint cube_indices[] = {
    0,  1,  2,      0,  2,  3,    // front
    4,  5,  6,      4,  6,  7,    // back
    8,  9,  10,     8,  10, 11,   // top
    12, 13, 14,     12, 14, 15,   // bottom
    16, 17, 18,     16, 18, 19,   // right
    20, 21, 22,     20, 22, 23    // left
};
tvfloat cube_texcos[] = {
	0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f,
	0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f,
	0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f,
	0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f
};

GLushort cube_tri_strip_indices[] = {
	2, 6, 3, 7,
	4, 5, 0, 1,
	7, 4, 3, 0,
	1, 5, 2, 6,
	4, 7, 5, 6,
	3, 0, 2, 1,
};

static tvuint num_tile_types = 0;

tvuint app_terrain_add_tile_type(app_terrain *terrain, tv_material *material)
{
	app_terrain_tile *tile = &terrain->tile_types[num_tile_types];
	tile->material = material;

	/* copy the main model (and all its vertex properties) and buffer this 
	 * model's indices. */
	tile->model = terrain->model;
	utarray_new(tile->indices, &ut_short_icd);
	tv_model_set_indices(tile->model, tile->indices);
	tv_model_optimize(tile->model);

	/* create/setup a renderer for this type of tile */
	tile->renderer = tv_model_renderer_new();
	tv_model_renderer_set_model(tile->renderer, tile->model);
	tv_renderer_set_material((tv_renderer*)tile->renderer, tile->material);

	++num_tile_types;
}

void app_terrain_set_tile(app_terrain *terrain, tvuint type, tvuint x, tvuint y, tvuint z)
{
	tvuint i;
	tvuint base_index = type * 8;
	tvuint model_index = z*(APP_TERRAIN_SIZE_Y*APP_TERRAIN_SIZE_X) + (x*APP_TERRAIN_SIZE_Y) + y;

	terrain->tiles[z][x][y] = type;
	for(i = 0; i < 6*6; ++i) {
		tv_model_set_index(terrain->model, model_index+i, cube_tri_strip_indices[i]+base_index);
	}
}

COMPONENT_NEW(app_terrain, tv_component)
	tv_model_property vertex_properties[] = {
		{TV_MODEL_PROPERTY_FLOAT, 3, 0},
		{TV_MODEL_PROPERTY_FLOAT, 4, 0}};	
	self->model = tv_model_new();
	tv_model_vertex_format(self->model, 2, vertex_properties);
END_COMPONENT_NEW(app_terrain)

COMPONENT_START(app_terrain)
	tvuint i, j, k, l;
	tvfloat x, y, z;
	terrain_vertex my_vertex;

	self->focus = tv_vector3_zero;
	self->focus.x = 10.0f;

	self->model->primitive = GL_TRIANGLES;
	srand(42);
	/* initialize all tiles to 0. */
	for(i = 0, z = 0; i < APP_TERRAIN_SIZE_Z; ++i, z += APP_TERRAIN_TILE_D) {
		for(j = 0, x = 0; j < APP_TERRAIN_SIZE_X; ++j, x += APP_TERRAIN_TILE_W) {
			for(k = 0, y = 0; k < APP_TERRAIN_SIZE_Y; ++k, y += APP_TERRAIN_TILE_H) {
				self->tiles[i][j][k] = 0;
				for(l = 0; l < 4*6; ++l) {
					my_vertex.pos.x = (cube_vertices[l*3]+1)/2*APP_TERRAIN_TILE_W + x;
					my_vertex.pos.y = (cube_vertices[l*3+1]+1)/2*APP_TERRAIN_TILE_H + y;
					my_vertex.pos.z = (cube_vertices[l*3+2]+1)/2*APP_TERRAIN_TILE_D + z;
					my_vertex.color.w = (rand() % 100) / 100.0f; //j / (float)APP_TERRAIN_SIZE_X;
					my_vertex.color.x = (rand() % 100) / 100.0f; //k / (float)APP_TERRAIN_SIZE_Y;
					my_vertex.color.y = (rand() % 100) / 100.0f; //i / (float)APP_TERRAIN_SIZE_Z;
					tv_model_append_vertex(self->model, &my_vertex);
				}
				for(l = 0; l < 36; ++l) {
					tv_model_append_indices1(self->model, 
						cube_indices[l] +
						(k + 
						j * APP_TERRAIN_SIZE_Y + 
						i* APP_TERRAIN_SIZE_Y * APP_TERRAIN_SIZE_X)*24);
				}
			}
		}
	}
	tv_model_optimize(self->model);
	self->renderer = (tv_model_renderer*)tv_component_get(self_component, tv_model_renderer_id());
	if(self->renderer) {
		tv_model_renderer_set_model(self->renderer, self->model);
	}
	self->material = (tv_material*)tv_component_get((tv_component*)self, tv_material_id());
	if(self->material) {
		self->focus_uniform = tv_material_get_uniform(self->material, "focus");
		glUniform3f(self->focus_uniform, self->focus.x, self->focus.y, self->focus.z);
	}
END_COMPONENT_START

COMPONENT_UPDATE(app_terrain)

END_COMPONENT_UPDATE