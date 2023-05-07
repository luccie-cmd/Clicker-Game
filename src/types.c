#include "types.h"

Vec4i vec4i(int x, int y, int w, int h){
    return (Vec4i) {.x = x, .y = y, .w = w, .h = h};
}

Vec2i vec2i(int x, int y){
    return (Vec2i) {.x = x, .y = y};
}

SDL_Color color_from_Vec4i(Vec4i color){
    return (SDL_Color) {.r = color.x, .g = color.y, .b = color.w, .a = color.h};
}

AABB aabb(Vec2i pos, Vec2i size){
    return (AABB) {.pos = pos, .size = size};
}

Clickable clickable(Vec2i pos, Vec2i size, Vec4i color, SDL_Texture* texture){
    Clickable buffer = {.pos.x = pos.x, .pos.y = pos.y, .size.x = size.x, .size.y = size.y, .color = color, .texture = texture};
    buffer.hitbox = aabb(buffer.pos, buffer.size);
    return buffer;
}

Mouse mouse(Vec2i pos){
    Mouse buffer = {.pos.x = pos.x, .pos.y = pos.y, .size.x = 4, .size.y = 4};
    buffer.hitbox = aabb(buffer.pos, buffer.size);
    return buffer;
}

Text text(TTF_Font* font, char* text, Vec2i pos, Vec4i color){
    Text buffer = {.font = font, .text = text, .pos = pos, .color = color};
    return buffer;
}