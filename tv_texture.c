#include "tv_texture.h"

/* a map of texture filenames to their texture handles to prevent reloading */
static TvTexture* loaded_textures;

int tv_texture_init()
{
    return 0;
}

TvTexture *tv_texture_new()
{
	return (TvTexture*)tv_alloc(sizeof(TvTexture));
}

void tv_texture_quit()
{
	
}

TvTexture* tv_texture_load_bmp(tvchar* file)
{
    int bpp;
    GLenum format;
    SDL_Surface* temp_surf;
    SDL_Surface* surface;
    Uint32 colorkey;
    TvTexture* tex;

	HASH_FIND_PTR(loaded_textures, file, tex);
    if(tex != NULL) {
        return NULL;
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
	tex = tv_texture_new();
	glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, bpp, surface->w, surface->h, 0, format,
            GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);

	HASH_ADD_PTR(loaded_textures, name, tex);
    return tex;
}
