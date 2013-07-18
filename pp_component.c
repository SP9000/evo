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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "glib.h"

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
    STATE_IS_MODIFIER,
    STATE_SEEK_NAME, 
    STATE_GET_NAME,
    STATE_TYPE_DECLARE,
    STATE_CHECK_IF_FUNCTION,
    STATE_FUNCTION_PROTOTYPE,
    STATE_FUNCTION_PTR_PROTOTYPE,
    STATE_FUNCTION_BODY,
    STATE_FUNCTION_BEGIN,
    STATE_SEEK_PARAM_NAME,
    STATE_SEEK_PARAM_TYPE,
    STATE_IS_PARAM_POINTER,
    STATE_SEEK_END,
    STATE_DONE,
    
    STATE_IS_MORE_PARAMS,
    STATE_GET_PARAM_TYPE
};

/* possible types an attribute may be. */
enum {
    UNDEFINED,
    VARIABLE,
    FUNCTION,
    FUNCTION_PTR,
    FUNCTION_PTR_MAYBE
};

/* ID's for various types */
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

/* the struct for the attributes contained by the COMPONENT definitions */
typedef struct tagAttribute {
    /* if nonzero, this attribute is to be exposed */
    int public;
    /* what...kind of attribute this is (VARIABLE, FUNCTION, etc.) */
    int kind; 
    /* the name of this attributes type e.g. "unsigned const int" */
    char* type;
    /* the name of this attribute */
    char* name;

    /* if kind is FUNCTION or FUNCTION_PTR, the attribute parameters */
    GList* parameters;    
    /* modifiers to the type e.g. short, const, etc. */
    GList* modifiers;
    /* if kind is FUNCTION. */
    char* definition; 
}Attribute;


/**
 * Display a help message
 */
void help();

/**
 * Check if the given attribute name exists by default in components.
 * @param a the attribute to check.
 * @return nonzero if the attribute is a default of components else zero.
 */
int is_default_attribute(char* a);

/**
 * Get the type ID of the given string.
 * @param text the string representation of the type to get the ID of.
 * @return the ID of the given type.
 */
int get_type(char* text);

/**
 * Get one or more attributes from the given file.
 * @param file the file to retrieve the attribute(s) from.
 * @return a list of one or more attributes or NULL if none are left
 */
GSList* get_attribute(FILE* file);

/**
 * Create/allocate a new attribute of the given parameters.
 * @param name the name of the new attribute.
 * @param type the type of the new attribute.
 * @param prototype a list of attributes if this is a function, else NULL.
 * @param body the body of this if it's a function, else NULL.
 * @return the attribute with the given parameters.
 */
Attribute* new_attribute(char* name, char* type, GList* modifiers, GList* parameters, char* body);

/**
 * Get all of the attributes from the given file.
 * @param in_file the file to retrieve the attributes of.
 * @param attributes the list to store the found attributes.
 * @return the offset where the end of the COMPONENT was found...don't ask...
 */
long get_attributes(char* in_file, GSList** attributes);

/**
 * Write a C file using the given file and attributes.
 * @param name the name of the component to write the file for.
 * @param attributes the attributes that the component contains.
 * @param infile the component file.
 * @param outfile the file to write to.
 * @param component_end the location of the end of the COMPONENT definition...
 */
void write_c_file(char* name, GSList* attributes,
    char* infile, char* outfile, long component_end);

/**
 * Write the header file for the component.
 * @param name the name of the component.
 * @attributes the attributes of the component.
 * @param outfile the file to create from the given information.
 */
void write_header(char* name, GSList* attributes, char* outfile);

/**
 * Move the given file pointer to the start of the component.
 * @param fp the file pointer to move.
 * @return the offset from the start of the file to the new position.
 */
long get_component_start(FILE* fp);

/**
 * Retrieve a list of all the components within the given directory.
 * @param path the directory to get all the components from.
 * @param components where to store the name of all the components.
 * @param component_paths where to store the paths of all the components.
 * @param num_components this is a recursive function, call with 0.
 * @return the number of components found.
 */
