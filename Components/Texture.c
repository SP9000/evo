/*****************************************************************************/
/* Texture.c                                                                 */
/* this component contains the data and functions needed to render textures  */
/* and load them from files.                                                 */
/*                                                                           */
/* Bryce Wilson                                                              */
/* Created: June, 2013                                                       */
/*****************************************************************************/

#include "../draw.h"

static GHashTable* loadedTextures;

COMPONENT Texture {
    public unsigned width;
    public unsigned height;
    GLuint id;

    public void LoadBMP(char* file)
    {
        int bpp;
        GLenum format;

        self->id = (GLuint)g_hash_table_lookup(loadedTextures, (gpointer)file);
        if(self->id != 0) {
            return;
        }
        SDL_Surface* surface = SDL_LoadBMP(file);
        if(surface == NULL) {
            fprintf(stderr, "Error: couldn't load file %s\n", file);
            return;
        }
        bpp = surface->format->BytesPerPixel;
        if(bpp == 4) {
            if(surface->format->Rmask == 0x000000ff) {
                format = GL_RGBA;
            }
            else {
                format = GL_BGRA;
            }
        }
        if(bpp == 3) {
            if(surface->format->Rmask == 0x0000000ff) {
                format = GL_RGB;
            }
            else {
                format = GL_BGR;
            }
        }
        else {
            fprintf(stderr, "Error: file %s is not a truecolor image.\n", 
                    file);
            return;
        }
        glGenTextures(1, &self->id);
        glBindTexture(GL_TEXTURE_2D, self->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, bpp, surface->w, surface->h, 0, format,
                GL_UNSIGNED_BYTE, surface->pixels);
        SDL_FreeSurface(surface);
    }

    void Start() 
    {
        if(loadedTextures == NULL) {
            loadedTextures == g_hash_table_new(g_str_hash, g_str_equal);
        }
    }
    void Update() 
    {
        
    }
    void Collide(Entity* e)
    {

    }
}

