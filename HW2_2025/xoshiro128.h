#ifndef _XOSHIRO_H
#define _XOSHIRO_H

#include <stdint.h>

#define RND() (((double)next() / 4294967295.0))

uint32_t next(void);
extern void jump(void);
extern void long_jump(void);

static uint32_t s[4]={1,2,4,7};
#endif
