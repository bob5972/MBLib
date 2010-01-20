#ifndef BitArray_H_201001192326
#define BitArray_H_201001192326

#include "mjbtypes.h"



class BitArray
{
	public:
		BitArray();
		BitArray(const BitArray &a);
		explicit BitArray(int size);
		BitArray(int size, bool initial);
		
		bool getFillValue() const;
		void setFillValue(bool f);
		bool get(int x) const;
		
		void put(int x, bool v);
		void set(int x);
		void flip(int x);
		void reset(int x);
		
		//affects the bits from [first..last] inclusive
		void setRange(int first, int last);
		void resetRange(int first, int last);
		
		void setAll();
		void resetAll();
		
		
		int size() const;
		void resize(int length);
		
		bool& operator [] (int x);
		bool operator [] (int x) const;
	
	
	
	private:
		uint32 *myBits;
		int mySize;
		int myArrSize;
		bool myFill;
		
		static const int ALL_ON = 0xFFFFFFFF;
		static const int UNIT_SIZE = 32;
	
	
};



#endif //BitArray_H_201001192326
