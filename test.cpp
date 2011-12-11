#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "MBString.h"
#include "MBStack.h"
#include "mbassert.h"
#include "MBVector.h"
#include "MBSet.h"
#include "IntSet.h"
#include "BitVector.h"
#include "mbutil.h"
#include "random.h"
#include "MBMap.h"

void testTypes()
{
	typedef struct TypeSize {
		const char *name;
		int expectedSize;
		int actualSize;
	} TypeSize;
	
	TypeSize types[] = {
		{"uint8",  1, sizeof(uint8)},
		{"int8",   1, sizeof(int8)},
		{"uint16", 2, sizeof(uint16)},
		{"int16",  2, sizeof(int16)},
		{"uint32", 4, sizeof(uint32)},
		{"int32",  4, sizeof(int32)},
		{"uint64", 8, sizeof(uint64)},
		{"int64",  8, sizeof(int64)},
		{"bool",   1, sizeof(bool)},
		{ NULL,    0, 0 }
	};
	
	int x = 0;
	while (types[x].name != NULL) {
		if (types[x].actualSize != types[x].expectedSize) {
			fprintf(stderr, "Bad %s size: expected %d, actual %d\n",
			        types[x].name,
			        types[x].expectedSize,
			        types[x].actualSize);
			ASSERT(FALSE);
        }
        x++;
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
	ASSERT(result == 12);
	result = cstr[0];
	ASSERT(result == 'H');
	result = cstr[1];
	ASSERT(result == 'e');
	result = cstr[6];
	ASSERT(result == 'W');
	result = cstr[10];
	ASSERT(result == 'd');
	result = cstr[11];
	ASSERT(result == '!');
	
	str = "Goodbye World!";
	str = "Hello World!";
	
	str2 = str;
	
	str2 = "Hello World!";
	result = str2.length();
	ASSERT(result == 12);
	result = str2[0];
	ASSERT(result == 'H');
	result = str2[1];
	ASSERT(result == 'e');
	result = str2[6];
	ASSERT(result == 'W');
	result = str2[10];
	ASSERT(result == 'd');
	result = str2[11];
	ASSERT(result == '!');
	
	str = "Hello World!";
	result = str.find('H');
	ASSERT(result == 0);
	result = str.find('W');
	ASSERT(result == 6);
	result = str.find("W");
	ASSERT(result == 6);
	
	result = str.find("Hello World!");
	ASSERT(result == 0);
	
	result = str.find(str);
	ASSERT(result == 0);
	
	result = str.find("rld");
	ASSERT(result == 8);
	
	result = str.find("Hello");
	ASSERT(result == 0);	

	result = str.find("ld!");
	ASSERT(result == 9);
	
	result = str.find("punk");
	ASSERT(result == -1);
	
	result = str.find("elxxW");
	ASSERT(result == -1);
	
	result = str.find("elW");
	ASSERT(result == -1);
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
		ASSERT(result == x);
		result = r.pop();
		ASSERT(result == x);
	}
}

void testMBVector(void)
{
	int count = 1000;
	int result;
	MBVector<int> s;
	MBVector<int> r;
	
	for (int y = 0; y < 2; y++) {
		s.makeEmpty();
		for (int x = 0; x <= count; x++) {
			s.push(x);
		}
		r = s;
	}
	
	for (int x = count; x >= 0; x--) {
		result = s.pop();
		ASSERT(result == x);
		result = r.pop();
		ASSERT(result == x);
	}
}

void testMBSet(void)
{
	int count = 1000;
	int result;
	
	MBSet<int> s;
	
	for (int x = 0; x <= count; x++) {
		s.add(x);
		ASSERT(s.size() == x + 1);
	}
	
	for (int x = 0; x <= count; x++) {
		result = s.contains(x);
		ASSERT(result);
	}
	
	for (int x = count+1; x <= count*2; x++) {
		result = s.contains(x);
		ASSERT(!result);
	}
}

void testIntSet(void)
{
	int count = 1000;
	int result;
	
	IntSet s;
	
	for (int x = 0; x <= count; x++) {
		s.add(x);
		ASSERT(s.size() == x + 1);
	}
	
	for (int x = 0; x <= count; x++) {
		result = s.contains(x);
		ASSERT(result);
	}
	
	for (int x = count+1; x <= count*2; x++) {
		result = s.contains(x);
		ASSERT(!result);
	}
}

