#include "types.h"

UT_icd ut_float_icd = {sizeof(GLfloat), NULL, NULL, NULL};
UT_icd ut_short_icd = {sizeof(GLshort), NULL, NULL, NULL};

int tv_rect_contains(tv_rect* r, tv_vector2* point)
{
    if((point->x >= r->x) &&  (point->x <= (r->x+r->w)) &&
            (point->x >= r->y) && (point->y <= (r->y+r->h))) {
        return 1;
    }
    return 0;
}

int tv_rect_overlaps(tv_rect* r1, tv_rect* r2)
{
    if((r1->x < (r2->x+r2->w)) &&
            ((r1->x+r1->w) > r2->x) &&
            (r1->y < (r2->y+r2->h)) &&
            ((r1->y+r1->h) > r2->y)) {
        return 1;
    }
    return 0;
}

