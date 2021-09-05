#include "world.h"

static particle_t _empty = {EMPTY, -1, 0} ;
static particle_t _fire = {FIRE, 1, 0} ;
static particle_t _ember = {EMBER, 300, 0} ;
static particle_t _plant_ded = {PLANT, 200, 1} ;
static particle_t _smoke = {SMOKE, 200, 0} ;
static particle_t _soil = {SOIL, 1, 0} ;
static particle_t _water = {WATER, 200, 0} ;

static int lenx = 0;
static int leny = 0;
static particle_t *buff = NULL;

particle_properties_t pallete[NO_PARTICLE_TYPES] = 
{
	{EMPTY,		0x000000,	-1,	5},
	{SAND,		0xc2b280,	4,	0},
	{WATER,		0xd4f1f9,	3,	0},
	{MARSH,		0x3b8753,	1,	99},
	{STONE,		0x928e85,	69,	0}, // why on gods heaven not
	{FIRE,		0xe25822,	69, 	0},
	{WOOD,		0x966f33,	69,	50},
	{OIL,		0x3c5061,	2,	80},
	{EMBER,		0x606070,	4,	0},
	{LAVA,		0xcf0820,	3,	0},
	{SOIL,		0x9b7653,	4,	0},
	{PLANT,		0x00ff00,	69,	50},
	{SMOKE,		0x738276,	0,	0},

	{POWDER_1,	0xff0000,	4,	3},
	{POWDER_2,	0xffa500,	4,	3},
	{POWDER_3,	0xffff00,	4,	3},
	{POWDER_4,	0x008000,	4,	3},
	{POWDER_5,	0x0000ff,	4,	3},
	{POWDER_6,	0x4b0082,	4, 	3},
	{POWDER_7,	0xee82ee,	4,	3},

	{SOLID_1, 	0xfd0000,	69,	12},
	{SOLID_2, 	0xfda300,	69,	12},
	{SOLID_3, 	0xfdfd00,	69,	12},
	{SOLID_4, 	0x007600,	69,	12},
	{SOLID_5, 	0x0000fd,	69,	12},
	{SOLID_6, 	0x390079,	69,	12},
	{SOLID_7,	0xee82ee,	69,	12}
} ;

void world_update(particle_t *buffer, int x, int y)
{
	lenx = x ;
	leny = y ;
	buff = buffer ;

	for(int i = 0; i < y; i++)
	{
		for(int j = 0; j < x; j++)
		{
			particle_t *p = getParticle(j, i);

			switch(p->id)
			{
				case SAND:  update_sand(j, i); break ;
				case WATER: update_water(j, i); break ;
				case MARSH: update_marsh(j, i); break ;
				case STONE: /* no movement */ break ;
				case WOOD: /* no movement */ break ;
				case FIRE:  update_fire(j, i); break ;
				case OIL:  update_oil(j, i); break ;
				case EMBER: update_ember(j, i) ; break ;
				case LAVA: update_lava(j, i); break ;
				case SOIL: update_soil(j, i); break ;
				case SMOKE: update_smoke(j, i); break ;

				case POWDER_1: update_sand(j, i); break;
				case POWDER_2: update_sand(j, i); break;
				case POWDER_3: update_sand(j, i); break;
				case POWDER_4: update_sand(j, i); break;
				case POWDER_5: update_sand(j, i); break;
				case POWDER_6: update_sand(j, i); break;
				case POWDER_7: update_sand(j, i); break;

				case SOLID_1: /* nothing at all */ break;
				case SOLID_2: /* nothing at all */ break;
				case SOLID_3: /* nothing at all */ break;
				case SOLID_4: /* nothing at all */ break;
				case SOLID_5: /* nothing at all */ break;
				case SOLID_6: /* nothing at all */ break;
				case SOLID_7: /* nothing at all */ break;

				case PLANT: /* we don't need over population*/ break ;
			}
		}
	}

	for(int i = 0; i < x * y; i++) buff[i].has_been_updated = 0 ;
}

