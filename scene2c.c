/*****************************************************************************/
/* scene2c.c                                                                 */
/* This program converts JSON "scene" files into C code that instantiates the*/
/* entities defined by these files.                                          */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 9, 2013                                                     */
/*****************************************************************************/
#include "util.h"
#include "cJSON.h"

FILE* out_fp;

void Scene_To_C(char* in_file, char* out_file);
void ReadComponent(cJSON* c);

int main(int argc, char** argv)
{
    if(argc != 3) {
        puts("Usage: ./thisprogram input_file output_file");
        exit(1);
    }
    Scene_To_C(argv[1], argv[2]);
    
    return 0;
}

/**
 * Converts the given JSON file to a C file for creating/initializing entities.
 */
void Scene_To_C(char* in_file, char* out_file)
{
    char* text;
    cJSON* root;
    cJSON* entity;
    cJSON* it;

    UtilReadFile(in_file, &text);
    root = cJSON_Parse(text);

    /* check for errors, return NULL if encountered */
    if(!root) {
        fprintf(stderr, "Error: JSON parse error before: [%s]\n", 
                cJSON_GetErrorPtr());
        return;
    }

    /* open output file */
    out_fp = fopen(out_file, "w");
    if(out_fp == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for writing", out_file);
    }

    /* read all entities */
    for(entity = root->child; entity != NULL; entity = entity->next) {
        fprintf(out_fp, "Entity* %s = Entity_New();\n", entity->string);
        /* read all components for this entity */
        for(it = entity->child; it != NULL; it = it->next) {
            /* declare/init a component */
            fprintf(out_fp, "c = Component_New_%s((struct Component_%s){",
                    it->string, it->string);
            ReadComponent(it->child);

            /* close the parentheses from Component_New_XX function call */
            fprintf(out_fp, "});\n"); 

            /* add the component to the entity */
            fprintf(out_fp, "Entity_AddComponent(%s, c);\n", entity->string);
        }
        /* make it a 'lil easier to read. */
        fprintf(out_fp, "\n");
    }
}

/**
 * Recursively read a component of an entity from its JSON representation.
 * TODO: reads all #'s as floats...need int support.
 */
void ReadComponent(cJSON* e)
{
    for(; e != NULL; e = e->next) {
        /* build the destination to the attribute .e.g. location.pos.x */
        if(e->child) {
            fprintf(out_fp, ".%s = {", e->string);
            ReadComponent(e->child);
            fprintf(out_fp, "}");
        }
        switch(e->type) {
            case cJSON_Number:
                fprintf(out_fp, ".%s = %f", e->string, (float)e->valuedouble);
                break;
            case cJSON_String:
                fprintf(out_fp, ".%s = \"%s\"", e->string, e->valuestring);
                break;
        }
        if(e->next != NULL) {
            fprintf(out_fp, ", ");
        }
    }
}

