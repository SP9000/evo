/*****************************************************************************/
/* pp_component.c                                                            */
/* preprocessor for component files.                                         */
/* This program reads component (.c) files and produces a .h and .c file that*/
/* can be included/compiled to use the component within the engine.          */
/* by using the scene2c program.                                             */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 9, 2013                                                     */
/*****************************************************************************/
#include "glib.h"
#include "util.h"
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

typedef struct tagAttribute {
    int public;
    int is_function;
    char* type;
    char* name;

    char* prototype;    //if function
    char* definition;    //if function
}Attribute;

typedef struct tagFunction {
    int pulic;
    char* prototype;
    char* name;
    char* definition;
}Function;

FILE* header_fp;
FILE* c_fp;

void write_json(cJSON* json, char* type, char* name);
void C_To_Scene(char* in_file, char* out_file);
void ReadComponent(cJSON* c);

/* given pointer at start of a block, returns pointer to end of { } block. */
char* get_block(char* text);
/* turns all whitespace into a single space and deletes comments */
void simplify(char* text);

int get_type(char* text);
Attribute* get_atttribute(char* text);
Attribute* new_attribute(char* name, char* type);

int main(int argc, char** argv)
{
    if(argc != 3) {
        puts("Usage: ./thisprogram input_file output_file");
        exit(1);
    }
    C_To_Scene(argv[1], argv[2]);
    
    return 0;
}

/**************************************
example:
---------- private (*.h) --------------
typedef struct Component_Animation {
    void DoSomething();
}Component_Animation;
Component* Component_New_Animation();

--------- public (*.c) ----------------
typedef struct Component_Animation {
    void DoSomething();
    private members....
}
Component* Component_New_Animation()
{
    ...
}
function prototypes
    ...
functions
    ...
**************************************/

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
    GSList* attributes;
    GSList* functions;

    /* open input file */
    in_fp = fopen(in_file, "r");
    if(in_fp == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for reading\n", in_file);
        exit(-1);
    }
    /* open output files (*.c and *.h) */
    header_fp = fopen(out_header_file, "w");
    if(header_fp == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for writing\n", out_header_file);
        exit(-2);
    }
    c_fp = fopen(out_c_file, "w");
    if(c_fp == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for writing\n", out_c_file);
        exit(-3);
    }

    root = cJSON_CreateObject();
    attributes=cJSON_CreateObject();
    cJSON_AddItemToObject(root, component_name, attributes);

    /* read the input file */
    text = Util_ReadFile(in_file);
    /* simplify the input */
    simplify(text);

    /* find Component:: definition */
    pch = strstr(text, "Component ");
    if(pch == NULL) {
        fprintf(stderr, "Error: no Component definition found\n");
        exit(-4);
    }
    
    /* copy everything before the component definition to the C file */
    for(; text != pch; ++text) {
        fputc(*text, out_c_file);
    }

    /* get the name of the component */
    name = pch = strtok(pch, " ");
    /* move pointer to the meat of the component */
    pch = strtok(pch, "{ ");

    /* read attributes */
    while(*text) {
        char* pname;
        char* ptype;
        Attribute* a;
        a = get_attribute(text);
        if(a != NULL) {
            g_slist_append(attributes, (gpointer)a);
        }
    }

    /* write header */
    fprintf(out_header_file, "#ifndef COMPONENT_%s\n#define COMPONENT_%s", 
            name, name);
    fprintf(out_header_file, "typedef struct Component_%s {\n", name);
    fprintf(out_header_file, "    Component base;\n", name);
    for(i = 0; i < num_public_attributes; ++i) {
        fprintf(out_header_file, "%s %s;\n", 
                public_attributes[i].type, public_attributes[i].name);
    }
    fprintf(out_header_file, "}Component_%s;\n", name);
    fprintf(out_header_file, "Component* Component_%s_New();\n", name);
    fprintf(out_header_file, "#endif\n");

    /* write C file */
    fprintf(out_c_file, "#include \"component.h\"\n");
    fprintf(out_c_file, "typedef struct Component_%s {\n", name);
    fprintf(out_header_file, "    Component base;\n", name);
    for(it = private_attributes; it != NULL; it = g_slist_next(it)) {
        fprintf(out_c_file, "    %s %s;\n",
                ((Attribute*)*it->data)->type,
                ((Attribute*)*it->data)->name);
    }
    fprintf(out_c_file, "}Component_%s;\n", name);
    for(it = functions; it != NULL; it = g_slist_next(it)) {
        fprintf(out_c_file, "static %s %s %s;\n", 
                ((Function*)*it->data)->type,
                ((Function*)*it->data)->name,
                ((Function*)*it->data)->prototype);
    }
    fprintf(out_c_file, "Component* Component_%s_New()\n{\n", name);
    fprintf(out_c_file, "    Component_%s* self = "
           "(Component_%s*)malloc(sizeof(Component_%s));\n", name, name, name);
    fprintf(out_c_file, "    self->base.start = Start;\n");
    fprintf(out_c_file, "    self->base.update = Update;\n");
    fprintf(out_c_file, "    self->base.collide = Collide;\n");
    fprintf(out_c_file, "    self->base.id = CID_%s;\n", name);
    for(it = functions; it != NULL; it = g_slist_next(it)) {
        fprintf(out_c_file, "    self->%s = %s;\n", 
                ((Function*)*it->data)->name,
                ((Function*)*it->data)->name);
    }
    fprintf(out_c_file, "}\n");
    for(it = functions; it != NULL; it = g_slist_next(it)) {
        fprintf(out_c_file, "%s\n", 
                ((Function*)*it->data)->definition);
    }
    /* write JSON skeleton TODO: */

    /* cleanup */
    out = cJSON_Print(root);
    cJSON_Delete(root);
    printf("%s\n", out);
    free(out);
}