////////////////////////////// STUFF SPECIFIC FUNCTIONS //////////////////////

void update_sand(int x, int y)
{
	if(hasBeenUpdated(x, y)) return ;

	particle_t *p = getParticle(x, y) ;
	p->has_been_updated = 1 ;

	move_solid(x, y) ;
}

void update_soil(int x, int y)
{
	if(hasBeenUpdated(x, y)) return ;

	particle_t *p = getParticle(x, y) ;
	p->has_been_updated = 1 ;

	if(getParticle(x, y - 1) != NULL && getParticle(x, y - 1)->id == WATER)
	{
		update_plant(x, y) ;
	}
	move_sappy_solid(x, y) ;
}

void update_ember(int x, int y)
{
	if(hasBeenUpdated(x, y)) return ;

	particle_t *p = getParticle(x, y) ;	
	p->has_been_updated = 1 ;

	p->life_time--;
	if(p->life_time < 0)
	{
		if(l_rand()%2)
			setParticle(x, y, _smoke) ;
		else
			setParticle(x, y, _soil) ;
	}

	if(inBounds(x, y - 1) && isEmpty(x, y - 1) && !hasBeenUpdated(x, y - 1))
	{
		setParticle(x, y-1, _fire) ;
	}

	move_solid(x, y) ;
}

void update_water(int x, int y)
{
	if(hasBeenUpdated(x, y)) return ;

	particle_t *p = getParticle(x, y) ;
	p->has_been_updated = 1 ;

	move_liquid(x, y) ;
}

void update_lava(int x, int y)
{
	if(hasBeenUpdated(x, y)) return ;

	particle_t *p = getParticle(x, y) ;
	p->has_been_updated = 1 ;

	p->life_time--;
	if(p->life_time < 0) setParticle(x, y, _ember) ;

	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			if(j == 0 || i == 0) continue ;
			if(inBounds(x + j, y + i) && !hasBeenUpdated(x + j, y + i))
			{
				particle_t *n = getParticle(j + x, i + y) ;
				if(pallete[n->id].flamability > 5)
					setParticle(j + x, i + y, _ember) ;
				if(n->id == WATER)
					setParticle(j + x, i + y, _smoke) ;
			}
		}
	}

	move_liquid(x, y) ;
}

void update_oil(int x, int y)
{
	if(hasBeenUpdated(x, y)) return ;

	particle_t *p = getParticle(x, y) ;
	p->has_been_updated = 1 ;

	move_liquid(x, y) ;
}

void update_smoke(int x, int y)
{
	if(hasBeenUpdated(x, y)) return ;

	particle_t *p = getParticle(x, y) ;	
	p->has_been_updated = 1 ;

	p->life_time--;
	if(p->life_time < 0) setParticle(x, y, _water) ;
	
	move_gas(x, y) ;
}

void update_marsh(int x, int y)
{
	if(hasBeenUpdated(x, y)) return ;

	particle_t *p = getParticle(x, y) ;	
	p->has_been_updated = 1 ;

	p->life_time--;
	if(p->life_time < 0) setParticle(x, y, _empty) ;
	
	move_gas(x, y) ;
}

void update_plant(int x, int y)
{
	for(int i = 0; i > -10; i--)
	{
		switch(l_rand()%2)
		{
			case 0:
				setParticle(x + 1*i/2, y + i, _plant_ded) ; break ;
			case 1:
				setParticle(x - 1*i/2, y + i, _plant_ded) ; break ;
		}
	}
}

