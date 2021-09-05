#ifndef _WORLD_H_
#define _WORLD_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

enum
{
	EMPTY,
	SAND,
	WATER,
	MARSH,
	STONE,
	FIRE,	// this particle is a bit weird but ok
	WOOD,
	OIL,
	EMBER,
	LAVA,
	SOIL,
	PLANT,
	SMOKE,

// more particles with cool colors for creative people

	POWDER_1,
	POWDER_2,
	POWDER_3,
	POWDER_4,
	POWDER_5,
	POWDER_6,
	POWDER_7,

	SOLID_1,
	SOLID_2,
	SOLID_3,
	SOLID_4,
	SOLID_5,
	SOLID_6,
	SOLID_7,
	NO_PARTICLE_TYPES
} ;

typedef struct
{
	uint8_t id ; // the type of particle
	uint32_t color ;
	int8_t weight ; // used for checking if the particle can displace others
	//int8_t state ; // the state of the substance
	int32_t flamability ; // used to check if the particle is flamable
	//uint32_t solubility ; // is surrounded by liquid the check if soluable
} particle_properties_t ;

extern particle_properties_t pallete[NO_PARTICLE_TYPES];

typedef struct
{
	uint8_t id ;
	int32_t life_time ;
	int32_t has_been_updated ;
} particle_t ;

void world_update(particle_t *buffer, int x, int y) ;

void update_sand(int x, int y) ;
void update_water(int x, int y) ;
void update_marsh(int x, int y) ;
void update_fire(int x, int y) ;
void update_oil(int x, int y) ;
void update_ember(int x, int y) ;
void update_lava(int x, int y) ;
void update_soil(int x, int y) ;
void update_plant(int x, int y) ;
void update_smoke(int x, int y) ;

int inBounds(int x, int y) ;
int isEmpty(int x, int y) ;
int hasBeenUpdated(int x, int y) ;

int isInLiquid(int x, int y) ;

void swap_particle(int x, int y, int x1, int y1) ;
void setParticle(int x, int y, particle_t p) ;
particle_t *getParticle(int x, int y) ;

void move_sappy_solid(int x, int y) ;
void move_solid(int x, int y) ;
void move_liquid(int x, int y) ;
void move_gas(int x, int y) ;

int cmp_weight(int x, int y, int x1, int y1) ;
int is_surrounded(int x, int y) ;

int isLighter(int x, int y, int x1, int y1) ;

#endif
