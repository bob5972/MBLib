
// MBLib test suite

#include <iostream>
#include <stdio.h>

#include "MBMap.h"
#include "IntMap.h"
#include "MBString.h"

#include "mjbtypes.h"
#include "mjbdebug.h"

using namespace std;

void testIntMap();
void testMBMap();
void testTypes();

int main()
{
	testTypes();
	testIntMap();
	testMBMap();	

	cout << "Finished!"<<endl;	
}

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
	
	cout << "Testing sizes ..." << endl;
	
	int x = 0;
	while (types[x].name != NULL) {
		if (types[x].actualSize != types[x].expectedSize) {
			fprintf(stderr, "Bad %s size: expected %d, actual %d\n",
			        types[x].name,
			        types[x].expectedSize,
			        types[x].actualSize);
        }
        x++;
    }
}

void testMBMap()
{
	MBMap<MBString, int> map;

	cout << "Testing MBMap ..." << endl;

	for(int x=0;x<25;x++) {
		MBString key = MBString::toString(x);
		map[key] = x;
	}
	
	for(int x=0;x<25;x++) {
		MBString key = MBString::toString(x);
		if(map[key] != x) {
			cerr << "Error on x = "<<x<<endl;
		}
	}
	
	for(int x=0;x<25;x++) {
		MBString key = MBString::toString(x);
		int value = x+10;
		map[key] = value;
	}
	
	for(int x=0;x<25;x++) {
		MBString key = MBString::toString(x);
		int value = x+10;
		if(map[key] != value) {
			cerr << "Error on pass 2, map[" << key << "] = "<<map[key] << ", should be "<<value << endl;
		}
	}
}



void testIntMap()
{
	IntMap m;
	IntMap n;

	cout << "Testing IntMap ..." << endl;
	
	for(int x=0;x<100;x++) {
		m.put(x,x);
		if( x != m.get(x)) {
			cerr << "Error on x = "<<x<<endl;
		}
	}
	
	if( 0 != m.get(101)) {
		cerr << "Error on line" << __LINE__ << endl;
	}
	
	for(int x=0;x<100;x++) {
		if( x != m.get(x)) {
			cerr << "Error on x = "<<x<<endl;
		}
	}

	n.insertAll(m);
	for(int x=0;x<100;x++) {
		if( x != n.get(x)) {
			cerr << "Error on insertAll, x = " << x << endl;
		}
	}
	for(int x=100;x<200;x++) {
		if( n.containsKey(x)) {
			cerr << "Error on insertAll, x = " << x << endl;
		}
	}
	
}
