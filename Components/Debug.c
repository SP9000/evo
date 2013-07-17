/*****************************************************************************/
/* The debug component.                                                      */
/* If this is attached to an object, it makes certain information visible    */
/* that may be useful for debugging.                                         */
/*****************************************************************************/
COMPONENT Debug {
    public Component_Transform* transform;
    public Component_Collider* col;
    public Component_Model* colModel;

    void Start()
    {
        self->transform = Component_GetAs(Transform);
        self->col = Component_GetAs(Collider);
        if(self->col) {
            /*
            AABB aabb = self->col->aabb;
             c->colModel = ModelGen_Box(c->transform->x, c->transform->y, c->transform->z,
                    aabb.w, aabb.h, aabb.d);
            */
        }
    }

    void Update() 
    {

    }

    void Collide(Entity* e)
    {
        puts("debug collision");
    }
}

