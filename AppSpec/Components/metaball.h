DEFINE_COMPONENT(
        Metaball,
        ATTRIBUTE(float radius)
        ATTRIBUTE(DrawTarget dt)
        ATTRIBUTE(Material* mat1)
        ATTRIBUTE(Material* mat2)
        ATTRIBUTE(Model* model)
)
void Start(Component_Metaball* c) 
{
    /*
    int i, j;
    Texel* texture = (Texel*)malloc(512*512*sizeof(Texel))

    mat1 = Material_Load("tex.mat");
    mat2 = Material_New("tex.mat");
    for(i = 0; i < 512; ++i) {
        for(j = 0; j < 512; ++j) {
            texture[i*512+j].r = 100;
            texture[i*512+j].g = 100;
            texture[i*512+j].b = 200;
            texture[i*512+j].a = 255;
        }
    }
    free(texture);
    */
    /* use the generated texture for the first material (in the RTT render) */
    //Material_AddTexture(mat1, 512, 512, texture);

    c->model = Model_Load("TestAssets/monkey.ply");
    c->model->material = Material_Load("test.mat");
}

void Update(Component_Metaball* e)
{
    Draw_Model(c->model);
}

