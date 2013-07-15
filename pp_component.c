/*****************************************************************************/
/* pp_component.c                                                            */
/* preprocessor for component files.                                         */
/* This program reads component (.c) files and produces a .h and .c file that*/
/* can be included/compiled to use the component within the engine.          */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 9, 2013                                                     */
/*****************************************************************************/
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "glib.h"
#include "util.h"
#include "cJSON.h"

#define ATTRIBUTE_MAX_NAME_SIZE 64
#define ATTRIBUTE_MAX_TYPE_NAME_SIZE 64
#define ATTRIBUTE_MAX_PROTOTYPE_SIZE 2048
/* who makes a MB function after comment removal!? */
#define ATTRIBUTE_MAX_BODY_SIZE 1048576

/* states for pre-component definition FSM */
enum {
    STATE_SEEK_COMPONENT_NAME,
    STATE_GET_COMPONENT_NAME,
    STATE_SEEK_COMPONENT_BODY,
    STATE_SEEK_SUPER,
    STATE_GET_SUPER,
    STATE_SEEK_COMPONENT_BRACE,
    STATE_COMPONENT_FOUND
};

/* states for parsing attributes FSM */
enum {
    STATE_BLOCK_COMMENT,
    STATE_LINE_COMMENT,
    STATE_IS_PUBLIC,
    STATE_SEEK_NAME, 
    STATE_TYPE_DECLARE,
    STATE_CHECK_IF_FUNCTION,
    STATE_FUNCTION_PROTOTYPE,
    STATE_FUNCTION_BODY,
    STATE_FUNCTION_BEGIN,
    STATE_DONE,
    STATE_SEEK_PARAM_NAME,
    STATE_SEEK_PARAM_TYPE,
    STATE_IS_MORE_PARAMS,
    STATE_GET_PARAM_TYPE
};

enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_POINTER,
    TYPE_BOOL,

    TYPE_VECTOR2,
    TYPE_VECTOR3,
    TYPE_RECT,
    TYPE_AABB  
};

typedef struct tagAttribute {
    int public;
    int is_function;
    char* type;
    char* name;

    GList* parameters;    //if function
    char* definition;     //if function
}Attribute;


FILE* header_fp;
FILE* c_fp;

void help();

/* given pointer at start of a block, returns pointer to end of { } block. */
char* get_block(char* text);
/* turns all whitespace into a single space and deletes comments */
void simplify(char* text);

int get_type(char* text);
Attribute* get_attribute(FILE* file);
Attribute* new_attribute(char* name, char* type, GList* prototype, char* body);
long get_attributes(char* in_file, GSList* attributes);

void write_attributes(char* name, GSList* attributes, FILE* header_fp, FILE* c_fp);
void write_c_file(char* name, GSList* attributes,
    char* infile, char* outfile, long component_end);
void write_header(char* name, GSList* attributes, char* outfile);
long get_component_start(FILE* fp);


char* out_dir;
char* in_dir;

int main(int argc, char** argv)
{
    DIR* component_dir;
    int num_components;
    struct dirent* ent;
    int i;
    char* components[512];
    char in_buff[512];
    char out_buff[512];
    in_dir = NULL;
    out_dir = NULL;

    for(i = 1; i < argc; ++i) {
        if(strncmp(argv[i], "help", 4) == 0) {
            help();
        }
        else if(strncmp(argv[i], "-I", 2) == 0) {
            in_dir = argv[++i];
        }
        else if(strncmp(argv[i], "-O", 2) == 0) {
            out_dir = argv[++i];
        }
        else {
            fprintf(stderr, "Unrecognized argument %s\n", argv[i]);
            help();
        }
    }
    if(in_dir == NULL) {
        fprintf(stderr, "Error: no component directory specified.\n");
        fprintf(stderr, "Use argument -help for help message.\n");
        exit(-1);
    }
    if(out_dir == NULL) {
        fprintf(stderr, "Error: no output directory specified.\n");
        fprintf(stderr, "Use argument -help for help message.\n");
        exit(-1);
    }
    if((component_dir = opendir(in_dir)) != NULL) {
        for(i = 0, num_components = 0; (ent = readdir(component_dir)); ++i) {
            struct stat s;
            strcpy(in_buff, in_dir);
            strcat(in_buff, ent->d_name);
            stat(in_buff, &s);
            /* only process file if it is not hidden and not a directory */
            if(!S_ISDIR(s.st_mode) && ent->d_name[0] != '.') {
                components[num_components] = (char*)malloc(sizeof(char)*(strlen(ent->d_name)+1));
                strcpy(components[num_components], ent->d_name);
                ++num_components;
            }
        }
        closedir(component_dir);
    }
    else {
        fprintf(stderr, "Error: couldn't open directory: %s\n", in_dir);
    }
    for(i = 0; i < num_components; ++i) {
        char* pch;
        long component_end;
        GSList* attributes = NULL;

        puts(components[i]);
        strcpy(in_buff, in_dir);
        strcat(in_buff, components[i]);
        strcpy(out_buff, out_dir);
        strcat(out_buff, components[i]);

        component_end = get_attributes(in_buff, attributes);
        write_c_file(components[i], attributes, in_buff, out_buff, component_end);

        /* TODO: will break with no extension...assumes 1 char extension */
        for(pch = &out_buff[strlen(out_buff)]; *pch != '.'; --pch);
        *(pch+1) = 'h';
        write_header(components[i], attributes, out_buff);
    }
    return 0;
}

