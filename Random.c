/*
 * random.c -- part of MBLib
 *
 * Copyright (c) 2015-2021 Michael Banack <github@banack.net>
 *
 * MIT License
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include "MBUtil.h"
#include "Random.h"
#include "MBAssert.h"

typedef struct RandomGlobalData {
    uint initializedCount;
    bool haveSeed;
    uint64 seed;

    RandomState rs;
} RandomGlobalData;

static RandomGlobalData randomData;


void RandomState_Create(RandomState *r)
{
    MBUtil_Zero(r, sizeof(*r));
    RandomState_GenerateSeed(r);
}

void RandomState_CreateWithSeed(RandomState *r, uint64 seed)
{
    MBUtil_Zero(r, sizeof(*r));
    RandomState_SetSeed(r, seed);
}

void RandomState_Destroy(RandomState *r)
{
    // Nothing to do...
}

void RandomState_GenerateSeed(RandomState *r)
{
    /*
     * Use the module Random to generate the seeds, so that we don't
     * keep re-reading from /dev/urandom.  This also makes re-seeding
     * RandomState's deterministic if someone called Random_SetSeed
     * before this.
     */
    ASSERT(randomData.initializedCount > 0);
    RandomState_SetSeed(r, Random_Uint64());
}

uint64 RandomState_GetSeed(RandomState *r)
{
    return r->seed;
}

/*
 * The Workhorse of the entire module.
 */
void RandomState_SetSeed(RandomState *r, uint64 seed)
{
    r->seed = seed;
    r->value = r->seed;

    r->bitBucket = 0;
    r->bitBucketSize = 0;
}

uint32 RandomState_Uint32(RandomState *r)
{
    static const uint64 constA = 2862933555777941757ULL;
    static const uint64 constB = 3037000493ULL;

    r->value = constA * r->value + constB;
    return r->value >> 32;
}

bool RandomState_Bit(RandomState *r)
{
    bool val;

    ASSERT(r->bitBucketSize >= 0);
    ASSERT(r->bitBucketSize <= 32);

    if (r->bitBucketSize == 0) {
        r->bitBucket = RandomState_Uint32(r);
        r->bitBucketSize = 32;
    }

    ASSERT(r->bitBucketSize > 0);
    r->bitBucketSize--;
    val = r->bitBucket & 0x1;
    r->bitBucket >>= 1;

    ASSERT(val == TRUE || val == FALSE);
    return val;
}

bool RandomState_Flip(RandomState *r, float trueProb)
{
    //XXX: Naive implementation.

    if (trueProb <= 0.0f) {
        return FALSE;
    }

    /*
     * I could assert that it's <= 1, but I worry about people computing
     * probabilities having things round slightly above 1.
     */
    if (trueProb >= 1.0f) {
        return TRUE;
    }

    if (RandomState_UnitFloat(r) <= trueProb) {
        return TRUE;
    } else {
        return FALSE;
    }
}

