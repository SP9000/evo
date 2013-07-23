/*****************************************************************************/
/* ScrollWidget.c                                                            */
/* This component creates a widget with scrollbars that can be used to pan   */
/* around the content of the widget.                                         */ 
/*                                                                           */
/* Bryce Wilson                                                              */
/* created: July 23, 2013                                                    */
/*****************************************************************************/

COMPONENT ScrollWidget : Widget {
    /*************************************************************************/
    /*                            Data                                       */
    /*************************************************************************/
    public float scrollbar_width;
    public Texture scrollbar;
    public Texture scrollbar_bg;

    Rect virtual_rect;
    /*************************************************************************/
    /*                             Functions                                 */
    /*************************************************************************/
    public void AddWidget(Component_Widget* w, float x, float y)
    {
        self->children = g_slist_append(self->children, (gpointer)w);

        /* resize the virtual rectangle if necessary */
        if(Rect_Overlaps(&self->virtual_rect, &w->rect)) {
            if(w->rect.x < self->virtual_rect.x) {
                self->virtual_rect.w += w->rect.x - self->virtual_rect.x;
                self->virtual_rect.x = w->rect.x;
            }
            if(w->rect.y < self->virtual_rect.y) {
                self->virtual_rect.h += w->rect.y - self->virtual_rect.y;
                self->virtual_rect.y = w->rect.y;
            }
            if(w->rect.x+w->rect.w > self->virtual_rect.x+self->virtual_rect.w) {
                self->virtual_rect.w = (w->rect.x+w->rect.w) - (self->virtual_rect.x);
            }
            if(w->rect.x+w->rect.h > self->virtual_rect.x+self->virtual_rect.h) {
                self->virtual_rect.h = (w->rect.x+w->rect.h) - (self->virtual_rect.x);
            }
        }
    }

    /*************************************************************************/
    /*                        Default Functions                              */
    /*************************************************************************/
    /**
     * Start this component.
     * Called when the entity this is attached to is "Start"ed.
     */
    void Start() 
    {
        self->virtual_rect = self->rect;
    }
    /**
     * Update the component.
     * Called every frame.
     */
    void Update()
    {

    }
    /**
     * Handle collision with another entity.
     * @param other the other colliding entity.
     */
    void Collide(Entity* other)
    {

    }
}

