/*****************************************************************************/
/* TextRenderer.c                                                            */
/* This component is designed for rendering text, but really can be used to  */
/* render any sequence of models.                                            */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: July 19, 2013                                                    */
/*****************************************************************************/

/*  TODO: could make this actually do text..make another component for more  */
/* general stuff.                                                            */

COMPONENT TextRenderer : Renderer {
    public char* text;
    public char* font;

    public int len;
    public Component_Model** models;

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
        glUseProgram(self->mat->program);
        for(i = 0; i < text_len; ++i) {
            glBindVertexArray(self->models[i]->vao);
            glDrawArrays(self->models[i]->primitive, 0, 
                    self->models[i]->numVertices);
            glBindVertexArray(0);
        }
        Mat4x4Pop(viewMat);
    }
}

