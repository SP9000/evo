/*****************************************************************************/
/* The animation component.                                                  */
/* The animation component contains the data to represent a group of model's */
/* as a heirarchal skeleton and to move this skeleton and the associated     */
/* models.                                                                   */
/* Since models already have some sort of heirarchy via subgroups, this      */
/* component stores just the root model, and you must access deeper levels   */
/* on a case-by-case basis.                                                  */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created July 8, 2013                                                      */
/*****************************************************************************/
#define ANIM_TRANSLATE 0 
#define ANIM_SCALE 1 
#define ANIM_ROTATE 2 

/**
 * Animation command structure.
 * The basic commands are: ANIM_TRANSLATE, ANIM_SCALE, and ANIM_ROTATE.
 */
typedef struct AnimationCmd {
    /* command to perform */
    int command;

    /* model to serve as root for this operation (all subgroups of model will
     * also have this operation applied). */
    struct Component_Model* base;

    /* amount to scale/translate/rotate by */
    Vector3 value;
}AnimationCmd;

Component Animation {
    public Component_Model* root;
    AnimationCmd** animations;

    void Start(Component_Animation* self) 
    {

    }
    void Update(Component_Animation* self) 
    {

    }
    void Collide(Entity* e)
    {
        
    }
}