long get_attributes(char* in_file, GSList* attributes)
{
    FILE* in_fp;

    int done;
    int i;
    int state;
    char name[64];
    char super_component[64];
    char super_file[256];


    /* open input file */
    in_fp = fopen(in_file, "rb");
    if(in_fp == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for reading\n", in_file);
        exit(-1);
    }
    /* move the file to the component start */
    get_component_start(in_fp);
    
    /* find the super component (if any) and the component name */
    super_component[0] = '\0';
    for(done = 0, state = STATE_SEEK_COMPONENT_NAME; !done; ) {
        char c = fgetc(in_fp);
        switch(state) {
            case STATE_SEEK_COMPONENT_NAME:
                if(!isspace(c)) {
                    state = STATE_GET_COMPONENT_NAME;
                    name[0] = c;
                    i = 1;
                }
                break;

            case STATE_GET_COMPONENT_NAME:
                if(c== '{') { 
                    name[i] = '\0';
                    state = STATE_COMPONENT_FOUND;
                }
                if(isspace(c)) {
                    name[i] = '\0';
                    state = STATE_SEEK_COMPONENT_BODY;
                }
                else {
                    name[i] = c;
                    ++i;
                }
                break;

            case STATE_SEEK_COMPONENT_BODY:
                if(c == ':') {
                    i = 0;
                    state = STATE_SEEK_SUPER;
                }
                else if(c == '{') {
                    state = STATE_COMPONENT_FOUND;
                }
                else if(!isspace(c)) {
                    fprintf(stderr, "Error: unexpected character(s) found after "
                            "COMPONENT %s\n", name);
                    exit(-9);
                }
                break;

            case STATE_SEEK_SUPER:
                if(c == '{') {
                    fprintf(stderr, "Error: expected super-component after :");
                    exit(-10);
                }
                if(!isspace(c)) {
                    state = STATE_GET_SUPER;
                    super_component[0] = c;
                    i = 1;
                }
                break;

            case STATE_GET_SUPER:
                if(c == '{') {
                    super_component[i] = '\0';
                    state = STATE_COMPONENT_FOUND;
                }
                else if(isspace(c)) {
                    super_component[i] = '\0';
                    state = STATE_SEEK_COMPONENT_BRACE;
                }
                else {
                    super_component[i] = c;
                    ++i;
                }
                break;

            case STATE_SEEK_COMPONENT_BRACE:
                if(c == '{') {
                    state = STATE_COMPONENT_FOUND;
                }
                else if(!isspace(c)) {
                    fprintf(stderr, "Error: unexpected character(s) found after "
                            "COMPONENT %s : %s\n", name, super_component);
                    exit(-9);
                }
                break;

           case STATE_COMPONENT_FOUND:
                done = 1;
                break;
        }
    }

    /* is there a super component? If so, get its attributes recursively */
    if(super_component[0]) {
        strcpy(super_file, out_dir);
        strcat(super_file, super_component);
        get_attributes(super_file, attributes);
    }

    /* read attributes */
    while(!feof(in_fp)) {
        Attribute* a;
        char c;
        /* eat spaces */
        do {
            c = fgetc(in_fp);
        }while(isspace(c));
        ungetc(c, in_fp);

        /* if floating '}' found, assume we're at the end */
        if(c == '}') {
            break;
        }
        a = get_attribute(in_fp);
        if(a == NULL) {
            fprintf(stderr, "Error: failed to retrieve attribute.\n");
            exit(-6);
        }
        attributes = g_slist_append(attributes, (gpointer)a);
    }
    return ftell(in_fp);
}

