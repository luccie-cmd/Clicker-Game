#include "render.h"
#include "meth.h"
#include <stdio.h>

void SetRenderColor(SDL_Renderer* renderer, SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
void DrawRect(SDL_Renderer* renderer, SDL_Color color, Vec2i pos, Vec2i size){
    SetRenderColor(renderer, color);
    SDL_Rect rPos = {
        .x = pos.x,
        .y = pos.y,
        .w = size.x, 
        .h = size.y,
    };
    SDL_RenderFillRect(renderer, &rPos);
    SetRenderColor(renderer, color_from_Vec4i(vec4i(0, 0, 0, 255)));
}
void DrawClickable(SDL_Renderer* renderer, Clickable clickable){
    if(clickable.texture == NULL){
        DrawRect(renderer, color_from_Vec4i(clickable.color), clickable.pos, clickable.size);
    }
    else{
        DrawTexture(renderer, clickable.texture, clickable.pos, clickable.size);
    }
}
void DrawClickables(SDL_Renderer* renderer, Clickable *clickables){
    int length = sizeof(clickables)/2;
    for(int i = 0; i <= length; ++i){
        DrawClickable(renderer, clickables[i]);
    }
}

void DrawText(SDL_Renderer* renderer, Text text){
    SDL_Surface *surface = TTF_RenderText_Solid(text.font, text.text, color_from_Vec4i(text.color));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(surface == NULL || texture == NULL){
        printf("Could not make texture or surface: %s\n", SDL_GetError());
    }
    SDL_Rect dstrect = {.x = text.pos.x, .y = text.pos.y, .w = surface->w, .h = surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
}
void DrawTexts(SDL_Renderer* renderer, Text *texts, int size){
    int length = sizeof(texts[0])/sizeof(texts);
    for(int i = 0; i <= size; ++i){
        DrawText(renderer, texts[i]);
    }
}
void DrawTexture(SDL_Renderer* renderer, SDL_Texture* texture, Vec2i pos, Vec2i size){
    int texture_w, texture_h;
    if(SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h) < 0){
        printf("Error: %s\n", SDL_GetError);
        exit(1);
    }
    SDL_Rect src = {.x = 0, .y = 0, .w = clamp(size.x, texture_w, size.x), .h = clamp(size.y, texture_h, size.y)};
    SDL_Rect dst = {.x = pos.x, .y = pos.y, .w = clamp(size.x, texture_w, size.x), .h = clamp(size.y, texture_h, size.y)};
    SDL_RenderCopy(renderer, texture, &src, &dst);
}
Vec2i get_surface_size(TTF_Font* font, char* text){
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color_from_Vec4i(vec4i(255, 255, 255, 255)));
    Vec2i buffer = {.x = surface->w, .y = surface->h};
    return buffer;
}