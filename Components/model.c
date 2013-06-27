#ifndef COMPONENT_MODEL
#define COMPONENT_MODEL

#include "../component.h"

DEFINE_COMPONENT(Model,
    ATTRIBUTE(Material* mat)
    ATTRIBUTE(float** attributes)
    ATTRIBUTE(int* attributeTable)
    ATTRIBUTE(int numAttributes)
    
    ATTRIBUTE(GLuint* vboIDs)
    ATTRIBUTE(GLuint vao)
    ATTRIBUTE(GLuint primitive)
    
    ATTRIBUTE(unsigned numVertices)
    ATTRIBUTE(struct Component_Model* subgroups),

    FUNCTION(LoadPLY),
    FUNCTION(BufferAttribute)
)


#ifdef BUILD
void Start(Component_Model* c)
{

}

void Update(Component_Model* c) 
{

}

void Collide(Entity* e)
{
    puts("model collision");
}

void LoadPLY(Component_Model* c, void* data)
{

}

void BufferAttribute(Component_Model* c, void* data)
{

}

#endif
#endif
