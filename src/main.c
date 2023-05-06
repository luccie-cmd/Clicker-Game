/* 
MiT License

Copyright (c) 2023 luccie-cmd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE iS PROViDED "AS iS", WiTHOUT WARRANTY OF ANY KiND, EXPRESS OR
iMPLiED, iNCLUDiNG BUT NOT LiMiTED TO THE WARRANTiES OF MERCHANTABiLiTY,
FiTNESS FOR A PARTiCULAR PURPOSE AND NONiNFRiNGEMENT. iN NO EVENT SHALL THE
AUTHORS OR COPYRiGHT HOLDERS BE LiABLE FOR ANY CLAiM, DAMAGES OR OTHER
LiABiLiTY, WHETHER iN AN ACTiON OF CONTRACT, TORT OR OTHERWiSE, ARiSiNG FROM,
OUT OF OR iN CONNECTiON WiTH THE SOFTWARE OR THE USE OR OTHER DEALiNGS iN THE
SOFTWARE.
*/
// i also appoligize if my english is not that good i'm a non native english speaker

#define SDL_MAIN_HANDLED
#define TITLE "Clicktopia"
#define BUY_SOUND "./Buy.wav"
#define CLICK_SOUND "./click.wav"
#define FAIL_INIT 1
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "types.h"
#include "render.h"
#include "meth.h"
#include "other.h"

