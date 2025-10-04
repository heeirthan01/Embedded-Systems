#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "cache_model.h"

#define CACHE_SIZE (CM_CACHE_LINES*CM_LINE_SIZE) //total cache size

static int cache_tags[CM_CACHE_LINES]; // tags for each line, which memory block is stored
static int cache_enabled; // cache used or not
static int dram_buffer_tag; // ID buffer for uncached 
static void do_uncached_access(int address); //
static void do_cached_access(int address);
static int access_cycles;
uint32_t next(void);
void jump(void);

void cm_init()
{
	cm_reset_model();
}

void cm_reset_model()
{
	int i;
	for(i=0; i<CM_CACHE_LINES; i++)
		cache_tags[i] = -1;
	dram_buffer_tag = -1;
}

//Checks if the address is valid 
void cm_do_access(int address)
{

	if((address <0)|| (address >= CM_ADDRESS_SPACE_SIZE))
	{
		fprintf(stderr, "cm_do_access(%d): Address out of range!\n", address);
		exit(1);
	}
	if(cache_enabled)
		do_cached_access(address);
	else
		do_uncached_access(address);

}

void cm_enable_cache()
{
	cm_reset_model();
	cache_enabled = 1;
}

void cm_disable_cache()
{
	cm_reset_model();
	cache_enabled = 0;
}

static void do_cached_access(int address)
{
	int line = (address/CM_LINE_SIZE) % CM_CACHE_LINES; //cache line idex
	int tag = address / CM_LINE_SIZE; //block ID

	if(cache_tags[line] == tag)
	{	//Hit
		access_cycles = 1;
		printf("Hit cache: ");
	}
	else
	{	//Miss, fetch from DRAM
		printf("Missed cache: ");
		access_cycles = CM_DRAM_LATENCY + CM_CACHE_UPDATE_CYCLES
			+ CM_DRAM_CYCLES_PER_WORD * CM_LINE_SIZE;
		cache_tags[line] = tag;
	}
}

static void do_uncached_access(int address)
{
	int tag = address/CM_DRAM_BUFFER_SIZE;
	if (tag == dram_buffer_tag)
	{	//Hit
		access_cycles =1;
	}
	else
	{
		access_cycles = CM_DRAM_LATENCY
			+ CM_DRAM_CYCLES_PER_WORD * CM_DRAM_BUFFER_SIZE;
		dram_buffer_tag = tag;
	}
}

int cm_get_last_access_cycles()
{
	return access_cycles;
}

int rand_int(int b) //Returns a pseudo-random integer <= b
{
	return (unsigned)next() % b;
}

void rand_jump() //Jumps the generator by 2^96 -- provides a new, unrelated starting point
{
    jump();
}

/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide.

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */



/* This is xoshiro128** 1.1, one of our 32-bit all-purpose, rock-solid
   generators. It has excellent speed, a state size (128 bits) that is
   large enough for mild parallelism, and it passes all tests we are aware
   of. The state must be seeded so that it is not everywhere zero. */


static inline uint32_t rotl(const uint32_t x, int k) {
	return (x << k) | (x >> (32 - k));
}


static uint32_t s[4] = {1,4,7,3};

uint32_t next(void) {
	const uint32_t result = rotl(s[1] * 5, 7) * 9;

	const uint32_t t = s[1] << 9;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = rotl(s[3], 11);

	return result;
}

/* This is the long-jump function for the generator. It is equivalent to
   2^96 calls to next(); it can be used to generate 2^32 starting points,
   from each of which jump() will generate 2^32 non-overlapping
   subsequences for parallel distributed computations. */

void jump(void) {
	static const uint32_t LONG_JUMP[] = { 0xb523952e, 0x0b6f099f, 0xccf5a0ef, 0x1c580662 };

	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;
	for(int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
		for(int b = 0; b < 32; b++) {
			if (LONG_JUMP[i] & UINT32_C(1) << b) {
				s0 ^= s[0];
				s1 ^= s[1];
				s2 ^= s[2];
				s3 ^= s[3];
			}
			next();	
		}
		
	s[0] = s0;
	s[1] = s1;
	s[2] = s2;
	s[3] = s3;
}

// For problem a
int avg_cycle(int runs, bool cache_status, bool randjump)
{
	
	cm_init();
	if (cache_status == true)
	{
		cm_enable_cache();
	}
	else
	{
		cm_disable_cache();
	} 
	
	float sumtime = 0.0;
	for (int i = 0; i < runs; i++)
	{
		int b = CM_ADDRESS_SPACE_SIZE;
		int s = rand_int(b);
		cm_do_access(s);
		int ac = cm_get_last_access_cycles();
		sumtime += ac;
		printf("Runtime in cycels for run %f is %d\n", i, ac);
		if (randjump == true)
		{
			rand_jump();
		}
		
	}
	float avgtime = sumtime / runs;
	printf("Average run time over %d runs is: %.2f cycles\n", runs, avgtime);
	return avgtime;
}

// Part b
bool check_in_range(int address, int new_address)
{
	if (new_address < address - 40 || new_address > address + 40)
	{
		return true;
	}
	return false;
}

int rnd_genb(int address)
{
	double r = (double)rand() / RAND_MAX; 
	double s = 0.6;
	double p = 0.35;
	int new_address;

	if (r < s) // we hit the cache as we are in the line
	{
		new_address = address + 1;
	}
	else if(r < s + p) // within +- 40 of the previous address
	{
		int offset = rand() % 81 - 40;
		new_address = address + offset;
	}
	else{
		bool check = false;
		while (!check)
		{
			new_address = rand() % CM_ADDRESS_SPACE_SIZE;
			check = check_in_range(address, new_address);
		}
		
	}

	new_address = (new_address > 0) ? new_address : 0;
	new_address = (new_address >= CM_ADDRESS_SPACE_SIZE) ? CM_ADDRESS_SPACE_SIZE - 1 : new_address;

	return new_address;

}

int part_b(int runs, bool cache_status, bool randjump)
{
	int b = CM_ADDRESS_SPACE_SIZE;
	int s = rand_int(b);
	srand(time(0));
	cm_init();
	if (cache_status == true)
	{
		cm_enable_cache();
	}
	else
	{
		cm_disable_cache();
	} 
	
	float sumtime = 0.0;
	for (int i = 0; i < runs; i++)
	{
		
		if (i > 0)
		{	
			s = rnd_genb(s);
		}
		

		cm_do_access(s);
		int ac = cm_get_last_access_cycles();
		sumtime += ac;
		printf("Runtime in cycels for run %f is %d\n", i, ac);

		if (randjump == true)
		{
			rand_jump();
		}
		
	}
	float avgtime = sumtime / runs;
	printf("Average run time over %d runs is: %.2f cycles\n", runs, avgtime);
	return avgtime;
}

int main(){
	// Part a
	int r = 10000;
	bool cache_stat = true; // cache on or off
	bool randj = false; // with or without guaranteed new sequence
	//float avgt = avg_cycle(r, cache_stat, randj);

	// Part b
	int ev = part_b(r, cache_stat, randj);
	return 0;
}

