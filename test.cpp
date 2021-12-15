/*
 * test.cpp -- part of MBLib
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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "MBConfig.h"

#ifdef MB_HAS_SDL2
#include <SDL2/SDL.h>
#include "MBLock.h"
#endif

#include "MBRegistry.h"

#include "MBString.hpp"
#include "MBStack.hpp"
#include "MBAssert.h"
#include "MBVector.hpp"
#include "MBSet.hpp"
#include "BitVector.hpp"
#include "MBUtil.h"
#include "Random.h"
#include "MBMap.hpp"
#include "MBQueue.hpp"
#include "IntMap.hpp"
#include "MBRing.h"

static int seed;

#define TEST(x) \
do { \
    int cond = (x); \
    if (UNLIKELY(!cond)) { \
        PANIC("Test failure: %s\n", #x); \
    } \
} while (FALSE)

void testTypes()
{
    typedef struct TypeSize
    {
            const char *name;
            int expectedSize;
            int actualSize;
    } TypeSize;

    TypeSize types[] = { { "uint8", 1, sizeof(uint8) }, { "int8", 1,
            sizeof(int8) }, { "uint16", 2, sizeof(uint16) }, { "int16", 2,
            sizeof(int16) }, { "uint32", 4, sizeof(uint32) }, { "int32", 4,
            sizeof(int32) }, { "uint64", 8, sizeof(uint64) }, { "int64", 8,
            sizeof(int64) }, { "bool", 1, sizeof(bool) }, };

    uint32 x;
    for (x = 0; x < ARRAYSIZE(types); x++) {
        if (types[x].actualSize != types[x].expectedSize) {
            fprintf(stderr, "Bad %s size: expected %d, actual %d\n",
                    types[x].name, types[x].expectedSize, types[x].actualSize);
            TEST(FALSE);
        }
    }
}

void testMBString(void)
{
    int result;
    MBString str(30, 'd');
    MBString str2(str);
    MBString cstr("Hello World!");

    cstr = "Hello World!";
    result = cstr.length();
    TEST(result == 12);
    result = cstr.getChar(0);
    TEST(result == 'H');
    result = cstr.getChar(1);
    TEST(result == 'e');
    result = cstr.getChar(6);
    TEST(result == 'W');
    result = cstr.getChar(10);
    TEST(result == 'd');
    result = cstr.getChar(11);
    TEST(result == '!');

    cstr = "Hello World!";
    result = (cstr < "Hello Q");
    TEST(!result);
    result = (cstr > "Hello Q");
    TEST(result);
    result = (cstr != "ABC");
    TEST(result);
    result = (cstr != "Hello Wozld!");
    TEST(result);
    result = (cstr == "Hello World!");
    TEST(result);

    str = "Goodbye World!";
    str2 = str.substr(0, 1);
    result = str2.length();
    TEST(result == 1);
    result = str2.getChar(0);
    TEST(result == 'G');

    str2 = str.substr(1, 3);
    result = str2.compare("ood");
    TEST(result == 0);
    str2 = str.substr(4, 4);
    result = str2.compare("bye ");
    TEST(result == 0);

    str = "Hello World!";

    str2 = str;

    str2 = "Hello World!";
    result = str2.length();
    TEST(result == 12);
    result = str2.getChar(0);
    TEST(result == 'H');
    result = str2.getChar(1);
    TEST(result == 'e');
    result = str2.getChar(6);
    TEST(result == 'W');
    result = str2.getChar(10);
    TEST(result == 'd');
    result = str2.getChar(11);
    TEST(result == '!');

    str = "Hello World!";
    result = str.find('H');
    TEST(result == 0);
    result = str.find('W');
    TEST(result == 6);
    result = str.find("W");
    TEST(result == 6);

    result = str.find("Hello World!");
    TEST(result == 0);

    result = str.find(str);
    TEST(result == 0);

    result = str.find("rld");
    TEST(result == 8);

    result = str.find("Hello");
    TEST(result == 0);

    result = str.find("ld!");
    TEST(result == 9);

    result = str.find("punk");
    TEST(result == -1);

    result = str.find("elxxW");
    TEST(result == -1);

    result = str.find("elW");
    TEST(result == -1);

    str = "This is a particularly long test string.";
    str2 = "";
    for (int x = 0; x < str.length(); x++) {
        str2 += str.getChar(x);
    }
    result = (str == str2);
    TEST(result);

    str2 = "";
    str2 += str;
    result = (str == str2);
    TEST(result);
}

void testMBStack(void)
{
    int count = 1200;
    int result;
    MBStack<int> s;
    MBStack<int> r;

    for (int y = 0; y < 3; y++) {
        s.makeEmpty();
        for (int x = 0; x <= count; x++) {
            s.push(x + seed);
        }
        r = s;
        r.trim();
    }

    for (int x = count; x >= 0; x--) {
        result = s.pop();
        TEST(result == x + seed);
        result = r.pop();
        TEST(result == x + seed);
    }
}

void testMBVector(void)
{
    int count = 100;
    int result;
    MBVector<int> s;
    MBVector<int> r;

    for (int y = 0; y < 2; y++) {
        s.makeEmpty();
        for (int x = 0; x <= count; x++) {
            s.push(x + seed);
            r.push(s[x]);
            result = r.pop();
            TEST(result == x + seed);
        }
        r = s;
    }

    for (int x = count; x >= 0; x--) {
        result = s[x];
        TEST(result == x + seed);
        result = s.size();
        TEST(result == x + 1);
        result = s.pop();
        TEST(result == x + seed);

        result = r[x];
        TEST(result == x + seed);
        result = r.size();
        TEST(result == x + 1);
        result = r.pop();
        TEST(result == x + seed);
    }

    s.resize(count + 1);
    r.resize(count + 1);
    for (int x = 0; x <= count; x++) {
        s[x] = x + seed;
        r[x] = s[x] + seed;
    }
    for (int x = 0; x <= count; x++) {
        result = s[x];
        TEST(result == x + seed);
        result = r[x];
        TEST(result == x + (2 * seed));
    }
}

void testCMBVector(void)
{
    int result;
    CMBIntVec s;
    CMBIntVec r;
    int sizes[] = {
        0, 1, 10, 100, 200, 300,
    };
    int *value;
    int count = 100;

    CMBIntVec_CreateEmpty(&s);
    CMBIntVec_CreateEmpty(&r);

    for (unsigned y = 0; y < ARRAYSIZE(sizes); y++) {
        CMBIntVec_MakeEmpty(&s);
        result = CMBIntVec_Size(&s);
        TEST(result == 0);

        for (unsigned x = y; x < ARRAYSIZE(sizes); x++) {
            CMBIntVec_Resize(&s, sizes[y]);
            result = CMBIntVec_Size(&s);
            TEST(result == sizes[y]);
            CMBIntVec_Resize(&s, sizes[x]);
            result = CMBIntVec_Size(&s);
            TEST(result == sizes[x]);
            CMBIntVec_Resize(&s, sizes[y]);
            result = CMBIntVec_Size(&s);
            TEST(result == sizes[y]);

            CMBIntVec_MakeEmpty(&s);
            result = CMBIntVec_Size(&s);
            TEST(result == 0);
            result = CMBIntVec_IsEmpty(&s);
            TEST(result);
        }
    }

    CMBIntVec_Resize(&s, count);
    result = CMBIntVec_Size(&s);
    TEST(result == count);
    result = CMBIntVec_IsEmpty(&s);
    TEST(!result);

    for (int x = 0; x < CMBIntVec_Size(&s); x++) {
        value = CMBIntVec_GetPtr(&s, x);
        *value = x + seed;
    }
    for (int x = 0; x < CMBIntVec_Size(&s); x++) {
        value = CMBIntVec_GetPtr(&s, x);
        TEST(*value == x + seed);
    }

    CMBIntVec_GrowBy(&s, count);
    result = CMBIntVec_Size(&s);
    TEST(result == 2 * count);
    for (int x = 0; x < CMBIntVec_Size(&s); x++) {
        value = CMBIntVec_GetPtr(&s, x);
        if (x < count) {
            TEST(*value == x + seed);
        }
        *value = x + seed;
    }
    for (int x = 0; x < CMBIntVec_Size(&s); x++) {
        value = CMBIntVec_GetPtr(&s, x);
        TEST(*value == x + seed);
    }

    CMBIntVec_ShrinkBy(&s, count);
    result = CMBIntVec_Size(&s);
    TEST(result == count);
    for (int x = 0; x < CMBIntVec_Size(&s); x++) {
        value = CMBIntVec_GetPtr(&s, x);
        TEST(*value == x + seed);
    }

    CMBIntVec_Copy(&r, &s);
    result = CMBIntVec_Size(&r);
    TEST(result == count);
    for (int x = 0; x < CMBIntVec_Size(&r); x++) {
        value = CMBIntVec_GetPtr(&r, x);
        TEST(*value == x + seed);
        TEST(*value == CMBIntVec_GetValue(&r, x));
    }

    CMBIntVec_Destroy(&s);
    CMBIntVec_Destroy(&r);
}

void testMBSet(void)
{
    int count = 1000;
    int result;

    MBSet<int> s;

    for (int x = 0; x <= count; x++) {
        s.add(x + seed);
        TEST(s.size() == x + 1);
    }

    for (int x = 0; x <= count; x++) {
        result = s.contains(x + seed);
        TEST(result);
    }

    for (int x = count + 1; x <= count * 2; x++) {
        result = s.contains(x + seed);
        TEST(!result);
    }
}

void testBitVector(void)
{
    int count;
    int result;
    BitVector b;

    // Test basic get/set
    {
        uint64 rawbit[50];
        int rawSize;

        rawSize = (int) ARRAYSIZE(rawbit) * sizeof(rawbit[0]) * 8;
        for (int x = 0; x < rawSize; x++) {
            BitVector_SetRaw(x, rawbit);
            result = BitVector_GetRaw(x, rawbit);
            TEST(result);

            BitVector_ResetRaw(x, rawbit);
            result = BitVector_GetRaw(x, rawbit);
            TEST(!result);

            BitVector_FlipRaw(x, rawbit);
            result = BitVector_GetRaw(x, rawbit);
            TEST(result);

            BitVector_FlipRaw(x, rawbit);
            result = BitVector_GetRaw(x, rawbit);
            TEST(!result);
        }
    }

    //Test static ranges
    {
        count = 2048;
        b.resize(count);

        b.setAll();
        b.flipRange(123, 518);
        for (int x = 0; x < count; x++) {
            result = b.get(x);
            if (x >= 123 && x <= 518) {
                TEST(!result);
            } else {
                TEST(result);
            }
        }

        b.setAll();
        b.resetRange(111, 717);
        for (int x = 0; x < count; x++) {
            result = b.get(x);
            if (x >= 111 && x <= 717) {
                TEST(!result);
            } else {
                TEST(result);
            }
        }

        b.resetAll();
        b.setRange(321, 711);
        for (int x = 0; x < count; x++) {
            result = b.get(x);
            if (x >= 321 && x <= 711) {
                TEST(result);
            } else {
                TEST(!result);
            }
        }
    }

    // Test resize
    {
        count = 10000;
        b.resize(count + 1);
        for (int x = 0; x <= count; x++) {
            b.set(x);
            result = b.get(x);
            TEST(result);
        }

        b.resize((count + 1) * 2);
        for (int x = count + 1; x <= count * 2; x++) {
            b.reset(x);
            result = b.get(x);
            TEST(!result);
        }

        for (int x = 0; x <= count; x++) {
            result = b.get(x);
            TEST(result);
        }

        for (int x = count + 1; x <= count * 2; x++) {
            result = b.get(x);
            TEST(!result);
        }
    }

    // test (re)setAll
    {
        int sizeArray[] = { 1, 257, 512, 1015, 1016, 2048, 2049, 10001, };
        for (int y = 1; y < (int) ARRAYSIZE(sizeArray); y++) {
            count = sizeArray[y];
            b.resize(count);

            b.setAll();
            for (int x = 0; x < count; x++) {
                result = b.get(x);
                TEST(result);
            }

            b.resetAll();
            for (int x = 0; x < count; x++) {
                result = b.get(x);
                TEST(!result);
            }

            b.flipAll();
            for (int x = 0; x < count; x++) {
                result = b.get(x);
                TEST(result);
            }
        }
    }

    //Test setRange
    {
        count = 10000;
        b.resize((count + 1) * 2);
        b.resetAll();
        for (int x = 0; x <= count * 2; x++) {
            result = b.get(x);
            TEST(!result);
        }

        b.setRange(1, count / 2);
        for (int x = 1; x <= count / 2; x++) {
            result = b.get(x);
            TEST(result);
        }
        for (int x = count / 2 + 1; x <= count; x++) {
            result = b.get(x);
            TEST(!result);
        }
    }

    //Test flip
    {
        int sizeArray[] = { 257, 512, 1015, 1016, 2048, 2049, 10001, 20002, };
        for (int y = 1; y < (int) ARRAYSIZE(sizeArray); y++) {
            count = sizeArray[y];
            b.resize(count);

            for (int x = 0; x < count; x++) {
                b.put(x, (x % 2) ? TRUE : FALSE);
            }
            for (int x = 0; x < count; x++) {
                result = b.get(x);
                TEST(result == (x % 2) ? TRUE : FALSE);
                b.flip(x);
                result = b.get(x);
                TEST(result == (x % 2) ? FALSE : TRUE);
            }
        }
    }

    // Test PopCount
    {
        count = 2049;
        b.resetAll();
        b.resize(count);
        for (int x = 0; x < 29; x++) {
            b.set(x);
        }

        result = b.popcount();
        TEST(result == 29);

        int array[] = { 33, 50, 51, 67, 101, 517, 1023, 2048 };
        for (int x = 0; x < (int) ARRAYSIZE(array); x++) {
            b.set(array[x]);
        }

        result = b.popcount();
        TEST(result == ARRAYSIZE(array) + 29);

        b.setAll();
        result = b.popcount();
        TEST(result == count);

        b.resetAll();
        result = b.popcount();
        TEST(result == 0);
    }

    // Test FlipAll
    {
        count = 1027;

        b.resize(count);
        b.resetAll();
        b.flipAll(); // all set

        for (int x = 0; x < count; x++) {
            result = b.get(x);
            TEST(result);
        }

        b.flipAll(); // all reset
        for (int x = 0; x < count; x++) {
            result = b.get(x);
            TEST(!result);
        }

        int array[] = { 0, 7, 67, 131, 1024, };

        for (int x = 0; x < (int) ARRAYSIZE(array); x++) {
            b.set(x);
        }

        b.flipAll(); // all set (except stragglers)

        for (int x = 0; x < (int) ARRAYSIZE(array); x++) {
            result = b.get(x);
            TEST(!result);
            b.set(x);
        }

        for (int x = 0; x < count; x++) {
            result = b.get(x);
            TEST(result);
        }
    }

    // Test FlipRange
    {
        int start;
        int end;

        count = 1027;

        b.resize(count);
        b.resetAll();

        start = 3;
        end = 515;
        b.flipRange(start, end);

        for (int x = 0; x < count; x++) {
            result = b.get(x);

            if (x < start) {
                TEST(!result);
            } else if (x >= start && x <= end) {
                TEST(result);
            } else {
                TEST(!result);
            }
        }

        b.flipRange(start, end);
        for (int x = 0; x < count; x++) {
            result = b.get(x);
            TEST(!result);
        }

        start = 3;
        end = 7;
        b.flipRange(start, end);
        for (int x = 0; x < count; x++) {
            result = b.get(x);

            if (x < start) {
                TEST(result == FALSE);
            } else if (x >= start && x <= end) {
                TEST(result == TRUE);
            } else {
                TEST(result == FALSE);
            }
        }
    }

}

void testMBMap()
{
    bool runStringTests = TRUE;
    bool runIntTests = TRUE;
    int result;
    int num;
    const int count = 1000;

    if (runStringTests) {
        MBMap<MBString, int> map;

        for (int x = 0; x < 25; x++) {
            MBString key = MBString::toString(x + seed);
            map[key] = x + seed;
        }

        for (int x = 0; x < 25; x++) {
            MBString key = MBString::toString(x + seed);
            TEST(map[key] == x + seed);
        }

        for (int x = 0; x < 25; x++) {
            MBString key = MBString::toString(x + seed);
            int value = x + 10;
            map[key] = value;
        }

        for (int x = 0; x < 25; x++) {
            MBString key = MBString::toString(x + seed);
            int value = x + 10;
            TEST(map[key] == value);
        }
    }

    if (runIntTests) {
        MBMap<int, int> m;

        for (int x = 0; x < count; x++) {
            m.put(x, x + seed);
            result = m.get(x);
            TEST(x + seed == result);
            m[x] += 1;
            result = m.get(x);
            TEST(result == x + seed + 1);
            m[x] -= 1;
            result = m.get(x);
            TEST(result == x + seed);

            result = m.size();
            TEST(result == x + 1);
        }

        result = m.size();
        TEST(result == count);

        for (int x = 0; x < count; x++) {
            result = m.containsKey(x);
            TEST(result);
            result = m.get(x);
            TEST(x + seed == result);
        }

        for (int x = count; x < 2 * count; x++) {
            result = m.containsKey(x);
            TEST(!result);
        }

        m.makeEmpty();
        for (int x = 0; x < count; x++) {
            num = x * x + seed;
            m.put(num, x);
            result = m.get(num);
            TEST(x == result);

            result = m.size();
            TEST(result == x + 1);
        }

        for (int x = 0; x < count; x++) {
            num = x * x * x + seed;
            m.put(num, x);
            result = m.get(num);
            TEST(x == result);
        }

    }
}

void testIntMap()
{
    IntMap m;
    int result;
    int num;
    const int count = 1000;

    TEST(m.get(0) == 0);
    m.put(0, 0);
    TEST(m.get(0) == 0);
    m.increment(0);
    TEST(m.get(0) == 1);
    m.decrement(0);
    TEST(m.get(0) == 0);
    m.makeEmpty();

    TEST(m.get(0) == 0);
    m.increment(0);
    TEST(m.get(0) == 1);
    m.makeEmpty();

    TEST(m.get(1) == 0);
    m.put(1, 1);
    TEST(m.get(1) == 1);
    m.increment(1);
    TEST(m.get(1) == 2);
    m.decrement(1);
    TEST(m.get(1) == 1);
    m.makeEmpty();

    TEST(m.get(1) == 0);
    m.increment(1);
    TEST(m.get(1) == 1);
    m.makeEmpty();

    for (int x = 0; x < count; x++) {
        m.put(x, x + seed);
        result = m.get(x);
        TEST(x + seed == result);
        result = m.increment(x);
        TEST(result == x + seed + 1);
        result = m.get(x);
        TEST(result == x + seed + 1);
        m.decrement(x);
        result = m.get(x);
        TEST(result == x + seed);

        result = m.size();
        TEST(result == x + 1);
    }

    result = m.size();
    TEST(result == count);

    result = m.get(count + 1);
    TEST(result == 0);

    for (int x = 0; x < count; x++) {
        result = m.containsKey(x);
        TEST(result);
        result = m.get(x);
        TEST(x + seed == result);
    }

    for (int x = count; x < 2 * count; x++) {
        result = m.containsKey(x);
        TEST(!result);
    }

    m.makeEmpty();
    for (int x = 0; x < count; x++) {
        num = x * x;
        m.put(num, x);
        result = m.get(num);
        TEST(x == result);

        result = m.size();
        TEST(result == x + 1);
    }

    for (int x = 0; x < count; x++) {
        num = x * x * x;
        m.put(num, x);
        result = m.get(num);
        TEST(x == result);
    }

    {
        /*
         * Test a problematic key sequence.
         */
        int ks[] = {
            47, 42, 12074, 10, 32, 2592, 791284256, 42, 32, 10784, 84,
            104, 21608, 706761832, 105, 115, 26995, 32, 102,
        };
        IntMap p;

        for (uint32 i = 0; i < ARRAYSIZE(ks); i++) {
            int v = p.get(ks[i]);
            p.increment(ks[i], 1);
            TEST(v + 1 == p.get(ks[i]));
        }

        for (uint32 i = 0; i < ARRAYSIZE(ks); i++) {
            if (ks[i] == 42) {
                TEST(p.get(ks[i]) == 2);
            } else if (ks[i] == 32) {
                TEST(p.get(ks[i]) == 3);
            } else {
                TEST(p.get(ks[i]) == 1);
            }
        }
    }

    {
        /*
         * Test EmptyValue
         */
        CMBIntMap map;
        CMBIntMap_Create(&map);
        CMBIntMap_SetEmptyValue(&map, -1);
        TEST(CMBIntMap_Get(&map, 0) == -1);
        CMBIntMap_Destroy(&map);
    }

    {
        /*
         * Test Put/Remove
         */
        CMBIntMap map;
        CMBIntMap_Create(&map);
        CMBIntMap_SetEmptyValue(&map, -1);

        CMBIntMap_Put(&map,   2, 1);
        CMBIntMap_Put(&map,   7, 1);
        CMBIntMap_Put(&map,   8, 1);
        CMBIntMap_Put(&map,  10, 1);
        CMBIntMap_Remove(&map, 8);

        CMBIntMap_Put(&map,  10, 2);
        CMBIntMap_Remove(&map, 10);
        CMBIntMap_Put(&map,  12, 1);
        CMBIntMap_Remove(&map, 12);
        CMBIntMap_Put(&map,  18, 1);
        CMBIntMap_Remove(&map, 18);
        CMBIntMap_Put(&map,  22, 1);
        CMBIntMap_Remove(&map, 22);
        CMBIntMap_Put(&map,  26, 1);
        CMBIntMap_Remove(&map, 26);
        CMBIntMap_Put(&map,  29, 1);
        CMBIntMap_Remove(&map, 29);
        CMBIntMap_Put(&map,  31, 1);
        CMBIntMap_Remove(&map, 31);
        CMBIntMap_Put(&map,  33, 1);
        CMBIntMap_Remove(&map, 33);
        CMBIntMap_Put(&map,  36, 1);
        CMBIntMap_Remove(&map, 36);
        CMBIntMap_Put(&map,  38, 1);
        CMBIntMap_Remove(&map, 38);
        CMBIntMap_Put(&map,  41, 1);
        CMBIntMap_Remove(&map, 41);
        CMBIntMap_Put(&map,  43, 1);
        CMBIntMap_Remove(&map, 43);

        CMBIntMap_Put(&map,  45, 1);

        CMBIntMap_Destroy(&map);
    }
}

