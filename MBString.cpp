#include "MBString.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "mbtypes.h"
#include "mbdebug.h"
#include "mbassert.h"

MBString::MBString()
:myLength(0),
 myCapacity(8),
 myChars(new char[myCapacity])
{
	myChars[myLength] = '\0';
}


MBString::MBString(int size, char fill)
:myLength(size),
 myCapacity(size+1),
 myChars(new char[myCapacity])
{
	memset(myChars, fill, myLength);
	myChars[myLength] = '\0';
}

MBString::MBString(int size)
:myLength(size),
 myCapacity(size+1),
 myChars(new char[myCapacity])
{
	myChars[myLength] = '\0';
}

MBString::MBString(char x)
:myLength(1),
 myCapacity(8),
 myChars(new char[myCapacity])
{
	myChars[0]=x;
	myChars[1]='\0';
}

//Precondition: c is a null-terminated string, or c = NULL
MBString::MBString(const char * c)
:myLength(0),
 myCapacity(0),
 myChars(null)
{	
	if (c) {
		myLength = strlen(c);
		myLength--;
		myCapacity = myLength+1;
		myChars = new char[myCapacity];
		memcpy(myChars, c, myLength);
	} else {
		myLength = 0;
		myCapacity = 8;
		myChars = new char[myCapacity];
	}
	
	myChars[myLength] = '\0';
	
}

MBString::MBString(const MBString & str)
:myLength(str.myLength),
 myCapacity(myLength+1),
 myChars(new char[myCapacity])
{
	memcpy(myChars, str.myChars, str.myLength);
	myChars[myLength] = '\0';
}

MBString::~MBString()
{
	delete [] myChars;
	myChars = NULL;
}

const MBString&  MBString::operator = (const MBString & str)
{
	if (this != &str) {
		myLength = 0;
		ensureCapacity(str.myLength + 1);
  		myLength = str.myLength;
  		memcpy(myChars, str.myChars, myLength);
		myChars[myLength] = '\0';
    }
    return *this;
}

//Precondition: c points to a null terminated string or c = NULL
const MBString & MBString::operator = (const char* c)
{
	int newLength;
	if (c) {
		newLength = strlen(c);
		newLength--;
		myLength = 0;
		ensureCapacity(newLength + 1);
		myLength = newLength;		
		memcpy(myChars, c, myLength);
	} else {
		myLength = 0;
	}
	myChars[myLength] = '\0';
	
	return *this;
}
const MBString & MBString::operator = (char c)
{
	myLength = 0;
	ensureCapacity(2);
	myLength = 1;
	myChars[0] = c;
	myChars[myLength] = '\0';
	return *this;
}

//Number of logical characters
int MBString::length() const
{
	return myLength;	
}

//return as C-String
const char* MBString::cstr() const
{
	return myChars;
}

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

MBString MBString::substr(int pos,int len) const
{
	if (pos + len > myLength) {
		PANIC("Substring out of range.");
	}
	if (pos < 0) {
		PANIC("Invalid index specified.");
	}
	
	MBString oup(len);
	
	for (int x = 0; x < len; x++) {
		oup += myChars[x+pos];
	}
	
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

char MBString::getCharAt(int k) const
{
	if (k >= myLength || k < 0) {
		PANIC("Index out of range.");
	}
	
	return myChars[k];
}

char MBString::operator[ ]( int k ) const
{
	return getCharAt(k);
}

char& MBString::operator[ ]( int k )
{
	if (k >= myLength || k < 0) {
		PANIC("Index out of range.");
	}
	return myChars[k];
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


void MBString::append(const MBString& str)
{
	int x;
	int y;
	
	ensureCapacity(myLength + str.myLength + 1);
	
	memcpy(&myChars[myLength], str.myChars, str.myLength);
	myLength = myLength + str.myLength;
	myChars[myLength] = '\0';
}

const MBString& MBString::operator += ( const MBString & str )
{
	append(str);
	
	return *this;
}


void MBString::append(char ch)
{
	ensureCapacity(myLength + 2);
	myChars[myLength] = ch;
	myLength++;
	myChars[myLength] = '\0';
}

const MBString& MBString::operator += ( char ch )
{
	append(ch);
	return *this;
}

ostream& operator << ( ostream& os, const MBString& str )
{
	return os << str.cstr();
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

// comparison operators:
bool operator == ( const MBString & lhs, const MBString & rhs )
{
	return lhs.compareTo(rhs) == 0;
}

bool operator != ( const MBString & lhs, const MBString & rhs )
{
	return lhs.compareTo(rhs) != 0;
}

bool operator <  ( const MBString & lhs, const MBString & rhs )
{
	return lhs.compareTo(rhs) < 0;
}

bool operator <= ( const MBString & lhs, const MBString & rhs )
{
	return lhs.compareTo(rhs) <= 0;
}
bool operator >  ( const MBString & lhs, const MBString & rhs )
{
	return lhs.compareTo(rhs) > 0;
}

bool operator >= ( const MBString & lhs, const MBString & rhs )
{
	return lhs.compareTo(rhs) >= 0;
}

// concatenation operator +
MBString operator + ( const MBString & lhs, const MBString & rhs )
{
	MBString oup(lhs.length() + rhs.length());
	oup = lhs;
	oup += rhs;
	return oup;
}

MBString operator + ( char ch, const MBString & str )
{
	MBString oup(1+str.length());
	oup = ch;
	oup += str;
	return oup;
}

MBString operator + ( const MBString & str, char ch )
{
	MBString oup (1+str.length());
	oup = str;
	oup += ch;
	return oup;
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