long get_component_start(FILE* fp)
{
    /* find COMPONENT definition  TODO: comments not supported here */
    int state;
    char buff[10];
    for(state = 0; state != 9; ) {
        buff[state] = fgetc(fp);
        if(buff[state] == "COMPONENT"[state]) {
            ++state;
        }
        if(feof(fp)) {
            fprintf(stderr, "Error: no COMPONENT definition found\n");
            exit(-4);
        }
    }
    return ftell(fp);
}

void write_c_file(char* name, GSList* attributes,
    char* in_filename, char* out_filename, long component_end)
{
    int i;
    GSList* it;
    FILE* infile;
    FILE* outfile;
    long component_offset;

    infile = fopen(in_filename, "rb");
    if(infile == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for reading\n", in_filename);
        exit(-10);
    }

    outfile = fopen(out_filename, "wb");
    if(outfile == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for writing\n", out_filename);
        exit(-11);
    }

    component_offset = get_component_start(infile);
    fprintf(outfile, 
            "/***************************************************************/\n"
            "/* This is a generated component C file...                     */\n"
            "/* Do whatever you want with it. I really don't care.          */\n"
            "/***************************************************************/\n\n");
    fprintf(outfile, "\n#include \"..\\component.h\"\n");

    /* write everything before the component declaration to the C file */
    fseek(infile, 0, SEEK_SET);
    for(i = 0; i < component_offset; ++i) {
        fputc(fgetc(infile), outfile);
    }

    /* write attributes */
    fprintf(outfile, "typedef struct Component_%s {\n", name);
    fprintf(outfile, "    Component base;\n");

    /* variables - public must go first to be compatible with what the */
    /* outside world believes to be the structure looks like           */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        if(((Attribute*)(it->data))->public) {
            if(!((Attribute*)it->data)->is_function) {
                fprintf(outfile, "    %s %s;\n", 
                        ((Attribute*)it->data)->type,
                        ((Attribute*)it->data)->name);
            }
        }
    }
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        if(!((Attribute*)(it->data))->public) {
            if(!((Attribute*)it->data)->is_function) {
                fprintf(outfile, "    %s %s;\n", 
                        ((Attribute*)it->data)->type,
                        ((Attribute*)it->data)->name);
            }
        }
    }
    /* function pointer variables */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        if(((Attribute*)it->data)->is_function) {
            GList* jt;
            fprintf(outfile, "    %s (*%s)(Component_%s*",
                    ((Attribute*)it->data)->type,
                    ((Attribute*)it->data)->name,
                    name);
            for(jt = ((Attribute*)it->data)->parameters; jt != NULL; jt = g_list_next(jt)) {
                fprintf(outfile, ", ");
                fprintf(outfile, "%s", ((Attribute*)jt->data)->type);
            }
            fprintf(outfile, ");\n");
        }
    }

    /* function prototypes */
    fprintf(outfile, "}Component_%s;\n", name);
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        if(((Attribute*)it->data)->is_function) {
            GList* jt;
            fprintf(outfile, "static %s %s(Component_%s* self", 
                    ((Attribute*)it->data)->type,
                    ((Attribute*)it->data)->name,
                    name);
            for(jt = ((Attribute*)it->data)->parameters; jt != NULL; jt = g_list_next(jt)) {
                fprintf(outfile, ", %s %s", 
                        ((Attribute*)jt->data)->type,
                        ((Attribute*)jt->data)->name);
            }
            fprintf(outfile, ");\n");
        }
    }

    /* Component_X_New definition */
    fprintf(outfile, "Component* Component_%s_New()\n{\n", name);
    fprintf(outfile, "    Component_%s* self = "
           "(Component_%s*)malloc(sizeof(Component_%s));\n", name, name, name);
    fprintf(outfile, "    self->base.start = Start;\n");
    fprintf(outfile, "    self->base.update = Update;\n");
    fprintf(outfile, "    self->base.collide = Collide;\n");
    fprintf(outfile, "    self->base.id = CID_%s;\n", name);
    /* assign function pointers to the function they should be set to */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        if(((Attribute*)it->data)->is_function) {
            fprintf(outfile, "    self->%s = %s;\n", 
                    (((Attribute*)it->data))->name,
                    (((Attribute*)it->data))->name);
        }
    }
    fprintf(outfile, "}\n");

    /* function bodies */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        if(((Attribute*)it->data)->is_function) {
            GList* jt;
            fprintf(outfile, "%s %s(Component_%s* self",
                    ((Attribute*)it->data)->type,
                    ((Attribute*)it->data)->name,
                    name);
            for(jt = ((Attribute*)it->data)->parameters; jt != NULL; jt = g_list_next(jt)) {
                fprintf(outfile, ", %s %s", ((Attribute*)jt->data)->type,
                        ((Attribute*)jt->data)->name);
            }
            fprintf(outfile, ")\n{\n%s\n}\n", 
                    (((Attribute*)it->data))->definition);
        }
    }

    /* write everything after the component declaration to the C file */
    fseek(infile, component_end, SEEK_SET);
    while(!feof(infile)) {
        fputc(fgetc(infile), outfile);
    }
}

