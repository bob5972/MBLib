
// MBLib test suite

#include <iostream>

#include "MBMap.h"
#include "IntMap.h"
#include "MBString.h"

using namespace std;

void testIntMap();
void testMBMap();

int main()
{
	testIntMap();
	testMBMap();	

	cout << "Finished!"<<endl;	
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
