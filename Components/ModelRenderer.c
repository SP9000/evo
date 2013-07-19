/*****************************************************************************/
/* ModelRenderer.c                                                           */
/* One of the more basic renderers.  This just renders a single model.       */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 19, 2013                                                    */
/*****************************************************************************/

COMPONENT ModelRenderer : Renderer {
    public Component_Model* model;
    Component_Transform* transform;

    void Start() 
    {
    }
    void Update() 
    {
        
    }
    void Collide(Entity* e)
    {

    }
    void Render()
    {
        Mat4x4Push(viewMat);
        glBindVertexArray(self->models[i]->vao);
        glDrawArrays(self->models[i]->primitive, 0, 
                self->models[i]->numVertices);
        glBindVertexArray(0);
        Mat4x4Pop(viewMat);
    }
}