void update_fire(int x, int y)
{
	if(hasBeenUpdated(x, y)) return ;

	particle_t *p = getParticle(x, y) ;	
	p->has_been_updated = 1 ;

	p->life_time--;
	if(p->life_time < 0) setParticle(x, y, _empty) ;

	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			if(inBounds(x + j, y + i) && !hasBeenUpdated(x + j, y + i))
			{
				particle_t *p = getParticle(x + j, y + i) ;

				if(pallete[p->id].flamability < 0)
				{
					setParticle(x, y, _empty) ;
					goto A ;
				}

				int chance = pallete[p->id].flamability ;
				if(chance > l_rand()%100)
				{
					setParticle(j + x, i + y, _fire) ;
					p->has_been_updated = 1 ;
				}
			}
		}
	}
A: (void)x ;
}

////////////////////////////// MOVEMENT FUNCTIONS //////////////////////////

void move_sappy_solid(int x, int y)
{
	int chk1 = isLighter(x, y, x + 0, y + 1) ;
	if(chk1) swap_particle(x, y, x + 0, y + 1) ;
}
void move_solid(int x, int y)
{
	int chk1 = isLighter(x, y, x + 0, y + 1) ;
	int chk2 = isLighter(x, y, x + 1, y + 1) ;
	int chk3 = isLighter(x, y, x - 1, y + 1) ;

	if(chk1) swap_particle(x, y, x + 0, y + 1) ;
	else if(chk2) swap_particle(x, y, x + 1, y + 1) ;
	else if(chk3) swap_particle(x, y, x - 1, y + 1) ;
	else if(chk2 || chk3)
	{	
		if(chk2 && !chk3) swap_particle(x, y, x + 1, y + 1) ;
		else if(chk3 && !chk2) swap_particle(x, y, x - 1, y + 1) ;
		else
		{
			switch(l_rand()%2)
			{
				case 0:
					swap_particle(x, y, x + 1, y + 1 ) ; break ;
				case 1:
					swap_particle(x, y, x - 1, y + 1 ) ; break ;
			}
		}
	}
}

void move_liquid(int x, int y)
{
	int chk1 = isLighter(x, y, x + 0, y + 1) ;
	int chk2 = isLighter(x, y, x + 1, y + 1) ;
	int chk3 = isLighter(x, y, x - 1, y + 1) ;
	int chk4 = isLighter(x, y, x + 1, y + 0) ;
	int chk5 = isLighter(x, y, x - 1, y + 0) ;

	if(chk1) swap_particle(x, y, x + 0, y + 1) ;
	else if(chk2 || chk3)
	{	
		if(chk2 && !chk3) swap_particle(x, y, x + 1, y + 1) ;
		else if(chk3 && !chk2) swap_particle(x, y, x - 1, y + 1) ;
		else
		{
			switch(l_rand()%2)
			{
				case 0:
					swap_particle(x, y, x + 1, y + 1) ; break ;
				case 1:
					swap_particle(x, y, x - 1, y + 1) ; break ;
			}
		}
	}
	else if(chk4 || chk5) {
		if(chk4 && chk5)
		{
			switch(l_rand()%2)
			{
				case 0:
					swap_particle(x, y, x + 1, y + 0) ; break ;
				case 1:
					swap_particle(x, y, x - 1, y + 0) ; break ;
			}
		}

		if(chk4 && !chk5) swap_particle(x, y, x + 1, y + 0) ;
		else if(chk5 && !chk4) swap_particle(x, y, x - 1, y + 0) ;
	}
}

