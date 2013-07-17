/*****************************************************************************/
/* The player motor component - attach this to a component to have it respond*/
/* to user input to move an entity.                                          */
/*                                                                           */
/* Bryce Wilson                                                              */
/* July 8, 2013                                                              */
/*****************************************************************************/
COMPONENT PlayerMotor {
    Component_Transform* transform;
    public float fallSpeed;
    public float jumpHeight;
    public float speed;

    void Start()
    {
        self->transform = Component_GetAs(Transform);
    }
    void Update() 
    {   
        if(Input_KeyDown(INPUT_KEY_RIGHT)) {
            self->transform->pos.x -= self->speed * time_delta;
        }
        if(Input_KeyDown(INPUT_KEY_LEFT)) {
            self->transform->pos.x += self->speed * time_delta;
        }
    }
    void Collide(Entity* e)
    {

    }
}
