#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "types.h"
#include "other.h"

/* checks if the 2 arguments overlap */
bool aabb_collision(AABB first, AABB second){
    if ((first.pos.x < second.pos.x + second.size.x) && (first.pos.x + first.size.x > second.pos.x) && 
        (first.pos.y < second.pos.y + second.size.y) && (first.size.y + first.pos.y > second.pos.y)) {
        // Collision detected!
        return true;
    } 
    else {
        // No collision
        return false;
    }
    return false;
}

SDL_Texture* get_texture(SDL_Renderer* renderer, const char* file_path){
    SDL_Texture* buffer = IMG_LoadTexture(renderer, file_path);
    if(buffer == NULL){
        printf("Warning %s\n", SDL_GetError());
        return NULL;
    }
    return buffer;
}