void testRandomIntMap()
{
    IntMap m;
    int result;
    uint32 num;

    /*
     * Making the hash tables get too big in a single run
     * causes it to spend a bunch of it's time in system calls
     * (presumably calling malloc?).
     */
    const int count = 10 * 1000;

    for (int x = 0; x < count; x++) {
        num = Random_Uint32();
        result = m.get(num);

        m.put(num, x);
        result = m.get(num);
        TEST(x == result);

        if (Random_Bit()) {
            m.remove(num);
            TEST(!m.containsKey(num));
        }
    }
}

void testRandom(void)
{
    int x, y;
    int count = 100;

    uint64 num;
    for (x = 0; x < count; x++) {
        num = Random_Uint64();
    }

    for (x = 0; x < count; x++) {
        num = Random_Uint32();
    }

    for (x = 0; x < count; x++) {
        num = 0;
        for (y = 0; y < 32; y++) {
            num <<= 1;
            if (Random_Bit()) {
                num |= 1;
            }
        }
    }

    for (x = 0; x < count; x++) {
        double d;
        d = Random_UnitFloat();
        TEST(d >= 0.0 && d <= 1.0);
    }
}

void testMBQueue(void)
{
    MBQueue<int> q;
    const int count = 1000;
    int result;

    for (int x = 0; x < count; x++) {
        q.enqueue(x);

        result = q.size();
        TEST(result == x + 1);
    }

    for (int x = 0; x < count; x++) {
        result = q.dequeue();
        TEST(result == x);

        result = q.size();
        TEST(result == count - (x + 1));
    }
}


