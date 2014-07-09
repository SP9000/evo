/*****************************************************************************/
/* A very simple queue structure implemented using the tv_array structure    */
/* When given the choice between a stack or queue, it is best to use a stack */
/* due to the way the two are implemented.									 */
/* July 9, 2014																 */
/* Bryce Wilson																 */
/*****************************************************************************/
#ifndef TV_QUEUE
#define TV_QUEUE
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef tv_array TV_queue;

#define tv_queue_push(q, p) \
	utarray_insert(q,p,0)

#define tv_queue_pop(q, p) \
	utarray_back(q, p); \
	utarray_pop_back(q) 

#define tv_queue_empty(q) \
	(q->i == 0)

#define tv_queue_new(q,_icd) \
	utarray_new(q,_icd)

#ifdef __cplusplus
}
#endif
#endif