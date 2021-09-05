#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"
#include "world.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void create_screen(int w, int h) ;
void delete_screen(void) ;

void main_game_loop(void(*)(void)) ;

void render_buffer_to_screen(particle_t *buffer, int x, int y, int c) ;
void event_handler(SDL_Event *eve) ;
void canvas_player(SDL_Event *eve) ;

SDL_Texture *renderText(const char *msg) ;
void render_ui(void) ;

#endif
