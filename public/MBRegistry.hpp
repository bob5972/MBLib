/*
 * MBRegistry.hpp -- part of MBLib
 *
 * Copyright (c) 2020 Michael Banack <github@banack.net>
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

#ifndef MBRegistry_HPP_202006121304
#define MBRegistry_HPP_202006121304

extern "C" {
#define MBRegistry CMBRegistry
#include "MBRegistry.h"
#undef MBRegistry
}

class MBRegistry {
    public:
        MBRegistry()
        {
            myReg = MBRegistry_Alloc();
        }

        ~MBRegistry()
        {
            MBRegistry_Free(myReg);
            myReg = NULL;
        }

        const char *get(const char *key)
        {
            return MBRegistry_Get(myReg, key);
        }

        const char *remove(const char *key)
        {
            return MBRegistry_Remove(myReg, key);
        }

        void put(const char *key, const char *value)
        {
            MBRegistry_Put(myReg, key, value);
        }

    private:
        CMBRegistry *myReg;
};

#endif // MBRegistry_HPP_202006121304