int get_all_components(char* path, char** components, char** component_paths,
        int num_components);

/**
 * Compare the string at the current position in the given FILE to the given
 * string. If no match is found, the file pointer is returned to its original
 * position.
 * @param fp the file-pointer to compare the string with.
 * @param str the string to compare with the file.
 * @param len the number of characters to compare.
 * @return 0 if a match is found, else nonzero.
 */
int fstrncmp(FILE* fp, char* str, int len);

/**
 * Append the given character to the given string.
 * @param str the string to append the character to.
 * @param c the character to append.
 */
void strappend(char* str, char c);

/**
 * Initialize the string.
 * @str the string to set to empty.
 */
void strclr(char* str);

/**
 * A compare function for comparing an attribute to a string by name.
 */
gint compare_attribute(gconstpointer a, gconstpointer b);

char* out_dir;
char* in_dir;
FILE* header_fp;
FILE* c_fp;

/*****************************************************************************/
int main(int argc, char** argv)
{
    int num_components;
    int i;
    char* pch;
    char* components[512];
    char* component_paths[512];
    char in_buff[512];
    char out_buff[512];
    FILE* include;
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
    num_components = get_all_components(in_dir, components, component_paths, 0);
    for(i = 0; i < num_components; ++i) {
        long component_end;
        GSList* attributes = NULL;

        strcpy(out_buff, out_dir);
        strcat(out_buff, components[i]);
        strcat(out_buff, ".c");

        component_end = get_attributes(component_paths[i], &attributes);
        puts(component_paths[i]);
        write_c_file(components[i], attributes, component_paths[i], out_buff, component_end);

        strcpy(out_buff, out_dir);
        strcat(out_buff, components[i]);
        strcat(out_buff, ".h");
        write_header(components[i], attributes, out_buff);
    }

    /* make file to include all components + define enumerations to ID them */
    strcpy(out_buff, out_dir);
    strcat(out_buff, "all.h");
    strcpy(in_buff, in_dir);
    while((pch = strchr(in_buff, '/'))) {
        *pch = '_';
    }
    include = fopen(out_buff, "wb");
    fprintf(include, "#ifndef COMPONENTS_%s\n", in_buff);
    fprintf(include, "#define COMPONENTS_%s\n", in_buff);
    fprintf(include, "#include \"../component.h\"\n");
    fprintf(include, "#include \"../types.h\"\n\n");
    if(include == NULL) {
        fprintf(stderr, "Error: failed to create include file %s\n", out_buff);
        exit(-1);
    }
    for(i = 0; i < num_components; ++i) {
        fprintf(include, "typedef struct Component_%s Component_%s;\n",
               components[i], components[i]);
    }
    fprintf(include, "\n");
    for(i = 0; i < num_components; ++i) {
        fprintf(include, "#include \"%s.h\"\n", components[i]);
    }
    fprintf(include, "\nenum {\n");
    for(i = 0; i < num_components; ++i) {
        fprintf(include, "    CID_%s,\n", components[i]);
    }
    fprintf(include, "};\n");
    fprintf(include, "#endif\n");
    return 0;
}

int get_all_components(char* path, char** components, char** component_paths, int num_components)
{
    DIR* dir;
    struct dirent* ent;
    char* pch;
    struct stat s;

    if((dir = opendir(path)) == NULL) {
        fprintf(stderr, "Error: couldn't open directory: %s\n", path);
        perror("");
        return -1;
    }

    while((ent = readdir(dir))) {
        /* only process file if it is not hidden and not a directory */
        char sub_path[1024];
        snprintf(sub_path, sizeof(sub_path)-1, "%s%s",
              path, ent->d_name);
        stat(sub_path, &s);
        if(ent->d_name[0] == '.') {
            continue;
        }
        if(!S_ISDIR(s.st_mode)) {
            components[num_components] = (char*)malloc(sizeof(char)*
                    (strlen(ent->d_name)+1));
            component_paths[num_components] = (char*)malloc(sizeof(char)*
                    (strlen(sub_path)+1));
            strcpy(component_paths[num_components], sub_path);
            pch = strchr(ent->d_name, '.');
            *pch = '\0';
            strcpy(components[num_components], ent->d_name);
            ++num_components;
        }
        else {
            strcat(sub_path, "/");
            num_components = get_all_components(sub_path, components, 
                    component_paths, num_components);
        }
    }
    closedir(dir);
    return num_components;
}

