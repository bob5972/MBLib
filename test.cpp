#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "MBString.hpp"
#include "MBStack.hpp"
#include "mbassert.h"
#include "MBVector.hpp"
#include "MBSet.hpp"
#include "IntSet.hpp"
#include "BitVector.hpp"
#include "mbutil.h"
#include "random.h"
#include "MBMap.hpp"
#include "MBQueue.hpp"

DECLARE_MBVECTOR_TYPE(int, IntVector);

#define TEST(x) \
do { \
    int cond = (x); \
    if (!cond) { \
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
            s.push(x);
        }
        r = s;
        r.trim();
    }

    for (int x = count; x >= 0; x--) {
        result = s.pop();
        TEST(result == x);
        result = r.pop();
        TEST(result == x);
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
            s.push(x);
            r.push(s[x]);
            result = r.pop();
            TEST(result == x);
        }
        r = s;
    }

    for (int x = count; x >= 0; x--) {
        result = s[x];
        TEST(result == x);
        result = s.size();
        TEST(result == x + 1);
        result = s.pop();
        TEST(result == x);

        result = r[x];
        TEST(result == x);
        result = r.size();
        TEST(result == x + 1);
        result = r.pop();
        TEST(result == x);
    }

    s.resize(count + 1);
    r.resize(count + 1);
    for (int x = 0; x <= count; x++) {
        s[x] = x;
        r[x] = s[x];
    }
    for (int x = 0; x <= count; x++) {
        result = s[x];
        TEST(result == x);
        result = r[x];
        TEST(result == x);
    }
}

void testCMBVector(void)
{
    int result;
    IntVector s;
    IntVector r;
    int sizes[] = {
        0, 1, 10, 100, 200, 300,
    };
    int *value;
    int count = 100;

    IntVector_CreateEmpty(&s);
    IntVector_CreateEmpty(&r);

    for (unsigned y = 0; y < ARRAYSIZE(sizes); y++) {
        IntVector_MakeEmpty(&s);
        result = IntVector_Size(&s);
        TEST(result == 0);

        for (unsigned x = y; x < ARRAYSIZE(sizes); x++) {
            IntVector_Resize(&s, sizes[y]);
            result = IntVector_Size(&s);
            TEST(result == sizes[y]);
            IntVector_Resize(&s, sizes[x]);
            result = IntVector_Size(&s);
            TEST(result == sizes[x]);
            IntVector_Resize(&s, sizes[y]);
            result = IntVector_Size(&s);
            TEST(result == sizes[y]);

            IntVector_MakeEmpty(&s);
            result = IntVector_Size(&s);
            TEST(result == 0);
            result = IntVector_IsEmpty(&s);
            TEST(result);
        }
    }

    IntVector_Resize(&s, count);
    result = IntVector_Size(&s);
    TEST(result == count);
    result = IntVector_IsEmpty(&s);
    TEST(!result);

    for (int x = 0; x < IntVector_Size(&s); x++) {
        value = IntVector_Get(&s, x);
        *value = x;
    }
    for (int x = 0; x < IntVector_Size(&s); x++) {
        value = IntVector_Get(&s, x);
        TEST(*value == x);
    }

    IntVector_GrowBy(&s, count);
    result = IntVector_Size(&s);
    TEST(result == 2 * count);
    for (int x = 0; x < IntVector_Size(&s); x++) {
        value = IntVector_Get(&s, x);
        if (x < count) {
            TEST(*value == x);
        }
        *value = x;
    }
    for (int x = 0; x < IntVector_Size(&s); x++) {
        value = IntVector_Get(&s, x);
        TEST(*value == x);
    }

    IntVector_ShrinkBy(&s, count);
    result = IntVector_Size(&s);
    TEST(result == count);
    for (int x = 0; x < IntVector_Size(&s); x++) {
        value = IntVector_Get(&s, x);
        TEST(*value == x);
    }

    IntVector_Copy(&r, &s);
    result = IntVector_Size(&r);
    TEST(result == count);
    for (int x = 0; x < IntVector_Size(&r); x++) {
        value = IntVector_Get(&r, x);
        TEST(*value == x);
    }

    IntVector_Destroy(&s);
    IntVector_Destroy(&r);
}