void write_header(char* name, GSList* attributes, char* out_filename)
{
    FILE* outfile;
    GSList* it;

    outfile = fopen(out_filename, "wb");
    if(outfile == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for writing\n", out_filename);
        exit(-11);
    }

    /* write some friendly messages */
    fprintf(outfile, 
            "/***************************************************************/\n"
            "/* This is a generated component header file...                */\n"
            "/* Do whatever you want with it. I really don't care.          */\n"
            "/***************************************************************/\n\n");

    /* write attributes */
    fprintf(outfile, "#ifndef COMPONENT_%s\n#define COMPONENT_%s\n", 
            name, name);
    fprintf(outfile, "typedef struct Component_%s {\n", name);
    fprintf(outfile, "    Component base;\n");

    /* variables */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        if(((Attribute*)(it->data))->public) {
            if(!((Attribute*)it->data)->is_function) {
                fprintf(outfile, "    %s %s;\n", 
                        ((Attribute*)it->data)->type,
                        ((Attribute*)it->data)->name);
            }
        }
    }
    /* function pointer variables */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        if(((Attribute*)(it->data))->public) {
            if(((Attribute*)it->data)->is_function) {
                GList* jt;
                fprintf(outfile, "    %s (*%s)(Component_%s*",
                        ((Attribute*)it->data)->type,
                        ((Attribute*)it->data)->name,
                        name);
                for(jt = ((Attribute*)it->data)->parameters; jt != NULL; jt = g_list_next(jt)) {
                    fprintf(outfile, ", ");
                    fprintf(outfile, "%s", ((Attribute*)jt->data)->type);
                }
                fprintf(outfile, ");\n");
            }
        }
    }

    /* close 'er up */
    fprintf(outfile, "}Component_%s;\n", name);
    fprintf(outfile, "Component* Component_%s_New();\n", name);
    fprintf(outfile, "#endif\n");
}