long get_attributes(char* in_file, GSList** attributes)
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
        strcpy(super_file, in_dir);
        strcat(super_file, super_component);
        strcat(super_file, ".c");
        get_attributes(super_file, attributes);
    }

    /* read attributes */
    while(!feof(in_fp)) {
        GSList* attrs;
        GSList* prev;
        GSList* it;
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
        attrs = get_attribute(in_fp);
        if(attrs == NULL) {
            fprintf(stderr, "Error: failed to retrieve attribute.\n");
            exit(-6);
        }
        for(it = attrs; it != NULL; it = g_slist_next(it)) {
            Attribute* a = (Attribute*)it->data;
            prev = g_slist_find_custom(*attributes, a->name, compare_attribute);
            if(prev) {
                free(prev->data);
                *attributes = g_slist_remove_link(*attributes, prev);
            }
            *attributes = g_slist_append(*attributes, (gpointer)a);
        }
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
        else {
            state = 0;
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
        fprintf(stderr, "Error: couldn't open file %s for reading\n", 
                in_filename);
        exit(-10);
    }

    outfile = fopen(out_filename, "wb");
    if(outfile == NULL) {
        fprintf(stderr, "Error: couldn't open file %s for writing\n", 
                out_filename);
        exit(-11);
    }

    component_offset = get_component_start(infile);
    fprintf(outfile, 
      "/***************************************************************/\n"
      "/* This is a generated component C file...                     */\n"
      "/* Do whatever you want with it. I really don't care.          */\n"
      "/***************************************************************/\n\n");
    fprintf(outfile, "#define BUILD_COMPONENT_%s\n", name);
    fprintf(outfile, "\n#include \"..\\component.h\"\n");

    /* write everything before the component declaration to the C file */
    fseek(infile, 0, SEEK_SET);
    for(i = 0; i < component_offset - sizeof("COMPONENT"); ++i) {
        fputc(fgetc(infile), outfile);
    }

    /* write attributes */
    fprintf(outfile, "\ntypedef struct Component_%s {\n", name);
    fprintf(outfile, "    void (*Start)(Component_%s*);\n", name);
    fprintf(outfile, "    void (*Update)(Component_%s*);\n", name);
    fprintf(outfile, "    void (*Collide)(Component_%s*, Entity*);\n", name);
    fprintf(outfile, "    struct Entity* entity;\n");
    fprintf(outfile, "    unsigned id;\n\n");

    /* variables - public must go first to be compatible with what the */
    /* outside world believes to be the structure looks like           */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(is_default_attribute(a->name)) {
            continue;
        }
        if(a->public) {
            if(a->kind == VARIABLE) {
                fprintf(outfile, "    %s %s;\n", a->type, a->name);
            }
        }
    }
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(a->public) {
            if(a->kind == FUNCTION_PTR) {
                GList* jt;
                fprintf(outfile, "    %s (%s)(",
                       a->type, a->name);
                for(jt = a->parameters; jt != NULL; jt = g_list_next(jt)) {
                    if(jt != a->parameters) {
                        fprintf(outfile, ", ");
                    }
                    fprintf(outfile, "%s", ((Attribute*)jt->data)->type);
                }
                fprintf(outfile, ");\n");
            }
        }
    }

    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(is_default_attribute(a->name)) {
            continue;
        }
        if(!a->public) {
            if(a->kind == VARIABLE) {
                fprintf(outfile, "    %s %s;\n", a->type, a->name);
            }
        }
    }
    /* unassigned function pointers */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(!a->public) {
            if(a->kind == FUNCTION_PTR) {
                GList* jt;
                fprintf(outfile, "    %s (%s)(",
                       a->type, a->name);
                for(jt = a->parameters; jt != NULL; jt = g_list_next(jt)) {
                    if(jt != a->parameters) {
                        fprintf(outfile, ", ");
                    }
                    fprintf(outfile, "%s", ((Attribute*)jt->data)->type);
                }
                fprintf(outfile, ");\n");
            }
        }
    }
    /* function pointer variables */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(is_default_attribute(a->name)) {
            continue;
        }
        if(a->kind == FUNCTION) {
            GList* jt;
            fprintf(outfile, "    %s (*%s)(Component_%s*", a->type, a->name, name);
            for(jt = a->parameters; jt != NULL; jt = g_list_next(jt)) {
                fprintf(outfile, ", %s", ((Attribute*)jt->data)->type);
            }
            fprintf(outfile, ");\n");
        }
    }
    fprintf(outfile, "}Component_%s;\n\n", name);

    /* function prototypes */
    fprintf(outfile, "static void Start(Component_%s*);\n", name);
    fprintf(outfile, "static void Update(Component_%s*);\n", name);
    fprintf(outfile, "static void Collide(Component_%s*, Entity* other);\n", name);
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(is_default_attribute(a->name)) {
            continue;
        }
        if(a->kind == FUNCTION) {
            GList* jt;
            fprintf(outfile, "static %s %s(Component_%s* self", a->type, a->name, name);
            for(jt = a->parameters; jt != NULL; jt = g_list_next(jt)) {
                fprintf(outfile, ", %s %s", 
                        ((Attribute*)jt->data)->type,
                        ((Attribute*)jt->data)->name);
            }
            fprintf(outfile, ");\n");
        }
    }

    /* Component_X_New definition */
    fprintf(outfile, "Component_%s* Component_%s_New()\n{\n", name, name);
    fprintf(outfile, "    Component_%s* self = "
           "(Component_%s*)malloc(sizeof(Component_%s));\n", name, name, name);
    fprintf(outfile, "    self->Start = Start;\n");
    fprintf(outfile, "    self->Update = Update;\n");
    fprintf(outfile, "    self->Collide = Collide;\n");
    fprintf(outfile, "    self->id = CID_%s;\n", name);
    /* assign function pointers to the function they should be set to */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* attr = (Attribute*)it->data;
        if(is_default_attribute(attr->name)) {
            continue;
        }
        Attribute* a = (Attribute*)it->data;
        if(a->kind == FUNCTION) {
            fprintf(outfile, "    self->%s = %s;\n", a->name, a->name);
        }
    }
    fprintf(outfile, "    return self;\n");
    fprintf(outfile, "}\n");

    /* function bodies */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(a->kind == FUNCTION) {
            GList* jt;
            fprintf(outfile, "%s %s(Component_%s* self", a->type, a->name, name);
            for(jt = a->parameters; jt != NULL; jt = g_list_next(jt)) {
                fprintf(outfile, ", %s %s", ((Attribute*)jt->data)->type,
                        ((Attribute*)jt->data)->name);
            }
            fprintf(outfile, ")\n{\n%s\n}\n", 
                    a->definition);
        }
    }

    /* write everything after the component declaration to the C file */
    fseek(infile, component_end+1, SEEK_SET);
    while(1) {
        char c = fgetc(infile);
        if(feof(infile)) {
            break;
        }
        fputc(c, outfile);
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
    fprintf(outfile, "#ifndef BUILD_COMPONENT_%s\n", name);
    fprintf(outfile, "#include \"all.h\"\n");
    fprintf(outfile, "typedef struct Component_%s {\n", name);
    fprintf(outfile, "    void (*Start)(Component_%s*);\n", name);
    fprintf(outfile, "    void (*Update)(Component_%s*);\n", name);
    fprintf(outfile, "    void (*Collide)(Component_%s*, Entity*);\n", name);
    fprintf(outfile, "    struct Entity* entity;\n");
    fprintf(outfile, "    unsigned id;\n\n");

    /* variables */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(a->public) {
            if(a->kind == VARIABLE) {
                fprintf(outfile, "    %s %s;\n", a->type, a->name);
            }
        }
    }
    /* unassigned function pointers */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(a->public) {
            if(a->kind == FUNCTION_PTR) {
                GList* jt;
                fprintf(outfile, "    %s (%s)(",
                       a->type, a->name);
                for(jt = a->parameters; jt != NULL; jt = g_list_next(jt)) {
                    if(jt != a->parameters) {
                        fprintf(outfile, ", ");
                    }
                    fprintf(outfile, "%s", ((Attribute*)jt->data)->type);
                }
                fprintf(outfile, ");\n");
            }
        }
    }
    /* function pointers to defined functions */
    for(it = attributes; it != NULL; it = g_slist_next(it)) {
        Attribute* a = (Attribute*)it->data;
        if(a->public) {
            if(a->kind == FUNCTION) {
                GList* jt;
                fprintf(outfile, "    %s (*%s)(Component_%s*",
                       a->type, a->name, name);
                for(jt = a->parameters; jt != NULL; jt = g_list_next(jt)) {
                    fprintf(outfile, ", ");
                    fprintf(outfile, "%s", ((Attribute*)jt->data)->type);
                }
                fprintf(outfile, ");\n");
            }
        }
    }

    /* close 'er up */
    fprintf(outfile, "}Component_%s;\n", name);
    fprintf(outfile, "#endif\n");
    fprintf(outfile, "Component_%s* Component_%s_New();\n", name, name);
    fprintf(outfile, "#endif\n");
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

