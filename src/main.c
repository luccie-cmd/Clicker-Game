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
#define BUY_SOUND "./res/sfx/Buy.wav"
#define CLICK_SOUND "./res/sfx/click.wav"
#define CLICK_TEXTURE "./res/gfx/click.png"
#define FONT_PATH "./res/fonts/8bitOperatorPlus-Regular.ttf"
#define FAIL_INIT 1
#define RENDER_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
//#include <SDL2/SDL_opengl.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "render.h"
#include "meth.h"
#include "other.h"

// render SDL

#ifdef RENDER_SDL

void PlaySound(const char* path){
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (!sound) {
        printf("Failed to load sound file: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, sound, 0);
}

void Start(SDL_Renderer* renderer, bool start_screen, SDL_Event event){
    while(start_screen){
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    start_screen = false;
                break;
                case SDL_KEYDOWN: {
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE: {
                            start_screen = false;
                        }
                    }
                } break;
                case SDL_MOUSEBUTTONDOWN: {
                    switch(event.button.button){
                        case SDL_BUTTON_LEFT: {
                            start_screen = false;

                        }
                    }
                }
            }
        }
    }
}

void End_Screen(bool end_screen, TTF_Font* font, Vec2i window_size, SDL_Event event, Mouse m, SDL_Renderer* renderer){
    int window_w = window_size.x;
    int window_h = window_size.y;
    int mousex, mousey;
    char* ctext = "You have completed the game";
    Text end_text = text(font, ctext, vec2i(window_w/2-(get_surface_size(font, ctext).x/2), window_h/2), vec4i(255, 255, 255, 255));
    Clickable quit = clickable(vec2i(window_w/2-50, window_h-100), vec2i(100, 50), vec4i(255, 255, 255, 255), NULL);
    while(end_screen){
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    end_screen = false;
                break;
                case SDL_KEYDOWN: {
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE: {
                            end_screen = false;
                        }
                    }
                } break;
                case SDL_MOUSEBUTTONDOWN: {
                    SDL_GetMouseState(&mousex, &mousey);
                    Vec2i mouse_pos = vec2i(mousex, mousey);
                    m.hitbox.pos = m.pos = mouse_pos;
                    switch(event.button.button){
                        case SDL_BUTTON_LEFT: {
                            if(aabb_collision(quit.hitbox, m.hitbox)){
                                end_screen = false;
                            }
                        }
                    }
                } break;
            }
        }
        SDL_RenderClear(renderer);
        DrawText(renderer, end_text);
        DrawClickable(renderer, quit);
        SDL_RenderPresent(renderer);
    }
}

void quit(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_CloseAudio();
    exit(0);
}