void write_json(cJSON* json, char* type, char* name)
{
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

char* get_block(char* text)
{
    char* block = text;
    int brace_cnt = 0;
    for(; *text; ++text) {
        if(*text == '{') {
            ++brace_cnt;
        }
        else if(*text == '}') {
            --brace_cnt;
            if(brace_cnt == 0) {
                /* find next whitespace */
                while(!isspace(*text)) {
                    ++text;
                }
                /* replace whitespace with terminating NULL */
                text = '\0';
                /* point text to 1 character after block */
                ++text;
                return block;
            }
        }
    }
    return NULL;
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
    return 0;
}

Attribute* new_attribute(char* name, char* type)
{
    Attribute* a = (Attribute*)malloc(sizeof(Attribute));
    a->name = (char*)malloc(strlen(name)*sizeof(char));
    strcpy(a->name, name);
    a->type = (char*)malloc(strlen(type)*sizeof(char));
    strcpy(a->type, type);
    return a;
}

Attribute* get_atttribute(char* text)
{
    int i;
    Attribute* a = (Attribute*)malloc(sizeof(Attribute));
    a->name = (char*)malloc(32);

    if(text == ' ') {
        text = strtok(text, " ");
    }
    /* public? */
    if(strncmp(text, "public", 6) == 0) {
        a->public = 1;
        text = strtok(text, " ");
    }
    /* get type */
    if(!get_type(text)) {
        return NULL;
    }
    type = text;

    /* get the name */
    for(i = 0; *text != '(' && *text != ' '; ++text, ++i) {
    }

    /* function? */
    if(text == '(') {
        a->is_function = 1;
    }
    else {
        a->is_function = 0;
    }
    
    /* copy name to avoid clobbering function prototype (if function) */
    strncpy(a->name, text, i);

    /* if function, get prototype and definition */
    if(a->is_function) {
        prototype = text = strtok(text, "{");
        a->definition = get_block(text);
    }
    /* if it wasn't a function, eat semicolon */
    else {
        text = strtok(text, " ;");
    }
    return a;
}

