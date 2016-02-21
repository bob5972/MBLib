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

#ifndef BitVector_HPP_201202062339
#define BitVector_HPP_201202062339

#include "mbtypes.h"
#include "mbutil.h"
#include "mbassert.h"
#include <string.h>

extern "C" {
#define BitVector CBitVector
#include "BitVector.h"
#undef BitVector
}

class BitVector
{
    public:
        BitVector() {
            BitVector_Create(&b);
        }

        BitVector(const BitVector &a) {
            BitVector_Create(&b);
            BitVector_Copy(&b, &a.b);
        }

        explicit BitVector(int size) {
            BitVector_CreateWithSize(&b, size);
        }

        BitVector(int size, bool initial) {
            BitVector_Create(&b);
            BitVector_SetFillValue(&b, initial);
            BitVector_Resize(&b, size);
        }

        ~BitVector() {
            BitVector_Destroy(&b);
        }

        bool getFillValue() const {
            return BitVector_GetFillValue(&b);
        }

        void setFillValue(bool f) {
            BitVector_SetFillValue(&b, f);
        }

        bool get(int x) const {
            return BitVector_Get(&b, x);
        }

        void put(int x, bool v) {
            BitVector_Put(&b, x, v);
        }

        void set(int x) {
            BitVector_Set(&b, x);
        }

        void reset(int x) {
            BitVector_Reset(&b, x);
        }

        void flip(int x) {
            BitVector_Flip(&b, x);
        }

        bool testAndSet(int x) {
            return BitVector_TestAndSet(&b, x);
        }

        void setRange(int first, int last) {
            BitVector_SetRange(&b, first, last);
        }

        void resetRange(int first, int last) {
            BitVector_ResetRange(&b, first, last);
        }

        void flipRange(int first, int last) {
            BitVector_FlipRange(&b, first, last);
        }

        void setAll() {
            BitVector_SetAll(&b);
        }

        void resetAll() {
            BitVector_ResetAll(&b);
        }

        void flipAll() {
            BitVector_FlipAll(&b);
        }

        int size() const {
            return BitVector_Size(&b);
        }

        void resize(int size) {
            BitVector_Resize(&b, size);
        }

        void makeEmpty() {
            BitVector_MakeEmpty(&b);
        }

        bool operator [](int x) const {
            return BitVector_Get(&b, x);
        }

        void consume(BitVector &src) {
            BitVector_Consume(&b, &src.b);
        }

        int popcount() const {
            return BitVector_PopCount(&b);
        }

    private:
        CBitVector b;
};

#endif // BitVector_HPP_201202062339