void testBitVector(void)
{
	int count = 10000;
	int result;
	BitVector b;
	
	b.resize(count + 1);
	
	for (int x = 0; x <= count; x++) {
		b.set(x);
	}
	
	b.resize((count + 1)* 2);
	for (int x = count + 1; x <= count * 2; x++) {
		b.reset(x);
	}
	
	for (int x = 0; x <= count; x++) {
		result = b.get(x);
		ASSERT(result);
	}
	
	for (int x = count + 1; x <= count * 2; x++) {
		result = b.get(x);
		ASSERT(!result);
	}
	
	b.setAll();
	for (int x = 0; x <= count * 2; x++) {
		result = b.get(x);
		ASSERT(result);
	}
	
	b.resetAll();
	for (int x = 0; x <= count * 2; x++) {
		result = b.get(x);
		ASSERT(!result);
	}
	
	b.setRange(1, count / 2);
	for (int x = 1; x <= count/2; x++) {
		result = b.get(x);
		ASSERT(result);
	}
	for (int x = count/2+1; x <= count; x++) {
		result = b.get(x);
		ASSERT(!result);
	}
	
	for (int x = 0; x <= count; x++) {
		b.put(x, (x % 2) ? TRUE : FALSE);
	}
	for (int x = 0; x <= count; x++) {
		result = b.get(x);
		ASSERT(result == (x % 2) ? TRUE : FALSE);
		b.flip(x);
		result = b.get(x);
		ASSERT(result == (x % 2) ? FALSE : TRUE);
	}
}

void testMBMap()
{
	MBMap<MBString, int> map;

	for(int x=0;x<25;x++) {
		MBString key = MBString::toString(x);
		map[key] = x;
	}
	
	for(int x=0;x<25;x++) {
		MBString key = MBString::toString(x);
		ASSERT(map[key] == x);
	}
	
	for(int x=0;x<25;x++) {
		MBString key = MBString::toString(x);
		int value = x+10;
		map[key] = value;
	}
	
	for(int x=0;x<25;x++) {
		MBString key = MBString::toString(x);
		int value = x+10;
		ASSERT(map[key] == value);
	}
}

void testIntMap()
{
	IntMap m;
	IntMap n;
	int result;

	for(int x=0;x<100;x++) {
		m.put(x,x);
		result = m.get(x);
		ASSERT(x == result);
	}
	
	result = m.get(101);
	ASSERT(result == 0);
	
	for(int x=0;x<100;x++) {
		result = m.get(x);
		ASSERT(x == result);
	}

	n.insertAll(m);
	for(int x=0;x<100;x++) {
		result = n.get(x);
		ASSERT(x == result);
	}
	
	for(int x=100;x<200;x++) {
		result = n.containsKey(x);
		ASSERT(!result);
	}	
}



void testRandom(void)
{
    Random_Init();

	int x, y;
	int count = 100;

	uint64 num;
	for (x = 0;x < count; x++) {
		num = Random_Uint64();
	}	
	
	for (x = 0;x < count; x++) {
		num = Random_Uint32();
	}
	
	for (x = 0;x < count; x++) {
		num = 0;
		for (y = 0; y < 32; y++) {
			num <<= 1;
			if (Random_Bit()) {
				num |= 1;
			}
		}
	}

	Random_Exit();
}


typedef struct {
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
	
	BenchmarkTest tests[] = {
		// enabled, weight, function
		{ 1, 120000, testMBString  },
		{ 1, 10000,  testMBVector  },
		{ 1, 5000,   testMBStack   },
		{ 1, 20,     testMBSet     },
		{ 1, 800,    testIntSet    },
		{ 1, 1300,   testBitVector },
		{ 1, 8000,   testMBMap     },
		{ 1, 2700,   testIntMap    },
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
	
	calibration = 10 * (seconds / numTests);
	if (calibration <= 0) {
		printf("Calibration < 0; resetting to 1\n");
		calibration = 1;
	}
	
	runs = 1;
	for (uint32 x = 0; x < ARRAYSIZE(tests); x++) {
		if (benchmark) {
			runs = tests[x].weight * calibration;
		}
		
		for (int y = 0; y < runs; y++) {
			tests[x].function();
		}
	}
	
	if (!benchmark) {
		printf("Done.\n");
	}
	
	return 0;
}

