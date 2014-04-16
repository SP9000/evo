#include "ScrollWidget.h"
void ScrollWidget_AddWidget(ScrollWidget* self, Widget* w, float x, float y)
{
    /* resize the virtual rectangle if necessary */
	if(tv_rect_overlaps(&self->virtual_rect, &w->rect)) {
        if(w->rect.x < self->virtual_rect.x) {
            self->virtual_rect.w += w->rect.x - self->virtual_rect.x;
            self->virtual_rect.x = w->rect.x;
        }
        if(w->rect.y < self->virtual_rect.y) {
            self->virtual_rect.h += w->rect.y - self->virtual_rect.y;
            self->virtual_rect.y = w->rect.y;
        }
        if(w->rect.x+w->rect.w > self->virtual_rect.x+self->virtual_rect.w) {
            self->virtual_rect.w = (w->rect.x+w->rect.w) - (self->virtual_rect.x);
        }
        if(w->rect.x+w->rect.h > self->virtual_rect.x+self->virtual_rect.h) {
            self->virtual_rect.h = (w->rect.x+w->rect.h) - (self->virtual_rect.x);
        }
    }

}
