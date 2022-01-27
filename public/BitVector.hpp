/*
 * BitVector.hpp -- part of MBLib
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

#ifndef BitVector_HPP_201202062339
#define BitVector_HPP_201202062339

#include "MBTypes.h"
#include "MBUtil.h"
#include "MBAssert.h"
#include <string.h>

#include "BitVector.h"

class CPBitVector
{
    public:
        CPBitVector() {
            BitVector_Create(&b);
        }

        CPBitVector(const CPBitVector &a) {
            BitVector_Create(&b);
            BitVector_Copy(&b, &a.b);
        }

        explicit CPBitVector(int size) {
            BitVector_CreateWithSize(&b, size);
        }

        CPBitVector(int size, bool initial) {
            BitVector_Create(&b);
            BitVector_SetFillValue(&b, initial);
            BitVector_Resize(&b, size);
        }

        ~CPBitVector() {
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

        const CPBitVector &operator =(const CPBitVector & rhs) {
            BitVector_Copy(&b, &rhs.b);
            return *this;
        }

        void consume(CPBitVector &src) {
            BitVector_Consume(&b, &src.b);
        }

        int popcount() const {
            return BitVector_PopCount(&b);
        }

    private:
        CBitVector b;
};

#endif // BitVector_HPP_201202062339