void testMBRegistry(void)
{
    MBRegistry *mreg;
    const char *s;

    mreg = MBRegistry_Alloc();
    MBRegistry_Free(mreg);

    mreg = MBRegistry_Alloc();
    MBRegistry_PutConst(mreg, "key", "value");
    TEST(strcmp(MBRegistry_Get(mreg, "key"), "value") == 0);
    s = "OtherValue";
    MBRegistry_PutConst(mreg, "OtherKey", s);
    TEST(MBRegistry_Get(mreg, "OtherKey") == s);
    MBRegistry_PutConst(mreg, "key", s);
    TEST(MBRegistry_Get(mreg, "key") == s);
    MBRegistry_Remove(mreg, "key");
    MBRegistry_Remove(mreg, "OtherKey");
    TEST(MBRegistry_Get(mreg, "key") == NULL);
    TEST(MBRegistry_Get(mreg, "OtherKey") == NULL);
    MBRegistry_Free(mreg);
}

int testCompareUint32(const void *lhs, const void *rhs, void *cbData)
{
    uint32 *lhsT = (uint32*)lhs;
    uint32 *rhsT = (uint32*)rhs;

    ASSERT(cbData == NULL);

    if (*lhsT < *rhsT) {
        return -1;
    } else if (*lhsT == *rhsT) {
        return 0;
    } else {
        return 1;
    }
}

