/*****************************************************************************/
/* The collider component.                                                   */
/* This component contains information that allows the entity it is attached */
/* to be checked for collision with other entities that have colliders.      */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: June, 2013                                                       */
/*****************************************************************************/
COMPONENT Collider {
    public Component_Transform* transform;
    public AABB aabb;

    void Start()
    {
        self->transform = Component_GetAs(Transform);
        self->aabb.w = 1.0f;
        self->aabb.h = 1.0f;
        self->aabb.d = 1.0f;
    }

    void Update() 
    {

    }

    void Collide(Entity* e)
    {
        puts("collider collision");
    }
}