void simplify(char* text)
{
    char* new = text;
    for(; *text; ++text) {
        /* delete any comments */
        if(*text == '/') {
            /* block comment */
            if(*(text+1) == '*') {
                while((*text != '*') || (*(text+1) != '/')) {
                    ++text;
                }
                /* get past comment */
                ++text;
            }
            /* line comment */
            else if(*(text+1) == '/') {
                while(*text != '\n') {
                    ++text;
                }
                ++text;
            }
        }
        else {
            *new++ = *text;
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

Attribute* new_attribute(char* name, char* type, GList* parameters, char* body)
{
    Attribute* a = (Attribute*)malloc(sizeof(Attribute));
    a->name = (char*)malloc((1+strlen(name))*sizeof(char));
    strcpy(a->name, name);

    a->type = (char*)malloc((1+strlen(type))*sizeof(char));
    strcpy(a->type, type);

    a->parameters = parameters;

    if(body != NULL && strlen(body) > 0) {
        a->definition = (char*)malloc((1+strlen(body))*sizeof(char));
        strcpy(a->definition, body);
    }

    return a;
}

Attribute* get_attribute(FILE* file)
{
    int i;
    Attribute* a;
    char* name_buff;
    char* type_buff;
    char* body_buff;
    char* param_name_buff;
    char* param_type_buff;
    GList* parameters;

    int done = 0;
    int public = 0;
    int is_function = 0;
    int paren_cnt = 0;

    int state = STATE_IS_PUBLIC;
    int save_state = state;

    name_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_NAME_SIZE);
    param_name_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_NAME_SIZE);
    type_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_TYPE_NAME_SIZE);
    param_type_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_TYPE_NAME_SIZE);
    body_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_BODY_SIZE);
    if(name_buff == NULL || type_buff == NULL || body_buff == NULL) {
        fprintf(stderr, "Error: failed to allocate buffers.\n");
        exit(-8);
    }

    body_buff[0] = '\0';
    parameters = NULL;

    while(!done) {
        char c = fgetc(file);

        /* comment? */
        if(state != STATE_BLOCK_COMMENT && 
                state != STATE_LINE_COMMENT &&
                c == '/') {
            char c2 = fgetc(file);
            if(c2 == '*') {
                save_state = state;
                state = STATE_BLOCK_COMMENT;
            }
            else if(c2 == '/') {
                save_state = state;
                state = STATE_LINE_COMMENT;
            }
            else {
                ungetc(c2, file);
            }
        }

        /* I hate state machines */
        switch(state) {
            /* done with a line comment? */
            case STATE_LINE_COMMENT:
                if(c == '\n') {
                    state = save_state;
                }
                break;

            /* done with a block comment? */
            case STATE_BLOCK_COMMENT:
                if(c == '*') {
                    if(fgetc(file) == '/') {
                        state = save_state;
                    }
                }
                break;

            /* test if public or not */
            case STATE_IS_PUBLIC:
                /* it's almost a thing of beauty isn't it? */
                if(c == 'p')  {
                    if(fgetc(file) == 'u') 
                        if(fgetc(file) == 'b') 
                            if(fgetc(file) == 'l') 
                                if(fgetc(file) == 'i') 
                                    if(fgetc(file) == 'c') 
                                        if(isspace(fgetc(file))) {
                                            public = 1;
                                            break;
                                        }
                                        else ungetc(' ', file);
                                    else ungetc('c', file);
                                else ungetc('i', file);
                            else ungetc('l', file);
                        else ungetc('b', file);
                    else ungetc('u', file);
                }
                if(!isspace(c)) {
                    type_buff[0] = c;
                    state = STATE_TYPE_DECLARE;
                    i = 1;
                }
                break;

            /* get the type of the attribute declaration */
            case STATE_TYPE_DECLARE:
                if(!isspace(c)) {
                    type_buff[i] = c;
                    ++i;
                }
                else {
                    type_buff[i] = '\0';
                    state = STATE_SEEK_NAME;
                    i = 0;
                }
                break;

            /* get the name of the attribute & check if function or not */
            case STATE_SEEK_NAME:
                if(isspace(c)) {
                    name_buff[i] = '\0';
                    state = STATE_CHECK_IF_FUNCTION;
                    i = 0;
                }
                else if(c == ';') {
                    name_buff[i] = '\0';
                    state = STATE_DONE;
                    i = 0;
                }
                else if(c == '(') {
                    name_buff[i] = '\0';
                    paren_cnt = 1;
                    state = STATE_FUNCTION_PROTOTYPE;
                    i = 0;
                }
                else {
                    name_buff[i] = c;
                    ++i;
                }
                break;

            /* check if the attribute is a function or variable */
            case STATE_CHECK_IF_FUNCTION:
                if(c == '(') {
                    paren_cnt = 1;
                    state = STATE_FUNCTION_PROTOTYPE;
                }
                if(c == ';') {
                    state = STATE_DONE;
                }
                else if(!isspace(c)) {
                    fprintf(stderr, "Error: expected ';' at end of declaration"
                            " of attribute %s\n", name_buff);
                    return NULL;
                }
                break;

            /* get everything in the parentheses for the function prototype */
            case STATE_FUNCTION_PROTOTYPE:
                is_function = 1;
                if(c == '(') {
                    ++paren_cnt;
                }
                else if(c == ')') {
                    --paren_cnt;
                    if(paren_cnt == 0) {
                        state = STATE_FUNCTION_BEGIN;
                    }
                }
                else if(!isspace(c)) {
                    param_type_buff[0] = c;
                    state = STATE_SEEK_PARAM_TYPE;
                    i = 1;
                }
                break;

            /* search for the type declaration for the parameter */
            case STATE_SEEK_PARAM_TYPE:
                if(c == ',') {
                    fprintf(stderr, "Error: expected name for parameter before"
                            "','\n");
                    return NULL;
                }
                if(!isspace(c)) {
                    state = STATE_GET_PARAM_TYPE;
                    param_type_buff[i] = c;
                    ++i;
                }
                break;
             
            /* get the type of the next parameter for the function */
            case STATE_GET_PARAM_TYPE:
                if(isspace(c)) {
                    state = STATE_SEEK_PARAM_NAME;
                    param_type_buff[i] = '\0';
                    i = 0;
                }
                else {
                    param_type_buff[i] = c;
                    ++i;
                }
                break;

            /* get the name of the parameter */
            case STATE_SEEK_PARAM_NAME:
                if(c == ')') {
                    --paren_cnt;
                    if(paren_cnt == 0) {
                        /* add last parameter and begin reading the body */
                        param_name_buff[i] = '\0';
                        a = new_attribute(param_name_buff, param_type_buff, NULL, NULL);
                        a->is_function = 0;
                        a->public = 0;
                        parameters = g_list_append(parameters, (gpointer)a);
                        state = STATE_FUNCTION_BEGIN;
                        i = 0;
                        break;
                    }
                }
                else if(isspace(c) || c == ',') {
                    state = STATE_FUNCTION_PROTOTYPE;
                    param_name_buff[i] = '\0';
                    a = new_attribute(param_name_buff, param_type_buff, NULL, NULL);
                    a->is_function = 0;
                    a->public = 0;
                    parameters = g_list_append(parameters, (gpointer)a);
                    /* more parameters */
                    if(c == ',') {
                        state = STATE_SEEK_PARAM_TYPE;
                        i = 0;
                    }
                    /* possibly no more parameters */
                    else {
                        state = STATE_IS_MORE_PARAMS;
                        i = 0;
                    }
                }
                else {
                    param_name_buff[i] = c;
                    ++i;
                }
                break;

            /* check if there are more parameters to the function */
            case STATE_IS_MORE_PARAMS:
                if(c == ',') {
                    state = STATE_SEEK_PARAM_TYPE;
                }
                else if(c == ')') {
                    --paren_cnt;
                    if(paren_cnt == 0) {
                        state = STATE_FUNCTION_BEGIN;
                    }
                }

            /* find the start of the function body - the opening brace '{' */
            case STATE_FUNCTION_BEGIN:
                if(c == '{') {
                    state = STATE_FUNCTION_BODY;
                    paren_cnt = 1;
                }
                break; 

            /* get the body of the function */
            case STATE_FUNCTION_BODY:
                if(c == '{') {
                    ++paren_cnt;
                }
                else if(c == '}') {
                    --paren_cnt;
                    if(paren_cnt == 0) {
                        body_buff[i] = '\0';
                        state = STATE_DONE;
                        break;
                    }
                }
                body_buff[i] = c;
                ++i;
                break;

            /* we're done */
            case STATE_DONE:
                done = 1;
                break;

            /* shouldn't reach */
            default:
                fprintf(stderr, "Uhhhh, something went wrong.\n");
                return NULL;
        }
    }
    a = new_attribute(name_buff, type_buff, parameters, body_buff);

    a->is_function = is_function;
    a->public = public;

    free(name_buff);
    free(type_buff);
    free(body_buff);
    return a;
}
 
void help()
{
    puts("command line arguments:");
    puts("-help: display this help message");
    puts("-I <directory>: specify the directory of the component "
            "files to process");
    puts("-O <directory>: specify the directory where the processed "
            "component files should be placed");
}