void testMBSet(void)
{
    int count = 1000;
    int result;

    MBSet<int> s;

    for (int x = 0; x <= count; x++) {
        s.add(x);
        TEST(s.size() == x + 1);
    }

    for (int x = 0; x <= count; x++) {
        result = s.contains(x);
        TEST(result);
    }

    for (int x = count + 1; x <= count * 2; x++) {
        result = s.contains(x);
        TEST(!result);
    }
}

void testIntSet(void)
{
    int count = 1000;
    int result;

    IntSet s;

    for (int x = 0; x <= count; x++) {
        s.add(x);
        result = s.size();
        TEST(result == x + 1);
    }

    for (int x = 0; x <= count; x++) {
        result = s.contains(x);
        TEST(result);
    }

    for (int x = count + 1; x <= count * 2; x++) {
        result = s.contains(x);
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
            MBString key = MBString::toString(x);
            map[key] = x;
        }

        for (int x = 0; x < 25; x++) {
            MBString key = MBString::toString(x);
            TEST(map[key] == x);
        }

        for (int x = 0; x < 25; x++) {
            MBString key = MBString::toString(x);
            int value = x + 10;
            map[key] = value;
        }

        for (int x = 0; x < 25; x++) {
            MBString key = MBString::toString(x);
            int value = x + 10;
            TEST(map[key] == value);
        }
    }

    if (runIntTests) {
        MBMap<int, int> m;

        for (int x = 0; x < count; x++) {
            m.put(x, x);
            result = m.get(x);
            TEST(x == result);
            m[x] += 1;
            result = m.get(x);
            TEST(result == x + 1);
            m[x] -= 1;
            result = m.get(x);
            TEST(result == x);

            result = m.size();
            TEST(result == x + 1);
        }

        result = m.size();
        TEST(result == count);

        for (int x = 0; x < count; x++) {
            result = m.containsKey(x);
            TEST(result);
            result = m.get(x);
            TEST(x == result);
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

    }
}

void testIntMap()
{
    IntMap m;
    int result;
    int num;
    const int count = 1000;

    for (int x = 0; x < count; x++) {
        m.put(x, x);
        result = m.get(x);
        TEST(x == result);
        m.increment(x);
        result = m.get(x);
        TEST(result == x + 1);
        m.decrement(x);
        result = m.get(x);
        TEST(result == x);

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
        TEST(x == result);
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
    const int count = 2000;

    for (int x = 0; x < count; x++) {
        num = Random_Uint32();
        result = m.get(num);

        m.put(num, x);
        result = m.get(num);
        TEST(x == result);
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
        UNUSED_VARIABLE(d);
        d = Random_UnitFloat();
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
    int runs;

    benchmark = FALSE;
#ifdef BENCHMARK
    benchmark = TRUE;
#endif

    if (argc > 1) {
        arg = argv[1];

        if (arg == "-b") {
            benchmark = TRUE;
        } else if (arg == "-f") {
            benchmark = FALSE;
        }
    }

    Random_Init();

    BenchmarkTest tests[] = {
            // enabled, weight, function
            { 1, 8000, testMBString     },
            { 1, 3000, testMBVector     },
            { 1, 3000, testCMBVector    },
            { 1, 400,  testMBStack      },
            { 1, 15,   testMBMap        },
            { 1, 20,   testIntMap       },
            { 1, 25,   testRandomIntMap },
            { 1, 65,   testIntSet       },
            { 1, 2,    testMBSet        },
            { 1, 12,   testBitVector    },
            { 1, 210,  testMBQueue       },
    };

    //Functional tests
    if (!benchmark) {
#if !MB_DEBUG
        printf("Warning: Running tests with assertions disabled.\n");
#endif
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
        if (benchmark) {
            runs = tests[x].weight * calibration;
        } else {
            runs = tests[x].enabled ? 1 : 0;
        }

        for (int y = 0; y < runs; y++) {
            tests[x].function();
        }
    }

    Random_Exit();

    if (!benchmark) {
        printf("\nTests successful!\n\n");
        printf("Done.\n");
    }

    return 0;
}