void move_gas(int x, int y)
{
	int chk1 = isLighter(x, y, x + 0, y - 1) ;
	int chk2 = isLighter(x, y, x + 1, y - 1) ;
	int chk3 = isLighter(x, y, x - 1, y - 1) ;
	int chk4 = isLighter(x, y, x + 1, y - 0) ;
	int chk5 = isLighter(x, y, x - 1, y - 0) ;

	if(chk1) swap_particle(x, y, x + 0, y - 1) ;
	else if(chk2 || chk3)
	{	
		if(chk2 && !chk3) swap_particle(x, y, x + 1, y - 1) ;
		else if(chk3 && !chk2) swap_particle(x, y, x - 1, y - 1) ;
		else
		{
			switch(l_rand()%2)
			{
				case 0:
					swap_particle(x, y, x + 1, y - 1) ; break ;
				case 1:
					swap_particle(x, y, x - 1, y - 1) ; break ;
			}
		}
	}
	else if(chk4 || chk5)
	{
		if(chk4 && chk5)
		{
			switch(l_rand()%2)
			{
				case 0:
					swap_particle(x, y, x + 1, y - 0) ; break ;
				case 1:
					swap_particle(x, y, x - 1, y - 0) ; break ;
			}
		}

		if(chk4 && !chk5) swap_particle(x, y, x + 1, y - 0) ;
		else if(chk5 && !chk4) swap_particle(x, y, x - 1, y - 0) ;
	}
}
#if 0
void move_solid(int x, int y, particle_t p)
{
	if(!(l_rand()%600))
	{
		int lx = x + map(l_rand()%3, 0, 2, -2, 2);
		int ly = y + map(l_rand()%3, 0, 2, -2, 2);

		if(isEmpty(lx, ly))
		{
			setParticle(x, y, _empty) ;
			x = lx ;
			y = ly ;
		}
	}

	int chk1 = isEmpty(x + 0, y + 1) ;
	int chk2 = isEmpty(x + 1, y + 1) ;
	int chk3 = isEmpty(x - 1, y + 1) ;

	if(chk1) setParticle(x + 0, y + 1, p) ;
	else if(chk2) setParticle(x + 1, y + 1, p) ;
	else if(chk3) setParticle(x - 1, y + 1, p) ;
	else if(chk2 || chk3)
	{	
		if(chk2 && !chk3) setParticle(x + 1, y + 1, p) ;
		else if(chk3 && !chk2) setParticle(x - 1, y + 1, p) ;
		else
		{
			switch(l_rand()%2)
			{
				case 0:
					setParticle(x + 1, y + 1, p) ; break ;
				case 1:
					setParticle(x - 1, y + 1, p) ; break ;
			}
		}
	}

	if(chk1 || chk2 || chk3)
		setParticle(x, y, _empty) ;
}

void move_liquid(int x, int y, particle_t p)
{
	if(!(l_rand()%200))
	{
		int lx = x + map(l_rand()%3, 0, 2, -2, 2);
		int ly = y + map(l_rand()%3, 0, 2, -2, 2);

		if(isEmpty(lx, ly))
		{
			setParticle(x, y, _empty) ;
			x = lx ;
			y = ly ;
		}
	}
	int chk1 = isEmpty(x + 0, y + 1) ;
	int chk2 = isEmpty(x + 1, y + 1) ;
	int chk3 = isEmpty(x - 1, y + 1) ;
	int chk4 = isEmpty(x + 1, y + 0) ;
	int chk5 = isEmpty(x - 1, y + 0) ;

	if(chk1) setParticle(x + 0, y + 1, p) ;
	else if(chk2 || chk3)
	{	
		if(chk2 && !chk3) setParticle(x + 1, y + 1, p) ;
		else if(chk3 && !chk2) setParticle(x - 1, y + 1, p) ;
		else
		{
			switch(l_rand()%2)
			{
				case 0:
					setParticle(x + 1, y + 1, p) ; break ;
				case 1:
					setParticle(x - 1, y + 1, p) ; break ;
			}
		}
	}
	else if(chk4 || chk5) {
		if(chk4 && chk5)
		{
			switch(l_rand()%2)
			{
				case 0:
					setParticle(x + 1, y + 0, p) ; break ;
				case 1:
					setParticle(x - 1, y + 0, p) ; break ;
			}
		}

		if(chk4 && !chk5) setParticle(x + 1, y + 0, p) ;
		else if(chk5 && !chk4) setParticle(x - 1, y + 0, p) ;
	}

	if(chk1 || chk2 || chk3 || chk4 || chk5)
		setParticle(x, y, _empty) ;
}

