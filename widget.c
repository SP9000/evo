#include "widget.h"
#include "camera.h"

COMPONENT_NEW(tv_widget, tv_component)
END_COMPONENT_NEW(tv_widget)

COMPONENT_START(tv_widget)
END_COMPONENT_START

COMPONENT_UPDATE(tv_widget)
END_COMPONENT_UPDATE

void tv_widget_add_child(tv_widget *widget, tv_widget *child)
{

}

void tv_widget_set_model(tv_widget *widget, tv_model *model)
{
	widget->model = model;
}

void tv_widget_set_material(tv_widget *widget, tv_material *material)
{
	widget->material = material;
}