Attribute* new_attribute(char* name, char* type, GList* modifiers, GList* parameters, char* body)
{
    Attribute* a = (Attribute*)malloc(sizeof(Attribute));
    a->name = (char*)malloc((1+strlen(name))*sizeof(char));
    strcpy(a->name, name);

    a->type = (char*)malloc((1+strlen(type))*sizeof(char));
    strcpy(a->type, type);

    a->modifiers = modifiers;
    a->parameters = parameters;

    if(body != NULL && strlen(body) > 0) {
        a->definition = (char*)malloc((1+strlen(body))*sizeof(char));
        strcpy(a->definition, body);
    }
    return a;
}
void a_set_type(Attribute* a, char* type)
{
    a->type = (char*)malloc((1+strlen(type))*sizeof(char));
    strcpy(a->type, type);
}
void a_set_name(Attribute* a, char* name)
{
    a->name = (char*)malloc((1+strlen(name))*sizeof(char));
    strcpy(a->name, name);
}
void a_add_modifier(Attribute* a, char* m)
{
    a->modifiers = g_list_append(a->modifiers, m);
}
void a_add_parameter(Attribute* a, Attribute* param)
{
    a->parameters = g_list_append(a->parameters, (gpointer)param); 
}
void a_set_body(Attribute* a, char* body)
{
    a->definition = (char*)malloc((1+strlen(body))*sizeof(char));
    strcpy(a->definition, body);
}

