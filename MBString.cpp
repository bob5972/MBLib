#include "MBString.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "mbtypes.h"
#include "mbdebug.h"
#include "mbassert.h"


int MBString::find(char ta) const
{
	for (int x=0;x<myLength;x++) {
		if (myChars[x] == ta) {
			return x;
		}
	}
	return -1;
}

int MBString::find(const MBString & str) const
{
	int x = 0;
	
	if (myLength == 0) {
		return -1;
	}
	
	if (str.myLength == 0) {
		return 0;
	}
	
	x = 0;
	while (x + str.myLength - 1 < myLength) {
		const char *start = &myChars[x];
		
		if (memcmp(start, str.myChars, str.myLength) == 0) {
			return x;
		}
		
		x++;
	}
		
	return -1;
}

/*
 * return len chars starting at pos
 */
MBString MBString::substr(int pos, int len) const
{
	ASSERT(pos + len <= myLength);
	ASSERT(pos >= 0);
	
	MBString oup;
	
	for (int x = 0; x < len; x++) {
		oup += myChars[x+pos];
	}
	
	ASSERT(oup.myLength == len);
	ASSERT(oup.myChars[len] == '\0');
	return oup;
}

MBString MBString::toUpper() const
{
	MBString oup(myLength);
	for (int x=0;x<myLength;x++) {
		oup += toupper(myChars[x]);
	}
	return oup;
}

MBString MBString::toLower() const
{
	MBString oup(myLength);
	for(int x=0;x<myLength;x++) {
		oup+= tolower(myChars[x]);
	}
	return oup;
}

/*
 * Ensure the string has at least the specified capacity.
 * Can only ever enlarge the buffer.
 */
 
void MBString::ensureCapacity(int cap)
{
	ASSERT(myCapacity >= myLength + 1);
	ASSERT(cap > 0);
	
	if (myCapacity < cap) {
		char *temp = myChars;
		while (myCapacity < cap) {
			ASSERT(myCapacity > 0);
			myCapacity *= 2;
		}
		ASSERT(myCapacity >= myLength + 1);
		
		myChars = new char[myCapacity];
		ASSERT(myChars != NULL);
		memcpy(myChars, temp, myLength + 1);
		ASSERT(myChars[myLength] == '\0');
		
		delete[] temp;
	}
	
	ASSERT(myCapacity >= cap);
}


istream& operator >> ( istream& is, MBString& str )
//precondition:  input stream is open for reading
//postcondition: the next string from input stream is has been read
//               and stored in str
{
    char ch;
    str = "";    // empty string, will build one char at-a-time
    is >> ch;    // whitespace skipped, first non-white char in ch
    
    if (! is.fail()) {
        do
        {
            str += ch;
            is.get(ch);
        } while (! is.fail() && ! isspace(ch));
        
		// put whitespace back on the stream
        if (isspace(ch)) {
            is.putback(ch);     
        }
    }
    
    return is;
}
	
istream& getline( istream& is, MBString& str )
//description:   reads a line from input stream is into the string str
//precondition:  input stream is open for reading
//postcondition: chars from input stream is up to '\n' have been read
{

    char ch;
    str = "";     // empty string, will build one char at-a-time
    
    while (is.get(ch) && ch != '\n') {
        str += ch;
    }
    
    return is;
}

int MBString::compareTo(const MBString& rhs) const
{
	int maxx;
	
	if (myLength < rhs.myLength) {
		maxx = myLength;
	} else {
		maxx = rhs.myLength;
	}
	
	for (int x = 0;x < maxx; x++) {
		if (myChars[x] < rhs.myChars[x]) {
			return -1;
		} else if (myChars[x] > rhs.myChars[x]) {
			return 1;
		}
	}
	
	if (myLength < rhs.myLength) {
		return -1;
	} else if ( myLength > rhs.myLength ) {
		return 1;
	}
	
	return 0;
}

//Not terribly efficient, but effective.
MBString MBString::toString(int x)
{
	if( x == 0) {
		return "0";
	}
	
	MBString oup;
	bool negative = false;
	if(x <0) {
		negative=true;
		x = -x;
	}
	
	int digit;
	char c;
	
	while(x>0) {
		digit = x%10;
		c = '0' + digit;
		oup += c;
		x-= digit;
		x/=10;
	}
	
	int size = oup.length();
	for(int x=0;x<size/2;x++) {
		c = oup[x];
		oup[x] = oup[size-x-1];
		oup[size-x-1] = c;
	}
	
	if(negative) {
		oup = '-'+oup;
	}
	

	return oup;
}

