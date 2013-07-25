#include "texture.h"

/* a map of texture filenames to their texture handles to prevent reloading */
static GHashTable* loaded_textures;

int tv_texture_init()
{
    loaded_textures = g_hash_table_new(g_str_hash, g_str_equal);
    return 0;
}

void tv_texture_quit()
{
	
}

Texture tv_texture_load_bmp(char* file)
{
    int bpp;
    GLenum format;
    SDL_Surface* temp_surf;
    SDL_Surface* surface;
    Uint32 colorkey;
    Texture tex;

    tex.id = 0;
    tex.id = (GLuint)g_hash_table_lookup(loaded_textures, (gpointer)file);
    if(tex.id != 0) {
        return tex;
    }

    temp_surf = SDL_LoadBMP(file);
    if(temp_surf == NULL) {
        fprintf(stderr, "Error: couldn't load file %s\n", file);
        return tex;
    }
    colorkey = SDL_MapRGB(temp_surf->format, 0, 0, 0);
    SDL_SetColorKey(temp_surf, SDL_SRCCOLORKEY, colorkey);
    surface = SDL_DisplayFormatAlpha(temp_surf);
    SDL_FreeSurface(temp_surf);

    bpp = surface->format->BytesPerPixel;
    if(bpp == 4) {
        if(surface->format->Rmask == 0x000000ff) {
            format = GL_RGBA;
        }
        else {
            format = GL_BGRA;
        }
    }
    else if(bpp == 3) {
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
        return tex;
    }
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, bpp, surface->w, surface->h, 0, format,
            GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);
    return tex;
}

