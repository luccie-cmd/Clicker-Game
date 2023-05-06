#ifndef _OTHER_H
#define _OTHER_H
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "types.h"

/* checks if the 2 arguments overlap */
bool aabb_collision(AABB first, AABB second);
SDL_Texture* get_texture(SDL_Renderer *renderer, const char* file_path);

#endif