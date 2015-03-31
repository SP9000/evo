/*****************************************************************************/
/* A very simple stack structure implemented using the tv_array structure    */
/* When given the choice between a stack or queue, it is best to use a stack */
/* due to the way the two are implemented.									 */
/* July 9, 2014																 */
/* Bryce Wilson																 */
/*****************************************************************************/
#ifndef TV_STACK
#define TV_STACK
#ifdef __cplusplus
extern "C" {
#endif

#include "tv_types.h"

typedef tv_array TV_stack;

#define tv_stack_push(s, p) \
	utarray_push_back(s, p)

#define tv_stack_pop(s, p) \
	utarray_back(s, p); \
	utarray_pop_back(s)

#define tv_stack_empty(s) \
	(s->i == 0)

#define tv_stack_new(s,_icd) \
	utarray_new(s,_icd)

#ifdef __cplusplus
}
#endif
#endif