float RandomState_Float(RandomState *r, float min, float max)
{
    double rval;
    double range;
    double dmin;
    double dmax;

    ASSERT(max >= min);

    if (min == max) {
        return min;
    }

    // Uniform randomData from [0, 1).
    rval = RandomState_UnitFloat(r);

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


int RandomState_Enum(RandomState *r,
                     EnumDistribution *dist, int numValues)
{
    float cumulative;
    float rval;
    int x;
    ASSERT(numValues > 0);

    rval = RandomState_UnitFloat(r);

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

uint64 RandomState_Uint64(RandomState *r)
{
    uint64 a;
    uint32 b;

    a = RandomState_Uint32(r);
    b = RandomState_Uint32(r);

    return (a << 32) | b;
}

/*
 * Random_Int --
 *   Returns a uniformly distributed int in the range [min, max] (inclusive).
 *   XXX: Not verified for negative values or ranges near max int.
 */
int RandomState_Int(RandomState *r, int min, int max)
{
    int range;
    int rval;

    ASSERT(max >= min);

    range = (max - min) + 1;
    rval = RandomState_Uint32(r);
    rval &= ~(1 << 31);

    ASSERT(rval >= 0);
    rval = (rval % range) + min;

    ASSERT(rval <= max);
    ASSERT(rval >= min);

    return rval;
}

/*
 * Random_UnitFloat --
 *     Returns a uniformly distributed float in the range [0, 1).
 */
float RandomState_UnitFloat(RandomState *r)
{
    float rval;

    rval = ((float) RandomState_Uint32(r)) / (float) ((uint32) -1);

    return rval;
}

/*
 * Sum numDice random die, between 1 and diceMax, inclusive.
 */
int RandomState_DiceSum(RandomState *r, int numDice, int diceMax)
{
    int x;
    int oup = 0;

    ASSERT(numDice >= 0 );

    for (x = 0; x < numDice; x++) {
        oup += RandomState_Int(r, 1, diceMax);
    }

    return oup;
}

/*
 * Initializes the random module.
 * This will generate a random seed if one has not
 * been previously set.
 */
void Random_Init(void)
{
    randomData.initializedCount++;

    if (randomData.initializedCount > 1) {
        return;
    }

    if (!randomData.haveSeed) {
        Random_GenerateSeed();
        ASSERT(randomData.haveSeed);
    }
}

void Random_Exit(void)
{
    ASSERT(randomData.initializedCount > 0);
    randomData.initializedCount--;

    if (randomData.initializedCount > 0) {
        return;
    }

    randomData.haveSeed = FALSE;
}


/*
 * Generate a new random seed for the module.
 */
void Random_GenerateSeed()
{
    uint64 seed;

    bool haveSeed = FALSE;

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
        Random_SetSeed(seed);

        Random_Uint64();
        seed = Random_Uint64();
        haveSeed = TRUE;
    }

    ASSERT(haveSeed);
    Random_SetSeed(seed);
}

/*
 * Returns the seed being used for this run.
 * (At least since the last time it was set...)
 */
uint64 Random_GetSeed()
{
    ASSERT(randomData.haveSeed);
    return RandomState_GetSeed(&randomData.rs);
}

/*
 * May be called before or after initialization.
 * Calling this before initialization prevents init from
 * attempting to generate a seed.
 */
void Random_SetSeed(uint64 seed)
{
    RandomState_SetSeed(&randomData.rs, seed);
    randomData.haveSeed = TRUE;
}


uint32 Random_Uint32(void)
{
    ASSERT(randomData.initializedCount > 0);
    ASSERT(randomData.haveSeed);
    return RandomState_Uint32(&randomData.rs);
}

bool Random_Bit(void)
{
    return RandomState_Bit(&randomData.rs);
}

bool Random_Flip(float trueProb)
{
    return RandomState_Flip(&randomData.rs, trueProb);
}

float Random_Float(float min, float max)
{
    ASSERT(randomData.initializedCount > 0);
    return RandomState_Float(&randomData.rs, min, max);
}


int Random_Enum(EnumDistribution *dist, int numValues)
{
    return RandomState_Enum(&randomData.rs, dist, numValues);
}

uint64 Random_Uint64(void)
{
    return RandomState_Uint64(&randomData.rs);
}

/*
 * Random_Int --
 *   Returns a uniformly distributed int in the range [min, max] (inclusive).
 *   XXX: Not verified for negative values or ranges near max int.
 */
int Random_Int(int min, int max)
{
    return RandomState_Int(&randomData.rs, min, max);
}

/*
 * Random_UnitFloat --
 *     Returns a uniformly distributed float in the range [0, 1).
 */
float Random_UnitFloat(void)
{
    return RandomState_UnitFloat(&randomData.rs);
}

/*
 * Sum numDice random die, between 1 and diceMax, inclusive.
 */
int Random_DiceSum(int numDice, int diceMax)
{
    return RandomState_DiceSum(&randomData.rs, numDice, diceMax);
}
