/******************************************************************************
 * util.h
 * Basic utility functions to handle various tasks.
 *
 * Bryce Wilson
 * created: June 18, 2013
 *****************************************************************************/
#ifndef _UTIL_H
#define _UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/* read the file "filename" into buffer */
int UtilReadFile(const char* filename, char** buffer);

#ifdef __cplusplus
}
#endif
#endif
