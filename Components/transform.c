/*****************************************************************************/
/* The transform component - common to all entities.                         */
/* The transform component contains the data to represent an entity's        */
/* location in the scene.                                                    */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: June, 2013                                                       */
/*****************************************************************************/

COMPONENT Transform {
    public Vector3 pos;
    public Vector3 scale;

    void Start() 
    {

    }
    void Update() 
    {

    }
    void Collide(Entity* e)
    {
        puts("transform collision");
    }
}