void testMBCompare(void)
{
    uint32 array[10];

    for (uint32 i = 0; i < ARRAYSIZE(array); i++) {
        array[i] = 100 - i;
    }

    MBCompare_SortFallback(array, ARRAYSIZE(array), sizeof(array[0]),
                           testCompareUint32, NULL);

    for (uint32 i = 1; i < ARRAYSIZE(array); i++) {
        TEST(array[i-1] <= array[i]);
    }

    for (uint32 i = 0; i < ARRAYSIZE(array); i++) {
        array[i] = 100 - i;
    }

    MBCompare_Sort(array, ARRAYSIZE(array), sizeof(array[0]),
                   testCompareUint32, NULL);

    for (uint32 i = 1; i < ARRAYSIZE(array); i++) {
        TEST(array[i-1] <= array[i]);
    }
}

void testMBLock(void)
{
#ifdef MB_HAS_SDL2
    MBLock lock;

    MBLock_Create(&lock);

    if (mb_debug) {
        TEST(!MBLock_IsLocked(&lock));
    }
    MBLock_Lock(&lock);

    if (mb_debug) {
        TEST(MBLock_IsLocked(&lock));
    }

    MBLock_Unlock(&lock);
    MBLock_Destroy(&lock);
#endif
}

void testMBRing(void)
{
    MBRing r;
    MBRing_Create(&r, sizeof(int));
    TEST(MBRing_Size(&r) == 0);

    for (int x = 0; x < 100; x++) {
        TEST(MBRing_Size(&r) == x);
        MBRing_InsertHead(&r, &x, sizeof(x));
        TEST(MBRing_Size(&r) == x + 1);
    }
    for (int x = 0; x < 100; x++) {
        int v;
        MBRing_RemoveTail(&r, &v, sizeof(v));
        TEST(v == x);
    }
    TEST(MBRing_Size(&r) == 0);

    for (int x = 0; x < 100; x++) {
        TEST(MBRing_Size(&r) == x);
        MBRing_InsertTail(&r, &x, sizeof(x));
        TEST(MBRing_Size(&r) == x + 1);
    }
    for (int x = 0; x < 100; x++) {
        int v;
        MBRing_RemoveHead(&r, &v, sizeof(v));
        TEST(v == x);
    }
    TEST(MBRing_Size(&r) == 0);

    for (int x = 0; x < 100; x++) {
        TEST(MBRing_Size(&r) == x);
        MBRing_InsertHead(&r, &x, sizeof(x));
        TEST(MBRing_Size(&r) == x + 1);
    }
    for (int x = 99; x >= 0; x--) {
        int v;
        MBRing_RemoveHead(&r, &v, sizeof(v));
        TEST(v == x);
    }
    TEST(MBRing_Size(&r) == 0);

    for (int x = 0; x < 100; x++) {
        TEST(MBRing_Size(&r) == x);
        MBRing_InsertTail(&r, &x, sizeof(x));
        TEST(MBRing_Size(&r) == x + 1);
    }
    for (int x = 99; x >= 0; x--) {
        int v;
        MBRing_RemoveTail(&r, &v, sizeof(v));
        TEST(v == x);
    }
    TEST(MBRing_Size(&r) == 0);

    for (int x = 0; x < 100; x++) {
        MBRing_InsertHead(&r, &x, sizeof(x));
        MBRing_InsertTail(&r, &x, sizeof(x));
    }
    for (int x = 99; x >= 0; x--) {
        int v;
        MBRing_RemoveHead(&r, &v, sizeof(v));
        TEST(v == x);

        MBRing_RemoveTail(&r, &v, sizeof(v));
        TEST(v == x);
    }
    TEST(MBRing_Size(&r) == 0);

    RandomState rs;
    RandomState_CreateWithSeed(&rs, 0x1234567);
    uint count = 0;
    for (int x = 0; x < 1000; x++) {
        int v = (int)RandomState_Uint32(&rs);
        int z = RandomState_Int(&rs, 0, 3);
        if (z == 0) {
            count++;
            MBRing_InsertHead(&r, &v, sizeof(v));
        } else if (z == 1) {
            count++;
            MBRing_InsertTail(&r, &v, sizeof(v));
        } else if (z == 2) {
            if (count > 0) {
                MBRing_RemoveHead(&r, &v, sizeof(v));
                count--;
            }
        } else {
            if (count > 0) {
                MBRing_RemoveTail(&r, &v, sizeof(v));
                count--;
            }
        }

        TEST(MBRing_Size(&r) == count);
    }

    MBRing_Destroy(&r);
}

