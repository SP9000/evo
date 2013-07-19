/*****************************************************************************/
/* Text.c                                                                    */
/* The transform component - common to all entities.                         */
/* The transform component contains the data to represent an entity's        */
/* location in the scene.                                                    */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: June, 2013                                                       */
/*****************************************************************************/
#include "../entity.h"

COMPONENT Text {
    public char* text;
    public Component_Model** font;

    Entity* text_object;
    Component_Model* modeled_text;
    Component_Transform* positon_tform;

    void Start() 
    {
        int i;
        Component_Material* mat = Component_GetAs(Material);
        for(i = 0; i < strlen(self->text); ++i) {
            Entity* c = Entity_New();
            /* create/add a transform and model (for the character) */
            Component_Transform* t = Component_Transform_New();
                t->pos.x = i; t->pos.y = 0.0f; t->pos.z = 0.0f;
                t->scale.x = 0.0f; t->scale.y = 0.0f; t->scale.z = 0.0f;
            Entity_AddComponent(c, self->font[self->text[i]-'A']);
            Entity_AddComponent(c, t);
            Entity_AddComponent(c, mat);
            Entity_Start(c);
            Entity_AddChild(self->text_object, c);
        }
    }
    void Update() 
    {

    }
    void Collide(Entity* e)
    {

    }
}

