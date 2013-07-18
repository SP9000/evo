/*****************************************************************************/
/* Text.c                                                                    */
/* The transform component - common to all entities.                         */
/* The transform component contains the data to represent an entity's        */
/* location in the scene.                                                    */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: June, 2013                                                       */
/*****************************************************************************/

COMPONENT Text {
    public char* text;
    public Component_Model* font;
    Component_Model* modeled_text;

    void Start() 
    {
        int i;
        for(i = 0; i < strlen(self->text); ++i) {
            self->modeled_text[i] = self->font[self->text[i]-'A'];
        }
    }
    void Update() 
    {

    }
    void Collide(Entity* e)
    {

    }
}

