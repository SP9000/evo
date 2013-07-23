#include "types.h"

int Rect_ContainsPoint(Rect* r, Vector2* point)
{
    if((point->x >= r->x) &&  (point->x <= (r->x+r->w)) &&
            (point->x >= r->y) && (point->y <= (r->y+r->h))) {
        return 1;
    }
    return 0;
}

int Rect_Overlaps(Rect* r1, Rect* r2)
{
    if((r1->x < (r2->x+r2->w)) &&
            ((r1->x+r1->w) > r2->x) &&
            (r1->y < (r2->y+r2->h)) &&
            ((r1->y+r1->h) > r2->y)) {
        return 1;
    }
    return 0;
}

