#include "render.h"

#define SPACER (100)
#define CELL (6)

static int width = 0;
static int height = 0;
static SDL_Window *win = NULL;
static SDL_Renderer *ren = NULL;
static SDL_Event eve ;
static TTF_Font *font;
static int pl = 1 ;

particle_t pic = {SAND, 1, 0};
int current_index = 0;
int brush_width = 2;

void sigsendbufferclear(void) ;

const char *names[NO_PARTICLE_TYPES] =
{
	"EMTPY",
	"SAND",
	"WATER",
	"MARSH",
	"STONE",
	"FIRE",
	"WOOD",
	"OIL",
	"EMBER",
	"LAVA",
	"SOIL",
	"PLANT",
	"SMOKE",

	"POWDER_1",
	"POWDER_2",
	"POWDER_3",
	"POWDER_4",
	"POWDER_5",
	"POWDER_6",
	"POWDER_7",

	"SOLID_1",
	"SOLID_2",
	"SOLID_3",
	"SOLID_4",
	"SOLID_5",
	"SOLID_6",
	"SOLID_7"
} ;

void create_screen(int w, int h)
{
	width = w ;
	height = h ;

	SDL_Init(SDL_INIT_VIDEO) ;

	if(TTF_Init() < 0) PANIC("FONT NOT FOUND!!");
	font = TTF_OpenFont("assets/CutiveMono-Regular.ttf", 36) ;
	if(font == NULL) PANIC("NOT FONT CALLED monospaced") ;

	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &win, &ren) ;
	if(win == NULL || ren == NULL) PANIC("WINDOW NOT CREATABLE!! SORRY :-(") ;
}

void render_buffer_to_screen(particle_t *buffer, int x, int y, int c)
{
	for(int i = 0; i < y; i++)
	{
		for(int j = 0; j < x; j++)
		{
			char r = (pallete[buffer[y * i + j].id].color >> 0x10) & 0xFF ;
			char g = (pallete[buffer[y * i + j].id].color >> 0x08) & 0xFF ;
			char b = (pallete[buffer[y * i + j].id].color >> 0x00) & 0xFF ;

			SDL_SetRenderDrawColor(ren, r, g, b, 0xFF) ;
			SDL_Rect rec = {j * c, i * c, c, c} ;
			SDL_RenderFillRect(ren, &rec) ;
		}
	}
}

void delete_screen(void)
{
	TTF_CloseFont(font) ;
	SDL_DestroyWindow(win) ;
	SDL_DestroyRenderer(ren) ;
	SDL_Quit() ;
}

void render_ui()
{
	SDL_Rect rec = {350, 600, 250, 100} ;
	SDL_Rect box = {2, 602, 100/3-4, 100/3-4} ;

	int index = 0 ;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			char r = (pallete[index].color >> 0x10) & 0xFF ;
			char g = (pallete[index].color >> 0x08) & 0xFF ;
			char b = (pallete[index].color >> 0x00) & 0xFF ;

			if(current_index == index)
			{
				SDL_Rect rxb = {box.x-2, box.y-2, 100/3, 100/3} ;
				SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, 0xFF) ;
				SDL_RenderDrawRect(ren, &rxb) ;
			}

			SDL_SetRenderDrawColor(ren, r, g, b, 0xFF) ;
			SDL_RenderFillRect(ren, &box) ;
			box.x = box.x + 100/3 ;
			index++ ;
		}
		box.x = 2 ;
		box.y = box.y + 100/3 ;
	}

	SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, 0xFF) ;
	SDL_Rect rrm = {304, 604, 42, 42} ;
	SDL_Rect rrmx = {304+(2 * brush_width + 10)/2, 604+(2 * brush_width + 10)/2,
		 2 * brush_width + 10, 2 * brush_width + 10} ;
	SDL_RenderDrawRect(ren, &rrm) ;
	SDL_RenderFillRect(ren, &rrmx) ;

	SDL_Rect ransack = {304, 654, 42, 42} ;

	SDL_Texture *tex = renderText(names[pic.id]) ;
	SDL_RenderCopy(ren, tex, NULL, &rec) ;

	SDL_Texture *tad = renderText("Reset") ;
	SDL_RenderCopy(ren, tad, NULL, &ransack) ;

	SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, 0xFF) ;
	SDL_RenderDrawRect(ren, &ransack) ;

	SDL_DestroyTexture(tex) ;
	SDL_DestroyTexture(tad) ;
}

void event_handler(SDL_Event *eve)
{
	canvas_player(eve) ;

	int x, y ;
	SDL_GetMouseState(&x, &y) ;
	if(y > 600 && y < 800 && x < 300 && x > 0)
	{
		int ly = (y - 600) / (100/3) ;
		int lx = x / (100/3) ;

		current_index = ly * 9 + lx ;

		if(eve->type == SDL_MOUSEBUTTONDOWN)
		{
			pic.id = pallete[current_index].id;
			pic.life_time = 150 ;
			pic.has_been_updated = 0 ;
		}
	}
	else if(y > 604 && y < 604+42 && x > 304 && x < 304+42)
	{
		if(eve->type == SDL_MOUSEBUTTONDOWN)
		{
			brush_width++ ;
			if(brush_width > 6) brush_width = 2 ;
		}
	}
	else if(y > 654 && y < 654 + 42 && x > 304 && x < 304 + 42)
	{
		if(eve->type == SDL_MOUSEBUTTONDOWN)
		{
			sigsendbufferclear() ;
		}
	}
}

SDL_Texture *renderText(const char *msg)
{
	SDL_Color col = {0xff, 0xff, 0xff} ;
	SDL_Surface *surf = TTF_RenderText_Blended(font, msg, col) ;
	if(surf == NULL) PANIC("SURFACE COULD NOT BE CREATED!!") ;

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf) ;
	if(tex == NULL) PANIC("TEXTURE COULD NOT BE CREATED!!") ;

	SDL_FreeSurface(surf) ;

	return tex;
}

static void Loop_internal(void *x)
{
	void(*Loop)() = (void(*)())x ;

	while(SDL_PollEvent(&eve))
	{
		if(eve.type == SDL_QUIT)
			pl = 0 ;
		event_handler(&eve) ;
	}
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren) ;
	Loop() ;
	SDL_RenderPresent(ren);
}

void main_game_loop(void(*Loop)(void))
{
#ifndef __EMSCRIPTEN__
	while(pl)
	{
		Loop_internal(Loop) ;
	}
#else
	emscripten_set_main_loop_arg(Loop_internal, (void*)Loop, 0, 1) ;
#endif
}