void main(){
    // C types (and SDL types)
    int mousex, mousey, collect_per_second = 0, collect=0, fps=60, desiredDelta=1000/fps, frames_passed=0, click_power=1, window_w=1280, window_h=720;
    char ccolect[50], ccollect_per_second[50], cclickpower[50];
    bool end_screen = false, should_quit = false;
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
    SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    if(window == NULL){
        printf("Could not make window reason: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    TTF_Font* font = TTF_OpenFont(FONT_PATH, 24);
    if (!font) {
        printf("TTF_OpenFont error: %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Event event;
    SDL_Texture* click_texture = get_texture(renderer, CLICK_TEXTURE);
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
        clickable(vec2i(window_w-30, 240), vec2i(30, 30), vec4i(255, 255, 255, 255), NULL), 
    };
    size_t click_len = sizeof(clickables)/sizeof(clickables[0]);
    Text collectText =            text(font, "Collect = ",            vec2i(0, 0), vec4i(255, 255, 255, 255));
    Text collectInt =             text(font, ccolect,                 vec2i(get_surface_size(font, "Collect = ").x, 0), vec4i(255, 255, 255, 255));
    Text collect_per_secondText = text(font, "Collect Per Second = ", vec2i(0, get_surface_size(font, "Collect = ").y), vec4i(255, 255, 255, 255));
    Text collect_per_secondInt =  text(font, ccollect_per_second,     vec2i(get_surface_size(font, "Collect Per Second = ").x, get_surface_size(font, "Collect = ").y), vec4i(255, 255, 255, 255));
    Text clickPowerText =         text(font, "Click Power = ",           vec2i(0, get_surface_size(font, "Collect = ").y*2), vec4i(255, 255, 255, 255));
    Text clickPowerInt =          text(font, cclickpower,             vec2i(get_surface_size(font, "Click Power = ").x, get_surface_size(font, "Collect = ").y*2), vec4i(255, 255, 255, 255));
    Text priceText = text(font, "Price -          Upgrade", vec2i(window_w-get_surface_size(font, "Price -          Upgrade").x, 0), vec4i(255, 255, 255, 255));
    Text price1text = text(font, "10       Click Power + 1", vec2i(window_w-get_surface_size(font, "10       Click Power + 1").x-50, 30), vec4i(255, 255, 255, 255));
    Text price2text = text(font, "100   Collect Per Second + 5", vec2i(window_w-get_surface_size(font, "100   Collect Per Second + 5").x-50, 60+5), vec4i(255, 255, 255, 255));
    Text price3text = text(font, "500   Collect Per Second + 20", vec2i(window_w-get_surface_size(font, "500   Collect Per Second + 20").x-50, 90+10), vec4i(255, 255, 255, 255));
    Text price4text = text(font, "1000   Collect Per Second + 50", vec2i(window_w-get_surface_size(font, "1000   Collect Per Second + 50").x-50, 120+15), vec4i(255, 255, 255, 255));
    Text price5text = text(font, "5000   Collect Per Second + 200", vec2i(window_w-get_surface_size(font, "5000   Collect Per Second + 200").x-50, 150+20), vec4i(255, 255, 255, 255));
    Text price6text = text(font, "10000      Click Power * 2", vec2i(window_w-get_surface_size(font, "10000           Click Power * 2").x-50, 180+25), vec4i(255, 255, 255, 255));
    Text price7text = text(font, "15000   Collect Per Second + 1000", vec2i(window_w-get_surface_size(font, "15000   Collect Per Second + 1000").x-50, 210+30), vec4i(255, 255, 255, 255));
    Mouse m = mouse(vec2i(0, 0));

    // -----------------------------------------------------------------------------------------

    while(!should_quit){
        int startLoop = SDL_GetTicks();
        if(collect >= 1000){
            end_screen = true;
            should_quit = true;
        }
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
                                            if(collect >= 10 && click_power <= 10000000){
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
                                        case 5: {
                                            if(collect >= 10000  && click_power <= 10000000){
                                                collect -= 10000;
                                                click_power *= 2;
                                                PlaySound(BUY_SOUND);
                                            }
                                            else{
                                                PlaySound("CantBuy.mp3");
                                            }
                                        } break;
                                        case 6: {
                                            if(collect >= 15000){
                                                collect -= 15000;
                                                collect_per_second += 1000;
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
        DrawClickables(renderer, clickables, click_len);
        DrawText(renderer, collectText);
        DrawText(renderer, collectInt);
        DrawText(renderer, collect_per_secondText);
        DrawText(renderer, collect_per_secondInt);
        DrawText(renderer, clickPowerText);
        DrawText(renderer, clickPowerInt);
        DrawText(renderer, priceText);
        DrawText(renderer, price1text);
        DrawText(renderer, price2text);
        DrawText(renderer, price3text);
        DrawText(renderer, price4text);
        DrawText(renderer, price5text);
        DrawText(renderer, price6text);
        DrawText(renderer, price7text);
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
    if(end_screen){
        End_Screen(end_screen, font, vec2i(window_w, window_h), event, m, renderer);
    }
    quit(window, renderer);
}

#else

void main(){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    if(SDL_Init(SDL_INIT_EVERYTHING) == FAIL_INIT){
        printf("Could not init SDL reason: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    if(window == NULL){
        printf("Could not make window reason: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_GL_CreateContext(window);
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
        printf("Could not initalize glad: %s\n", SDL_GetError());
        exit(1);
    }
    bool should_quit = false;
    while(!should_quit){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    should_quit = true;
                break;
                case SDL_KEYDOWN: {
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE: {
                            should_quit = true;
                        }
                    }
                } break;
            }
        }
    }
}

#endif // RENDER_SDL