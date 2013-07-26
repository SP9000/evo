#include "scene.h"

/* TODO: nice "spatial" representation we got here */
GSList* scene_layers[32];

TvHashTable* renderers_table;
TvHashTable* renderer_layer_table;

TvCamera* main_cam;
static GSList* cameras;

int tv_scene_init()
{
    int i;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
        scene_layers[i] = NULL;
    }
    return 0;
	renderers_table = g_hash_table_new(g_int_hash, g_int_equal);
	renderer_layer_table = g_hash_table_new(g_int_hash, g_int_equal);
}

int tv_scene_quit()
{
    int i;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
        g_slist_free(scene_layers[i]);
    }
    return 0;
}
void tv_scene_register_renderer(TvSceneRenderFunc render_func, tvuint layer, tvuint id)
{
	g_hash_table_insert(renderers_table, (gpointer)id, (gpointer)render_func);
	g_hash_table_insert(renderer_layer_table, (gpointer)id, (gpointer)layer);
}

void tv_scene_add(TvComponent* c)
{
	tvuint layer = (tvuint)g_hash_table_lookup(renderer_layer_table, (gpointer)c->id);
    scene_layers[layer] = g_slist_append(scene_layers[layer], (gpointer)c);
}

void tv_scene_update()
{
	GSList* it;
    for(it = cameras; it != NULL; it = g_slist_next(it)) {
        int i;
        TvCamera* cam = (TvCamera*)it->data;
        /* position the camera */
		tv_mat4x4_load_identity(cam->view_mat);
		tv_mat4x4_translate(cam->view_mat, -cam->pos.x, 
                -cam->pos.y, cam->pos.z);
        main_cam = cam;
        for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
            if((1 << i) & cam->render_layers) {
                GSList* jt;
                for(jt = scene_layers[i]; jt != NULL; jt = g_slist_next(jt)) {
					TvComponent* c = (TvComponent*)jt;
					((TvSceneRenderFunc)(g_hash_table_lookup(renderers_table, (gpointer)c->id)))(c);
                }
            }
        }
    }
}

GSList* tv_scene_get_cameras()
{
	return cameras;
}

void tv_scene_add_camera(TvCamera* cam)
{
	cameras = g_slist_append(cameras, (gpointer)cam);
}