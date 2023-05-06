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
    AABB buffer;
    buffer.pos = pos;
    buffer.size = size;
    return buffer;
}

Clickable clickable(Vec2i pos, Vec2i size, Vec4i color, SDL_Texture* texture){
    Clickable buffer;
    buffer.color = color;
    buffer.hitbox = aabb(buffer.pos, buffer.size);
    buffer.pos = pos;
    buffer.size = size;
    buffer.texture = texture;
    return buffer;
}

Mouse mouse(Vec2i pos){
    Mouse buffer;
    buffer.hitbox = aabb(buffer.pos, buffer.size);
    buffer.pos = pos;
    buffer.size = vec2i(4, 4);
    return buffer;
}

Text text(TTF_Font* font, char* text, Vec2i pos, Vec4i color){
    Text buffer;
    buffer.color = color;
    buffer.font = font;
    buffer.pos = pos;
    buffer.text = text;
    return buffer;
}