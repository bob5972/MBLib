/*
 * This file is part of MBLib.
 * Copyright (c) 2015 Michael Banack <bob5972@banack.net>
 *
 * MBLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MBLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MBLib.  If not, see <http://www.gnu.org/licenses/>.
 */

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
