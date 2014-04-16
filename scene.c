#include "scene.h"


/* pairs a renderer ID to a renderer function */
typedef struct ComponentIDtoRenderFuncHash {
	tvuint cid;
	TvSceneRenderFunc render_func;
	TvHashHandle hh;
}ComponentIDtoRenderFuncHash;


/* TODO: nice "spatial" representation we got here */
tv_componentNode* scene_layers[32];
ComponentIDtoRenderFuncHash* renderers_table;

tv_camera* main_cam;
TvCameraNode* cameras;

int tv_scene_init()
{
    int i;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
        scene_layers[i] = NULL;
    }
	renderers_table = NULL;
	return 0;
}

int tv_scene_quit()
{
    int i;
	tv_componentNode* it;
    for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
		for(it = scene_layers[i]; it != NULL; it = it->next) {
			/* TODO: free  */
		}
    }
    return 0;
}

void tv_scene_register_renderer(TvSceneRenderFunc render_func, tvuint layer, tvuint id)
{
	ComponentIDtoRenderFuncHash* entry = (ComponentIDtoRenderFuncHash*)malloc(sizeof(ComponentIDtoRenderFuncHash));
		entry->cid = id;
		entry->render_func = render_func;
	HASH_ADD_INT(renderers_table, cid, entry);
}

void tv_scene_add(tv_component* c, tvuint layer)
{
	tv_componentNode* n = (tv_componentNode*)malloc(sizeof(tv_componentNode));
	LL_PREPEND(scene_layers[layer], n);
}

void tv_scene_update()
{
	TvCameraNode* it;
	for(it = cameras; it != NULL; it = it->next) {
        int i;
        tv_camera* cam = (tv_camera*)it->cam;
        /* position the camera */
		tv_mat4x4_load_identity(cam->view_mat);
		tv_mat4x4_translate(cam->view_mat, -cam->pos.x, 
                -cam->pos.y, cam->pos.z);
        main_cam = cam;
        for(i = 0; i < RENDER_LAYER_COUNT; ++i) {
            if((1 << i) & cam->render_layers) {
				tv_componentNode* jt;
				for(jt = scene_layers[i]; jt != NULL; jt = jt->next) {
					ComponentIDtoRenderFuncHash* f;
					tv_component* c = jt->c;
					HASH_FIND_INT(renderers_table, &c->id, f);
					f->render_func(c);
                }
            }
        }
    }
}

TvCameraNode* tv_scene_get_cameras()
{
	return cameras;
}

void tv_scene_add_camera(tv_camera* cam)
{
	TvCameraNode* n = (TvCameraNode*)malloc(sizeof(TvCameraNode));
	LL_PREPEND(cameras, n);
}