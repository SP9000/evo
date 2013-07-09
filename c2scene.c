/*****************************************************************************/
/* c2scene.c                                                                 */
/* This program reads component (c) files and produces a JSON skeleton that  */
/* can be used to initialize the properties of the component within an entity*/
/* by using the scene2c program.                                             */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 9, 2013                                                     */
/*****************************************************************************/
#include <stdio.h>
#include "cJSON.h"

FILE* out_fp;

void C_To_Scene(char* in_file, char* out_file);
void ReadComponent(cJSON* c);

int main(int argc, char** argv)
{
    if(argc != 3) {
        puts("Usage: ./thisprogram input_file output_file");
        exit(1);
    }
    C_To_Scene(argv[1], argv[2]);
    
    return 0;
}

/**
 * Converts the given JSON file to a C file for creating/initializing entities.
 */
void C_To_Scene(char* in_file, char* out_file)
{
    FILE* in_fp;
    cJSON* root;
    char line[512];

    /* open input file */
    in_fp = fopen(in_file, "r");
    if(in_fp == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for reading\n", in_file);
        exit(-1);
    }
    /* open output file */
    out_fp = fopen(out_file, "w");
    if(out_fp == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for writing\n", out_file);
        exit(-2);
    }
    while(!feof(in_fp)) {
        char* pch;
        fgets(line, 512, in_fp);
        pch = strtok(line, " \t");

        /* only expose PUBLIC attributes */
        if(strncmp("PUBLIC", pch, 6) == 0) {
            char* type = strtok(NULL, " \t");
            char* name;
            /* get the last word in the line, we'll naively assume that's the
             * attribute name. */
            while(pch = strtok(NULL, " \t")) {
                name = pch;
            }

            /* determine what type we're dealing with and create the 
             * appropriate JSON for that type. */
            if(strncmp("AABB", 4) == 0) {

            }
            else if(strncmp("Vector3", 7) == 0) {

            }
            else {

            }
        }
    }
}

