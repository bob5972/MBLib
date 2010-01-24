#include "IntMap.h"
#include <iostream>

using namespace std;

int main()
{
	IntMap m;
	
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
	
	
	
	cout << "Finished!"<<endl;	
}
