#ifndef _MODELGEN_H
#define _MODELGEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "draw.h"
#include "model.h"

/**
 * Create a model of the specified text.
 * @param text the text to generate a model of.
 */
Model* GenText(char *text);


#ifdef __cplusplus
}
#endif

#endif
