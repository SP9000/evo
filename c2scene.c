/*****************************************************************************/
/* c2scene.c                                                                 */
/* This program reads component (.c) files and produces a JSON skeleton that */
/* can be used to initialize the properties of the component within an entity*/
/* by using the scene2c program.                                             */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 9, 2013                                                     */
/*****************************************************************************/
#include <stdio.h>
#include "cJSON.h"

#define TYPE_INT        1
#define TYPE_FLOAT      2
#define TYPE_STRING     3
#define TYPE_POINTER    4
#define TYPE_BOOL       5

#define TYPE_VECTOR2    6
#define TYPE_VECTOR3    7
#define TYPE_RECT       8
#define TYPE_AABB       9


FILE* out_fp;

void C_To_Scene(char* in_file, char* out_file);
void ReadComponent(cJSON* c);
int get_type(char* text)
void simplify(char* text);

int main(int argc, char** argv)
{
    if(argc != 3) {
        puts("Usage: ./thisprogram input_file output_file");
        exit(1);
    }
    C_To_Scene(argv[1], argv[2]);
    
    return 0;
}

void C_To_Scene(char* in_file, char* out_file)
{
    char* line;
    char* text;
    FILE* in_fp;
    cJSON* root;
    cJSON* attributes;
    cJSON* tmp;
    char* pch;
    char* out;

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

    root = cJSON_CreateObject();
    attributes=cJSON_CreateObject();
    cJSON_AddItemToObject(root, component_name, attributes);

    /* read the input file */
    text = Util_ReadFile(in_file);
    /* simplify the input */
    simplify(text);

    /* find COMPONENT definition */
    do {
        line = strtok(text, "\n");
    }while(strncmp(line, "COMPONENT", 9) != 0);

    /* find opening parentheses of COMPONENT definition */
    pch = strtok(text, "(");

    /* get the name of the component */
    pch = strtok(pch, " \t\n");
    name = strtok(pch, ",");
    
    while(!feof(in_fp)) {
        fgets(line, 512, in_fp);
        pch = strtok(line, " \t");

        /* only expose PUBLIC attributes */
        if(strncmp("PUBLIC", pch, 6) == 0) {
            char* type = strtok(NULL, " \t");
            char* name;
            /* get the last word in the line; we'll naively assume that's the
             * attribute name. */
            while(pch = strtok(NULL, " \t")) {
                name = pch;
            }

            /* determine what type we're dealing with and create the 
             * appropriate JSON for that type. */
            switch(get_type(type)) {
                case TYPE_INT:
                    cJSON_AddNumberToObject(attributes, name, 0);
                    break;
                case TYPE_FLOAT:
                    cJSON_AddNumberToObject(attributes, name, 0);
                    break;
                case TYPE_STRING:
                    cJSON_AddStringToObject(attributes, name, "");
                    break;
                case TYPE_BOOL:
                    cJSON_AddFalseToObject(attributes, name);
                    break;
                case TYPE_VECTOR2:
                    tmp = cJSON_CreateObject();
                    cJSON_AddNumberToObject(tmp, "x", 0);
                    cJSON_AddNumberToObject(tmp, "y", 0);
                    cJSON_AddItemToObject(attributes, name, tmp);
                    break;
                case TYPE_VECTOR3:
                    tmp = cJSON_CreateObject();
                    cJSON_AddNumberToObject(tmp, "x", 0);
                    cJSON_AddNumberToObject(tmp, "y", 0);
                    cJSON_AddNumberToObject(tmp, "z", 0);
                    cJSON_AddItemToObject(attributes, name, tmp);
                    break;
                case TYPE_RECT:
                    tmp = cJSON_CreateObject();
                    cJSON_AddNumberToObject(tmp, "x", 0);
                    cJSON_AddNumberToObject(tmp, "y", 0);
                    cJSON_AddNumberToObject(tmp, "w", 0);
                    cJSON_AddNumberToObject(tmp, "h", 0);
                    cJSON_AddItemToObject(attributes, name, tmp);
                    break;
                case TYPE_AABB:
                    cJSON_AddNumberToObject(tmp, "w", 0);
                    cJSON_AddNumberToObject(tmp, "h", 0);
                    cJSON_AddNumberToObject(tmp, "d", 0);
                    cJSON_AddItemToObject(attributes, name, tmp);
                    break;
                default:
                    break;
            }
        }
    }
    out = cJSON_Print(root);
    cJSON_Delete(root);
    printf("%s\n", out);
    free(out);
}

void simplify(char* text)
{
    char* new = *text;
    for(; *text; ++text) {
        *new++ = *text;

        /* delete any comments */
        if(*text == '/') {
            /* block comment */
            if(*text+1 == '*') {
                while(*text != '*' && (*(text+1) != '/')) {
                    ++text;
                }
                /* get past comment */
                ++text;
                ++text;
            }
            /* line comment */
            else if(*text+1 == '/') {
                while(*text != '\n') {
                    ++text;
                }
                ++text;
            }
        }
        /* compress whitespace to single space */
        if(isspace(*text)) {
            do {
                ++text; 
            }while(isspace(*text));
            --text;
        }
    }
    *new = '\0';
}

int get_type(char* text)
{
    if(strncmp("int", text, 3) == 0) {
        return TYPE_INT;
    }
    if(strncmp("float", text, 5) == 0) {
        return TYPE_FLOAT;
    }
    if(strncmp("char*", text, 5) == 0) {
        return TYPE_STRING;
    }
    if(strncmp("bool", text, 4) == 0) {
        return TYPE_BOOL;
    }
    if(strncmp("Vector2", text, 6) == 0) {
        return TYPE_VECTOR2;
    }
    if(strncmp("Vector3", text, 6) == 0) {
        return TYPE_VECTOR3;
    }
    if(strncmp("Rect", text, 4) == 0) {
        return TYPE_RECT;
    }
    if(strncmp("AABB", text, 4) == 0) {
        return TYPE_AABB;
    }
}