GSList* get_attribute(FILE* file)
{
    int i;
    Attribute* a;
    char* name_buff;
    char* type_buff;
    char* body_buff;
    char* param_name_buff;
    char* param_type_buff;
    GList* modifiers = NULL;
    GList* parameters = NULL;
    GSList* attrs = NULL;

    int done = 0;
    int public = 0;
    int paren_cnt = 0;
    int brace_cnt = 0;
    int type_found = 0;
    int kind = VARIABLE;

    int state = STATE_IS_MODIFIER;
    int save_state = state;

    long cur;
    char public_buff[7];

    name_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_NAME_SIZE);
    param_name_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_NAME_SIZE);
    type_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_TYPE_NAME_SIZE);
    param_type_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_TYPE_NAME_SIZE);
    body_buff = (char*)malloc(sizeof(char) * ATTRIBUTE_MAX_BODY_SIZE);
    if(name_buff == NULL || type_buff == NULL || body_buff == NULL) {
        fprintf(stderr, "Error: failed to allocate buffers.\n");
        exit(-8);
    }

    strclr(type_buff);
    strclr(name_buff);
    strclr(body_buff);
    strclr(param_type_buff);
    strclr(param_name_buff);
    paren_cnt = 0;
    while(!done) {
        char c = fgetc(file);

        /* update parentheses and brace counts */
        if(c == '(') {
            ++paren_cnt;
        }
        else if(c == ')') {
            --paren_cnt;
        }
        else if(c == '{') {
            ++brace_cnt;
        }
        else if(c == '}') {
            --brace_cnt;
        }

        /* comment? */
        if(state != STATE_BLOCK_COMMENT && 
                state != STATE_LINE_COMMENT &&
                c == '/') {
            char c2 = fgetc(file);

            /* block comment */
            if(c2 == '*') {
                save_state = state;
                state = STATE_BLOCK_COMMENT;
            }
            /* line comment */
            else if(c2 == '/') {
                save_state = state;
                state = STATE_LINE_COMMENT;
            }
            /* it wasn't a comment after all :D */
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
                    char c2 = fgetc(file);
                    if(c2 == '/') {
                        state = save_state;
                    }
                    else {
                        ungetc(c2, file);
                    }
                }
                break;

            /* test if public or not */
            case STATE_IS_MODIFIER:
                /* is this a type modifier? */
                fseek(file, -1, SEEK_CUR);
                if(fstrncmp(file, "public ", 7) == 0) {
                    public = 1;
                    break;
                }
                else if(fstrncmp(file, "const ", 6) == 0) {
                    modifiers = g_list_append(modifiers, "const");
                }
                else if(fstrncmp(file, "static ", 7) == 0) {
                    modifiers = g_list_append(modifiers, "static");
                }
                else if(fstrncmp(file, "register ", 8) == 0) {
                    modifiers = g_list_append(modifiers, "register");
                }
                else if(fstrncmp(file, "volatile ", 9) == 0) {
                    modifiers = g_list_append(modifiers, "volatile");
                }
                else if(fstrncmp(file, "auto ", 5) == 0) {
                    modifiers = g_list_append(modifiers, "auto");
                }

                /* not a modifier, must be the type...unless programmer dum */ 
                else {
                    /* restore c */
                    c = fgetc(file);
                    if(!isspace(c)) {
                        if(!type_found) {
                            ungetc(c, file);
                            state = STATE_TYPE_DECLARE;
                        }
                        else if(c == '*') {
                            strappend(type_buff, '*');
                        }
                        else if(c == '(') {
                            kind = FUNCTION_PTR_MAYBE;
                            state = STATE_GET_NAME;
                        }
                        else {
                            ungetc(c, file);
                            state = STATE_GET_NAME;
                        }
                    }
                }
                break;

            /* get the type of the attribute declaration */
            case STATE_TYPE_DECLARE:
                if(!isspace(c)) {
                    strappend(type_buff,  c);
                }
                else {
                    type_found = 1;
                    state = STATE_IS_MODIFIER;
                }
                break;


            /* get the name of the attribute & check if function or not */
            case STATE_GET_NAME:
                if(paren_cnt > 1) {
                    fprintf(stderr, "Error: too many fucking parentheses (%d) "
                            "in attribute of type %s\n", paren_cnt, type_buff);
                    return NULL;
                }
                if(isspace(c) && (paren_cnt == 0)) {
                    state = STATE_CHECK_IF_FUNCTION;
                }
                /* it was a variable and we've found the end of its name */
                else if(c == ';' && (paren_cnt == 0)) {
                    kind = VARIABLE;
                    state = STATE_DONE;
                }
                /* it's a function or a function pointer, get the prototype */
                else if(c == '(' && (paren_cnt == 1)) {
                    if(name_buff[0] == '*' && kind == FUNCTION_PTR_MAYBE) {
                        kind = FUNCTION_PTR;
                        state = STATE_FUNCTION_PTR_PROTOTYPE;
                    }
                    else {
                        kind = FUNCTION;
                        state = STATE_FUNCTION_PROTOTYPE;
                    }
                }
                /* it was a variable and there's more of the same type to come */
                else if(c == ',' && (paren_cnt == 0)) {
                    a = new_attribute(name_buff, type_buff, modifiers, parameters, body_buff);
                    a->public = public;
                    a->kind = VARIABLE;
                    attrs = g_slist_append(attrs, a);
                    state = STATE_SEEK_NAME;
                }
                else {
                    if(c != '(' && c != ')') {
                        strappend(name_buff, c);
                    }
                }
                break;

            /* check if the attribute is a function/function ptr or variable */
            case STATE_CHECK_IF_FUNCTION:
                if(c == '(') {
                    if(kind == FUNCTION_PTR_MAYBE) {
                        kind = FUNCTION_PTR;
                        state = STATE_FUNCTION_PTR_PROTOTYPE;
                    }
                    else {
                        kind = FUNCTION;
                        state = STATE_FUNCTION_PROTOTYPE;
                    }
                }
                /* it was a variable and we've found the end of its name */
                else if(c == ';' && (paren_cnt == 0)) {
                    state = STATE_DONE;
                }
                /* it was a variable and there's more of the same type to come */
                else if(c == ',' && (paren_cnt == 0)) {
                    a = new_attribute(name_buff, type_buff, modifiers, parameters, body_buff);
                    a->kind = VARIABLE;
                    a->public = public;
                    attrs = g_slist_append(attrs, a);
                    state = STATE_SEEK_NAME;
                }
                else if(!isspace(c)) {
                    fprintf(stderr, "Error: expected ';' at end of declaration"
                            " of attribute %s %s\n", type_buff, name_buff);
                    return NULL;
                }
                break;

            /* function: find all the attributes between the parentheses */
            case STATE_FUNCTION_PROTOTYPE:
                /* all parentheses are closed, start reading body */
                if(paren_cnt == 0) {
                    state = STATE_FUNCTION_BEGIN;
                }
                else if(!isspace(c)) {
                    ungetc(c, file);
                    state = STATE_SEEK_PARAM_TYPE;
                }
                break;

            /* function ptr: find all the attributes between the parentheses */
            case STATE_FUNCTION_PTR_PROTOTYPE:
                /* parentheses closed, look for the semicolon */
                if(paren_cnt == 0) {
                    state = STATE_SEEK_END;
                }
                else if(!isspace(c)) {
                }
                break;

            /* search for the type declaration for the parameter */
            case STATE_SEEK_PARAM_TYPE:
                if(c == ',') {
                    fprintf(stderr, "Error: expected name for parameter before"
                            "','\n");
                    return NULL;
                }
                else if(!isspace(c)) {
                    ungetc(c, file);
                    state = STATE_GET_PARAM_TYPE;
                }
                break;
             
            /* get the type of the next parameter for the function */
            case STATE_GET_PARAM_TYPE:
                if(isspace(c)) {
                    state = STATE_IS_PARAM_POINTER;
                }
                else {
                    strappend(param_type_buff, c);
                }
                break;

            /* we've found the type, but add as many asterisks as we find */
            case STATE_IS_PARAM_POINTER:
                if(c == '*') {
                    strappend(param_type_buff, c);
                }
                else if(!isspace(c)) {
                    ungetc(c, file);
                    /* if it's a function: get the name. ptr: don't care. */
                    if(kind == FUNCTION)  {
                        state = STATE_SEEK_PARAM_NAME;
                    }
                    else {
                        state = STATE_IS_MORE_PARAMS;
                    }
                }
                break;

            /* get the name of the parameter */
            case STATE_SEEK_PARAM_NAME:
                if(paren_cnt == 0 || isspace(c) || c == ',') {
                        /* add parameter */
                        a = new_attribute(param_name_buff, param_type_buff, 
                                modifiers, NULL, NULL);
                        a->kind = FUNCTION;
                        a->public = 0;
                        parameters = g_list_append(parameters, (gpointer)a);

                        /* closing parentheses: begin reading the body */
                        if(c == '(') {
                            state = STATE_FUNCTION_BEGIN;
                            break;
                        }
                        /* more parameters */
                        else if(c == ',') {
                            state = STATE_SEEK_PARAM_TYPE;
                            strclr(param_type_buff);
                            strclr(param_name_buff);
                        }
                        /* maybe more parameters */
                        else {
                            state = STATE_IS_MORE_PARAMS;
                        }
                    }
                    else {
                        strappend(param_name_buff, c);
                    }
                break;

            /* check if there are more parameters to the function */
            case STATE_IS_MORE_PARAMS:
                if(c == ',') {
                    state = STATE_SEEK_PARAM_TYPE;
                }
                /* if function: get body. if ptr: we're done */
                else if(paren_cnt == 0) {
                    if(kind == FUNCTION) {
                        state = STATE_FUNCTION_BEGIN;
                    }
                    else {
                        state = STATE_SEEK_END;
                    }
                }
                break;

            /* find the start of the function body - the opening brace '{' */
            case STATE_FUNCTION_BEGIN:
                if(c == '{') {
                    state = STATE_FUNCTION_BODY;
                }
                break; 

            /* get the body of the function */
            case STATE_FUNCTION_BODY:
                if(brace_cnt == 0) {
                    state = STATE_DONE;
                }
                else {
                    strappend(body_buff, c);
                }
                break;

            /* search for the semicolon */
            case STATE_SEEK_END:
                if(c == ';') {
                    state = STATE_DONE;
                }
                else if(!isspace(c)) {
                    fprintf(stderr, "Error: expected ';' after %s %s %s\n", 
                            type_buff, name_buff, param_type_buff);
                    return NULL;
                }
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
    a = new_attribute(name_buff, type_buff, modifiers, parameters, body_buff);
    a->kind = kind;
    a->public = public;
    attrs = g_slist_append(attrs, a);

    free(name_buff);
    free(type_buff);
    free(body_buff);
    return attrs;
}

