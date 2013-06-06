
#ifndef _ENTITY_H
#define _ENTITY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Entity;

typedef union {
    char *s;
    float f;
    int i;
    void *v;
}Attribute;

typedef struct Component {
    Attribute *attributes;
    void (*Update)(void);
    void (*Create)(void);
}Component;

typedef struct tagEntity {
    Entity *children;
    Component *components;
    void (*Start)(void);
    void (*Update)(void);
}Entity;

#ifdef __cplusplus
}
#endif

#endif
