#ifndef _RENDER_H
#define _RENDER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "types.h"

void SetRenderColor(SDL_Renderer* renderer, SDL_Color color);
void DrawRect(SDL_Renderer* renderer, SDL_Color color, Vec2i pos, Vec2i size);
void DrawClickable(SDL_Renderer* renderer, Clickable clickable);
void DrawClickables(SDL_Renderer* renderer, Clickable *clickables, size_t length);
void DrawText(SDL_Renderer* renderer, Text text);
void DrawTexture(SDL_Renderer* renderer, SDL_Texture* texture, Vec2i pos, Vec2i size);
Vec2i get_surface_size(TTF_Font *font, char* text);
#endif