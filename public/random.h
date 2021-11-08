/*
 * random.h -- part of MBLib
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

#ifndef _RANDOM_H_201103252317
#define _RANDOM_H_201103252317

#ifdef __cplusplus
    extern "C" {
#endif

#include "MBTypes.h"

typedef struct RandomState {
    uint64 value;
    uint64 seed;
    uint32 bitBucket;
    int bitBucketSize;
} RandomState;

typedef struct EnumDistribution {
    int value;
    float probability;
} EnumDistribution;

void RandomState_Create(RandomState *r);
void RandomState_CreateWithSeed(RandomState *r, uint64 seed);
void RandomState_Destroy(RandomState *r);
void RandomState_GenerateSeed(RandomState *r);
uint64 RandomState_GetSeed(RandomState *r);
void RandomState_SetSeed(RandomState *r, uint64 seed);
bool RandomState_Flip(RandomState *r, float trueProb);
bool RandomState_Bit(RandomState *r);
int RandomState_Int(RandomState *r, int min, int max);
float RandomState_Float(RandomState *r, float min, float max);
uint32 RandomState_Uint32(RandomState *r);
uint64 RandomState_Uint64(RandomState *r);
float RandomState_UnitFloat(RandomState *r);
int RandomState_Enum(RandomState *r, EnumDistribution *dst,
                     int numValues);
int RandomState_DiceSum(RandomState *r, int numDice, int diceMax);

void Random_Init(void);
void Random_Exit(void);

void Random_GenerateSeed();

uint64 Random_GetSeed();
void Random_SetSeed(uint64 seed);

/*
 * Return TRUE with probability trueProb.
 */
bool Random_Flip(float trueProb);

/*
 * Return TRUE with probability 1/2.
 */
bool Random_Bit();

/*
 * Uniformly distributed random number in the given range.
 */
int Random_Int(int min, int max);
float Random_Float(float min, float max);

uint32 Random_Uint32(void);
uint64 Random_Uint64(void);
float Random_UnitFloat(void);

int Random_Enum(EnumDistribution *dist, int numValues);

/*
 * Sum numDice random die, between 1 and diceMax, inclusive.
 */
int Random_DiceSum(int numDice, int diceMax);

#ifdef __cplusplus
	}
#endif

#endif //_RANDOM_H_201103252317
