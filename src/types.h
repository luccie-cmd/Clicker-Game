#ifndef  _TYPES_H
#define  _TYPES_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct Vec4i{
    int x, y, w, h;
} Vec4i;

typedef struct Vec2i{
    int x, y;
} Vec2i;

typedef struct AABB{
    Vec2i pos;
    Vec2i size;
} AABB;

typedef struct Mouse{
    AABB hitbox;
    Vec2i pos;
    Vec2i size;
} Mouse;

typedef struct Clickable{
    Vec2i pos;
    Vec2i size;
    AABB hitbox;
    Vec4i color;
    SDL_Texture* texture;
} Clickable;

typedef struct Text{
    TTF_Font* font;
    char* text;
    Vec2i pos;
    Vec4i color;
} Text;

Vec4i vec4i(int x, int y, int w, int h);
Vec2i vec2i(int x, int y);
SDL_Color color_from_Vec4i(Vec4i color);
AABB aabb(Vec2i pos, Vec2i size);
Clickable clickable(Vec2i pos, Vec2i size, Vec4i color, SDL_Texture* texture);
Mouse mouse(Vec2i pos);
Text text(TTF_Font* font, char* text, Vec2i pos, Vec4i color);

#endif// _TYPES_H