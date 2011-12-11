/*
 * random.h --
 */


#ifndef _RANDOM_H_201103252317
#define _RANDOM_H_201103252317

#ifdef __cplusplus
	extern "C" {
#endif 

#include "mbtypes.h"

typedef struct EnumDistribution {
	int value;
	float probability;
} EnumDistribution;

uint64 Random_Init(void);
void Random_Exit(void);

void Random_Seed(uint64 seed);

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
