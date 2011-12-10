#ifndef BitVector_H_201001192326
#define BitVector_H_201001192326

#include "mbtypes.h"

class BitVector
{
	public:
		BitVector();
		BitVector(const BitVector &a);
		explicit BitVector(int size);
		BitVector(int size, bool initial);
		~BitVector();
		
		bool getFillValue() const;
		void setFillValue(bool f);
		bool get(int x) const;
		
		//returns the old value
		bool put(int x, bool v);
		bool set(int x);
		bool reset(int x);
		
		void flip(int x);
		
		//affects the bits from [first..last] inclusive
		void setRange(int first, int last);
		void resetRange(int first, int last);
		
		void setAll();
		void resetAll();		
		
		int size() const;
		void resize(int length);
		void makeEmpty();
		
		bool operator [] (int x) const;
		
		//This is a weird function.
		//It empties this array, copies over everything from a,
		//  and then leaves a empty.
		//Fill is left unchanged
		void consume(BitVector & a);	
	
	private:
		uint32 *myBits;
		int mySize;
		int myArrSize;
		bool myFill;
		
		static const int ALL_ON = 0xFFFFFFFF;
		static const int UNIT_SIZE = 32;
		static const int DEFAULT_SPACE = 1;	
};



#endif //BitVector_H_201001192326
