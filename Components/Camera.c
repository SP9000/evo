/*****************************************************************************/
/* The camera component - contains information to represent the window(s) to */
/* render within.                                                            */
/*                                                                           */
/* Bryce Wilson                                                              */
/* July 1, 2013                                                              */
/*****************************************************************************/
COMPONENT Camera {
    getset Component_Transform* transform;

	/* the camera from the engine, which this component is a wrapper for */
	getset TvCamera* cam;

    public void Perspective(float fov, float aspect, float nearZ, float farZ)
    {
		tv_camera_perspective(self->cam, fov, aspect, nearZ, farZ)
    }
    public void Orthographic(float left, float right, float top, float bottom,
            float nearZ, float farZ)
    {
		tv_camera_orthographic(self->cam, left, right, top, bottom, nearZ, farZ);    
	}
    void Start() 
    {
        self->transform = Component_GetAs(Transform);
    }
    void Update() 
    {
    }
    void Collide(Entity* e)
    {
        puts("camera collision");
    }
}

