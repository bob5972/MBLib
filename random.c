/*
 * random.c --
 */

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include "mbutil.h"
#include "random.h"
#include "mbassert.h"

typedef struct RandomGlobalData {
	bool initialized;
	uint64 seed;
	
	uint32 bitBucket;
	int bitBucketSize;
} RandomGlobalData;


static RandomGlobalData randomData;

uint64 Random_Init(void)
{	
	uint64 seed;
	bool haveSeed = FALSE;
	
	ASSERT(!randomData.initialized);
	
	randomData.initialized = TRUE;
	
	int fd;
	
	fd = open ("/dev/urandom", O_RDONLY);
	
	if (fd != -1) {
		int count;
		count = read(fd, &seed, sizeof(seed));
		if (count == sizeof(seed)) {
			haveSeed = TRUE;
		}
	    close (fd);
	}
    
    if (!haveSeed) {
    	pid_t pid = getpid();
	    seed = time(0) * pid + pid;
    	Random_Seed(seed);
    	
    	seed = Random_Uint64();
	    haveSeed = TRUE;
    }
    	
	randomData.bitBucket = 0;
	randomData.bitBucketSize = 0;
	
	ASSERT(haveSeed);
	Random_Seed(seed);
	
	return seed;
}

void Random_Exit(void)
{	
	randomData.initialized = FALSE;
}

void Random_Seed(uint64 seed)
{
	ASSERT(randomData.initialized);
	
	randomData.seed = seed;
}

bool Random_Bit(void)
{
	bool val;
	
	ASSERT(randomData.initialized);
	ASSERT(randomData.bitBucketSize >= 0);
	ASSERT(randomData.bitBucketSize <= 32);
	
	if (randomData.bitBucketSize == 0) {
		randomData.bitBucket = Random_Uint32();
		randomData.bitBucketSize = 32;
	}
	
	ASSERT(randomData.bitBucketSize > 0);
	randomData.bitBucketSize--;
	val = randomData.bitBucket & 0x1;
	randomData.bitBucket >>= 1;
	
	ASSERT(val == TRUE || val == FALSE);
	return val;
}

bool Random_Flip(float trueProb)
{
	//XXX: Naive implementation.

	ASSERT(trueProb >= 0);
	
	if (trueProb == 0) {
		return FALSE;
	}
	
	/*
	 * I could assert that it's <= 1, but I worry about people computing
	 * probabilities having things round slightly above 1.
	 */
	if (trueProb >= 1.0) {
		return TRUE;
	}
	
	if (trueProb == 0.5) {
		return Random_Bit();
	}
	
	if (Random_UnitFloat() <= trueProb) {
		return TRUE;
	} else {
		return FALSE;
	}
}

float Random_Float(float min, float max)
{
	double rval;
	double range;
	double dmin;
	double dmax;
	
	ASSERT(randomData.initialized);
	ASSERT(max >= min);

	//This might work, but I'm too lazy to figure out signs right now.
	ASSERT(max >= 0);
	ASSERT(min >= 0);
	
	if (min == max) {
		return min;
	}
	
	// Uniform randomData from [0, 1).
	rval = Random_UnitFloat();
	
	if (min == 0.0 && max == 1.0) {
		//Let's not screw up the obvious case here.
	} else {
		dmin = min;
		dmax = max;
		range = dmax - dmin;
	
		rval = rval * range + dmin;
	}
	
	ASSERT(rval >= min);
	ASSERT(rval <= max);

	return rval;
}


int Random_Enum(EnumDistribution *dist, int numValues)
{
	float cumulative;
	float rval;
	int x;
	ASSERT(numValues > 0);
	
	rval = Random_UnitFloat();
	
	cumulative = 0.0;
	for (x = 0; x < numValues; x++) {
		cumulative += dist[x].probability;
		if (rval < cumulative) {
			return dist[x].value;
		}
	}
	
	/*
	 * Rounding error, and/or caller is dumb.
	 */
	Log(0, "%s: Distribution exceeded: cumulative = %f\n", cumulative);
	return dist[numValues - 1].value;
}

uint32 Random_Uint32(void)
{
	static const uint64 constA = 2862933555777941757ULL;
	static const uint64 constB = 3037000493ULL;

	ASSERT(randomData.initialized);
	randomData.seed = constA * randomData.seed + constB;
	return randomData.seed >> 32;
}

uint64 Random_Uint64(void)
{
	uint64 a;
	uint32 b;
	
	a = Random_Uint32();
	b = Random_Uint32();
	
	return (a << 32) | b;
}

//XXX: Not verified for negative values or ranges near max int.
int Random_Int(int min, int max)
{
	int range;
	int rval;

	ASSERT(max >= min);

	if (min == max) {
		return min;
	}

	range = (max - min);
	rval = Random_Uint32();
	if (rval < 0) {
		rval *= -1;
	}

	rval = (rval % range) + min;

	ASSERT(rval <= max);
	ASSERT(rval >= min);

	return rval;
}

/*
 * Random_UnitFloat --
 * 	Returns a uniformly distributed float in the range [0, 1).
 */
float Random_UnitFloat(void)
{
	float rval;
	
	rval = ((float) Random_Uint32()) / (float) ((uint32) -1);
	
	return rval;
}