void move_gas(int x, int y, particle_t p)
{
	if(!(l_rand()%125))
	{
		int lx = x + map(l_rand()%3, 0, 2, -2, 2);
		int ly = y + map(l_rand()%3, 0, 2, -2, 2);

		if(isEmpty(lx, ly))
		{
			setParticle(x, y, _empty) ;
			x = lx ;
			y = ly ;
		}
	}

	int chk1 = isEmpty(x + 0, y - 1) ;
	int chk2 = isEmpty(x + 1, y - 1) ;
	int chk3 = isEmpty(x - 1, y - 1) ;
	int chk4 = isEmpty(x + 1, y - 0) ;
	int chk5 = isEmpty(x - 1, y - 0) ;

	if(chk1) setParticle(x + 0, y - 1, p) ;
	else if(chk2 || chk3)
	{	
		if(chk2 && !chk3) setParticle(x + 1, y - 1, p) ;
		else if(chk3 && !chk2) setParticle(x - 1, y - 1, p) ;
		else
		{
			switch(l_rand()%2)
			{
				case 0:
					setParticle(x + 1, y - 1, p) ; break ;
				case 1:
					setParticle(x - 1, y - 1, p) ; break ;
			}
		}
	}
	else if(chk4 || chk5)
	{
		if(chk4 && chk5)
		{
			switch(l_rand()%2)
			{
				case 0:
					setParticle(x + 1, y - 0, p) ; break ;
				case 1:
					setParticle(x - 1, y - 0, p) ; break ;
			}
		}

		if(chk4 && !chk5) setParticle(x + 1, y - 0, p) ;
		else if(chk5 && !chk4) setParticle(x - 1, y - 0, p) ;
	}

	if(chk1 || chk2 || chk3 || chk4 || chk5)
		setParticle(x, y, _empty) ;
}
#endif

////////////////////////////// HELPER FUNCTIONS ///////////////////////////

int inBounds(int x, int y)
{
	return (x < lenx && y < leny && x >= 0 && y >= 0);
}

int isEmpty(int x, int y)
{
	return inBounds(x, y) && getParticle(x, y)->id == EMPTY;
}

int isLighter(int x, int y, int x1, int y1)
{
	return inBounds(x, y) && inBounds(x1, y1)
		&& (pallete[getParticle(x, y)->id].weight > pallete[getParticle(x1, y1)->id].weight) ;
}

particle_t *getParticle(int x, int y)
{
	if(inBounds(x, y)) return &buff[y * lenx + x] ;
	else return NULL ;
}

void setParticle(int x, int y, particle_t p)
{
	if(inBounds(x, y))
	{
		buff[y * lenx + x] = p ;
	}
}

int hasBeenUpdated(int x, int y)
{
	particle_t *p = getParticle(x, y) ;
	
	// sending out one for error becuase if its null then we don't want to 
	// do anything to it.
	return p != NULL ? p->has_been_updated : 1 ;
}

int cmp_weight(int x, int y, int x1, int y1)
{
	particle_t *p = getParticle(x, y) ;
	particle_t *p1 = getParticle(x, y) ;

	if(p == NULL) return -0xFEED ;
	else if(p1 == NULL) return -0xFEED ;

	return pallete[p->id].weight - pallete[p1->id].weight ;
}

int is_surrounded(int x, int y)
{	
	int sum = 0 ;
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			if(!isEmpty(j+x, i+y)) sum++ ;
		}
	}
	
	return sum == 9;
}

void swap_particle(int x, int y, int x1, int y1)
{
	if(!inBounds(x, y) || !inBounds(x1, y1)) return ;

	particle_t p1 = *getParticle(x, y) ;
	particle_t p2 = *getParticle(x1, y1) ;

	setParticle(x, y, p2) ;
	setParticle(x1, y1, p1) ;
}