typedef struct
{
        bool enabled;
        int weight;
        void (*function)(void);
} BenchmarkTest;

int main(int argc, char *argv[])
{
    // Rough calibration on run time of the benchmark (debug off)
    const double seconds = 10.0;
    int numTests = 0;
    int calibration;
    MBString arg;
    bool benchmark;

#ifdef BENCHMARK
    benchmark = TRUE;
#else
    benchmark = FALSE;
#endif

    if (argc > 1) {
        arg = argv[1];

        if (arg == "-b") {
            benchmark = TRUE;
        } else if (arg == "-f") {
            benchmark = FALSE;
        }
    }

    /*
     * Print Configuration
     */
    printf("\n");
    printf("Starting MBLib %s ...\n", benchmark ? "Benchmark" : "Tests");
    printf("DEBUG=%d, DEVEL=%d\n", mb_debug, mb_devel);
    printf("MB_HAS_SDL2=%d\n", mb_has_sdl2);
    printf("\n");

    if (benchmark && mb_debug) {
        printf("WARNING: Running benchmark with assertions enabled.\n");
    } else if (!benchmark && !mb_debug) {
        printf("WARNING: Running tests with assertions disabled.\n");
    }

    /*
     * Make benchmark mode deterministic.
     */
    if (benchmark) {
        Random_SetSeed(0);
    } else {
        Random_GenerateSeed();
    }
    Random_Init();

#ifdef MB_HAS_SDL2
    SDL_Init(0);
#endif

    BenchmarkTest tests[] = {
            // enabled, weight, function
            { 1, 9000, testMBString     },
            { 1, 6000, testMBVector     },
            { 1, 6000, testCMBVector    },
            { 1, 800,  testMBStack      },
            { 1, 35,   testMBMap        },
            { 1, 400,  testIntMap       },
            { 1, 25,   testRandomIntMap },
            { 1, 4,    testMBSet        },
            { 1, 22,   testBitVector    },
            { 1, 410,  testMBQueue      },
            { 1, 4,    testMBRegistry   },
            { 1, 1,    testMBCompare    },
            { 1, 1,    testMBLock       },
            { 1, 1,    testMBRing       },
    };

    //Functional tests
    if (!benchmark) {
        testTypes();
        testRandom();
    }

    for (uint32 x = 0; x < ARRAYSIZE(tests); x++) {
        if (tests[x].enabled) {
            numTests++;
        } else {
            tests[x].weight = 0;
        }
    }

    for (uint32 x = 0; x < ARRAYSIZE(tests); x++) {
        if (tests[x].enabled) {
            numTests++;
        } else {
            tests[x].weight = 0;
        }
    }

    calibration = 150 * (seconds / numTests);
    if (calibration <= 0) {
        printf("Calibration < 0; resetting to 1\n");
        calibration = 1;
    }

    for (uint32 x = 0; x < ARRAYSIZE(tests); x++) {
        int runs;

        if (benchmark) {
            runs = tests[x].weight * calibration;
        } else {
            runs = tests[x].enabled ? 20 : 0;
        }

        /*
         * Do one run with a seed of zero, and then permute.
         */
        seed = 0;
        for (int y = 0; y < runs; y++) {
            tests[x].function();
            seed = Random_Uint32();
        }
    }

    Random_Exit();

    printf("%s successful!\n\n", benchmark ? "Benchmark" : "Tests");

#ifdef MB_HAS_SDL2
    SDL_Quit();
#endif

    printf("Done.\n");
    return 0;
}
