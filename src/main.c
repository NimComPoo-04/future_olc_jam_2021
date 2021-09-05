#include "render.h"
#include "world.h"

void Loop(void) ;

#define SPACER (100)
#define CELL (6)

extern int brush_width ;
extern particle_t pic ;
particle_t *buffer ;

int main(int argc, char **args)
{
	create_screen(600, 700) ;
	buffer = calloc(SPACER * SPACER, sizeof(particle_t)) ;
	main_game_loop(Loop) ;
	delete_screen() ;
}

void Loop(void)
{
	world_update(buffer, SPACER, SPACER) ;
	render_ui() ;
	render_buffer_to_screen(buffer, SPACER, SPACER, CELL) ;
}

// serializing the enum for later
void canvas_player(SDL_Event *eve)
{
	int x, y ;
	SDL_GetMouseState(&x, &y) ;

	if(x > 0 && y > 0 && y < 600 && x < 600)
	{
		if(eve->type == SDL_MOUSEBUTTONDOWN)
		{
			for(int i = -brush_width; i < brush_width; i++)
			{
				for(int j = -brush_width; j < brush_width; j++)
				{
					if(i * i + j * j >= brush_width * brush_width ) continue ;
					if((y/CELL + i) < 0 || (y/CELL + i) >= SPACER ||
							(x/CELL + j) < 0 || (x/CELL + j) >= SPACER)
						continue ;
					buffer[SPACER * (y/CELL + i) + (x/CELL + j)] = pic;
				}
			}
		}
	}

	if(eve->type == SDL_KEYDOWN)
	{
		if(eve->key.keysym.sym == SDLK_SPACE)
		{
			for(int i = 0; i < SPACER; i++)
			{
				for(int j = 0; j < SPACER; j++)
				{
					buffer[i * SPACER + j] = (particle_t){l_rand()%SMOKE, 30, 0} ;
				}
			}
		}
	}
}

void sigsendbufferclear()
{
	memset(buffer, 0, sizeof(particle_t) * SPACER * SPACER) ;
}
