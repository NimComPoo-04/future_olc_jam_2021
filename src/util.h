#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdarg.h>
#include <stdio.h>

#define PANIC(a, ...) __panik(__LINE__, __FILE__, __func__, a,##__VA_ARGS__)

static inline void __panik(int x, const char *fil, const char *fun, const char *dat, ...)
{
	va_list v ;
	printf("[ %d ][ %s ][ %s ]:  ", x, fil, fun) ;
	va_start(v, dat) ;
	vprintf(dat, v) ;
	va_end(v) ;
	printf("\n") ;

	exit(1) ;
}

#ifndef __EMSCRIPTEN__
static uint32_t __lehmer_state_rand__ = 0xffffffff;

static inline uint32_t l_rand()
{
	if(__lehmer_state_rand__ == 0xffffffff)
		__lehmer_state_rand__ = rand() ;
	return (__lehmer_state_rand__ = __lehmer_state_rand__ * 48271 % 0x7fffffff) ;
}
#else

static inline uint32_t l_rand()
{
	return rand() ;
}

#endif

static inline int map(int k, int a, int b, int c, int d)
{
	return (k - a)/(b - a) * (d - c) + c ;
}

#endif