void PlaySound(const char* path){
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (!sound) {
        printf("Failed to load sound file: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, sound, 0);
}

void main(){
    // C types (and SDL types)
    int mousex, mousey, collect_per_second = 0, collect=5000, fps=60, desiredDelta=1000/fps, frames_passed=0, click_power=1, window_w=1180, window_h=620;
    char ccolect[50], ccollect_per_second[50], cclickpower[50];
    sprintf(ccolect, "%d", collect);
    sprintf(ccollect_per_second, "%d", collect_per_second);
    sprintf(cclickpower, "%d", click_power);
    if(SDL_Init(SDL_INIT_EVERYTHING) == FAIL_INIT){
        printf("Could not init SDL reason: %s\n", SDL_GetError());
        exit(1);
    }
    if(!IMG_Init(IMG_INIT_PNG) == FAIL_INIT){
        printf("Could not init IMG reason: %s\n", SDL_GetError());
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Failed to initialize SDL Mixer: %s", Mix_GetError());
        exit(1);
    }
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf("Could not make window reason: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    TTF_Font* font = TTF_OpenFont("../bin/8bitOperatorPlus-Regular.ttf", 24);
    if (!font) {
        printf("TTF_OpenFont error: %s\n", TTF_GetError());
        exit(1);
    }
    bool should_quit = false;
    SDL_Event event;
    SDL_Texture* click_texture = get_texture(renderer, "./click.png");
    SDL_GetWindowSize(window, &window_w, &window_h);

    // CUSTOM -------------------------------------------

    Clickable click = clickable(vec2i(20, window_h/2), vec2i(50, 50), vec4i(255, 0, 0, 255), click_texture);
    Clickable clickables[] = {
        clickable(vec2i(window_w-30, 30), vec2i(30, 30), vec4i(255, 255, 255, 255), NULL),
        clickable(vec2i(window_w-30, 65), vec2i(30, 30), vec4i(255, 255, 255, 255), NULL),
        clickable(vec2i(window_w-30, 100), vec2i(30, 30), vec4i(255, 255, 255, 255), NULL),
        clickable(vec2i(window_w-30, 135), vec2i(30, 30), vec4i(255, 255, 255, 255), NULL),
        clickable(vec2i(window_w-30, 170), vec2i(30, 30), vec4i(255, 255, 255, 255), NULL),
        clickable(vec2i(window_w-30, 205), vec2i(30, 30), vec4i(255, 255, 255, 255), NULL),
    };
    Text texts[] = {
        text(font, "Collect = ",            vec2i(0, 0), vec4i(255, 255, 255, 255)),
        text(font, "Collect Per Second = ", vec2i(0, get_surface_size(font, "Collect = ").y), vec4i(255, 255, 255, 255)),
        text(font, "Click Power = ",           vec2i(0, get_surface_size(font, "Collect = ").y*2), vec4i(255, 255, 255, 255)),
        text(font, "Price - Collect Per Second", vec2i(window_w-get_surface_size(font, "Price - Collect Per Second").x-31, 0), vec4i(255, 255, 255, 255)),
        text(font, "  10       Click Power + 1", vec2i(window_w-get_surface_size(font, "Price - Collect Per Second").x-31, 30), vec4i(255, 255, 255, 255)),
        text(font, "  100         5", vec2i(window_w-get_surface_size(font, "Price - Collect Per Second").x-31, 60+5), vec4i(255, 255, 255, 255)),
        text(font, "  500        20", vec2i(window_w-get_surface_size(font, "Price - Collect Per Second").x-31, 90+10), vec4i(255, 255, 255, 255)),
        text(font, " 1000        50", vec2i(window_w-get_surface_size(font, "Price - Collect Per Second").x-31, 120+15), vec4i(255, 255, 255, 255)),
        text(font, " 5000        200", vec2i(window_w-get_surface_size(font, "Price - Collect Per Second").x-31, 150+20), vec4i(255, 255, 255, 255)),
        text(font, " 10000     Click Power * 2", vec2i(window_w-get_surface_size(font, "Price - Collect Per Second").x-31, 180+25), vec4i(255, 255, 255, 255)),
        text(font, " 20000      2000", vec2i(window_w-get_surface_size(font, "Price - Collect Per Second").x-31, 210+30), vec4i(255, 255, 255, 255)),
    };
    Text collectInt =             text(font, ccolect,                 vec2i(get_surface_size(font, "Collect = ").x, 0), vec4i(255, 255, 255, 255));
    Text collect_per_secondInt =  text(font, ccollect_per_second,     vec2i(get_surface_size(font, "Collect Per Second = ").x, get_surface_size(font, "Collect = ").y), vec4i(255, 255, 255, 255));
    Text clickPowerInt =          text(font, cclickpower,             vec2i(get_surface_size(font, "Click Power = ").x, get_surface_size(font, "Collect = ").y*2), vec4i(255, 255, 255, 255));
    Mouse m = mouse(vec2i(0, 0));
    
    // -----------------------------------------------------------------------------------------

    while(!should_quit){
        int startLoop = SDL_GetTicks();
        sprintf(ccolect, "%d", collect);
        sprintf(ccollect_per_second, "%d", collect_per_second);
        sprintf(cclickpower, "%d", click_power);
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT: {
                    should_quit = true;
                } break;
                case SDL_KEYDOWN: {
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE: {
                            should_quit = true;
                        } break;
                    }
                } break;
                case SDL_MOUSEBUTTONDOWN: {
                    SDL_GetMouseState(&mousex, &mousey);
                    Vec2i mouse_pos = vec2i(mousex, mousey);
                    m.hitbox.pos = m.pos = mouse_pos;
                    switch(event.button.button){
                        case SDL_BUTTON_LEFT: {
                            if(aabb_collision(click.hitbox, m.hitbox)){
                                PlaySound(CLICK_SOUND);
                                collect += click_power;
                            }
                            int length = sizeof(clickables) / sizeof(clickables[0]);
                            for(int i = 0; i <= length; ++i){
                                Clickable c = clickables[i];
                                if(aabb_collision(c.hitbox, m.hitbox)){
                                    switch(i){
                                        case 0: {
                                            if(collect >= 10){
                                                collect -= 10;
                                                click_power += 1;
                                                PlaySound(BUY_SOUND);
                                            }
                                            else{
                                                PlaySound("CantBuy.mp3");
                                            }
                                        } break;
                                        case 1: {
                                            if(collect >= 100){
                                                collect -= 100;
                                                collect_per_second += 5;
                                                PlaySound(BUY_SOUND);
                                            }
                                            else{
                                                PlaySound("CantBuy.mp3");
                                            }
                                        } break;
                                        case 2: {
                                            if(collect >= 500){
                                                collect -= 500;
                                                collect_per_second += 20;
                                                PlaySound(BUY_SOUND);
                                            }
                                            else{
                                                PlaySound("CantBuy.mp3");
                                            }
                                        } break;
                                        case 3: {
                                            if(collect >= 1000){
                                                collect -= 1000;
                                                collect_per_second += 50;
                                                PlaySound(BUY_SOUND);
                                            }
                                            else{
                                                PlaySound("CantBuy.mp3");
                                            }
                                        } break;
                                        case 4: {
                                            if(collect >= 5000){
                                                collect -= 5000;
                                                collect_per_second += 200;
                                                PlaySound(BUY_SOUND);
                                            }
                                            else{
                                                PlaySound("CantBuy.mp3");
                                            }
                                        } break;
                                        default: break;
                                    }
                                }
                            }
                        } break;
                    }
                } break;
            }
        }
        SDL_RenderClear(renderer);
        DrawClickable(renderer, click);
        DrawClickables(renderer, clickables);
        DrawTexts(renderer, texts, sizeof(texts)/sizeof(texts[0]));
        DrawText(renderer, collectInt);
        DrawText(renderer, collect_per_secondInt);
        DrawText(renderer, clickPowerInt);
        SDL_RenderPresent(renderer);
        frames_passed += 1;
        int delta = SDL_GetTicks()-startLoop;
        if(delta < desiredDelta){
            SDL_Delay(desiredDelta-delta);
        }
        if(frames_passed % 60 == 0){
            collect+=collect_per_second;
        }
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_CloseAudio();
    exit(0);
}