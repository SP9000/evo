#include "app_entities.h"
ENTITY(cc, 0,0,0, 1,1,1, 0,0,0)
ADDCOMPONENT(app_camera_controller, c)
tv_vector2 pan_x_lim = {-5.0f, 1000.0f};
tv_vector2 pan_y_lim = {-2.0f, 10.0f};
tv_vector2 pan_z_lim = {0.0f, -30.0f};
app_camera_controller_set_pan_speed(c, 7.25f);
app_camera_controller_set_pan_accel(c, 1.0f);
app_camera_controller_set_pan_limits(c, pan_x_lim, pan_y_lim, pan_z_lim);
ENTITY_END

