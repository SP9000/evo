/*****************************************************************************/
/* The camera component - contains information to represent the window(s) to */
/* render within.                                                            */
/*                                                                           */
/* Bryce Wilson                                                              */
/* July 1, 2013                                                              */
/*****************************************************************************/
COMPONENT Camera {
    public Component_Transform* transform;
    public float fov;
    public float aspect;
    public float nearZ;
    public float farZ;

    public Mat4x4 modelMat;
    public Mat4x4 viewMat;
    public Mat4x4 projectionMat;

    public void Perspective(float fov, float aspect, float nearZ, float farZ)
    {
        Mat4x4LoadIdentity(self->modelMat);
        Mat4x4LoadIdentity(self->viewMat);
        Mat4x4LoadIdentity(self->projectionMat);
        Mat4x4PerspMat(self->projectionMat, fov, aspect, nearZ, farZ);
    }
    public void Orthographic(float left, float right, float top, float bottom,
            float nearZ, float farZ)
    {
        Mat4x4LoadIdentity(self->modelMat);
        Mat4x4LoadIdentity(self->viewMat);
        Mat4x4LoadIdentity(self->projectionMat);
        Mat4x4OrthoMat(self->projectionMat, 
                left, right, top, bottom, nearZ, farZ);
    }
    void Start() 
    {
        self->transform = Component_GetAs(Transform);
        Mat4x4LoadIdentity(self->modelMat);
        Mat4x4LoadIdentity(self->viewMat);
        Mat4x4LoadIdentity(self->projectionMat);
    }
    void Update() 
    {

    }
    void Collide(Entity* e)
    {
        puts("camera collision");
    }
}