int fstrncmp(FILE* fp, char* str, int len)
{
    int i;
    long pos = ftell(fp);
    for(i = 0; i < len; ++i) {
        char c = fgetc(fp);
        if(c != *str++) {
            fseek(fp, pos, SEEK_SET);
            return 1;
        }
        if(feof(fp)) {
            fseek(fp, pos, SEEK_SET);
            return -1;
        }
    }
    return 0;
}

void strappend(char* str, char c)
{
    static char* prev = NULL;
    static int off;

    /* since this is likely to be called repeatedly, only do strlen on new */
    /* strings                                                             */
    if(prev != str) {
        off = strlen(str);
        prev = str;
    }
    str[off++] = c;
    str[off] = '\0';
}

void strclr(char* str)
{
    str[0] = '\0';
}

int is_default_attribute(char* a)
{
    if(strncmp(a, "Start", 6) == 0 || strncmp(a, "Update", 7) == 0 ||
        strncmp(a, "Collide", 8) == 0 || strncmp(a, "entity", 7) == 0 ||
            strncmp(a, "id", 3) == 0) {
        return 1;
    }
    return 0;
}
 
gint compare_attribute(gconstpointer a, gconstpointer b)
{
    Attribute* a1 = (Attribute*)a;
    char* a2 = (char*)b;

    int s = (strlen(a1->name) > strlen(a2)) ? strlen(a1->name) : strlen(a2);
    return strncmp(a1->name, a2, s);
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

