#include "BasicString.h"

#include <stdlib.h>
#include <ctype.h>

#include "mjbtypes.h"
#include "mjbdebug.h"
#include "mjbassert.h"

BasicString::BasicString()
:myLength(0),
 myCapacity(1),
 myChars(new char[myCapacity])
{
	myChars[myLength] = '\0';
}


BasicString::BasicString(int size, char fill)
:myLength(size),
 myCapacity(size+1),
 myChars(new char[myCapacity])
{
	for (int x=0;x<myLength;x++)
	{
		myChars[x] = fill;
	}
	myChars[myLength] = '\0';
}

BasicString::BasicString(int size)
:myLength(size),
 myCapacity(size+1),
 myChars(new char[myCapacity])
{
	myChars[myLength] = '\0';
}

BasicString::BasicString(char x)
:myLength(1),
 myCapacity(2),
 myChars(new char[myCapacity])
{
	myChars[0]=x;
	myChars[1]='\0';
}

//Precondition: c is a null-terminated string, or c = NULL
BasicString::BasicString(const char * c)
:myLength(0),
 myCapacity(0),
 myChars(null)
{	
	if (c) {
		while (c[myLength++]);
		myLength--;
		myCapacity = myLength+1;
		myChars = new char[myCapacity];
		for (int x=0;x<myLength;x++) {
			myChars[x] = c[x];
		}
	} else {
		myLength = 0;
		myCapacity = 1;
		myChars = new char[myCapacity];
	}
	
	myChars[myLength] = '\0';
	
}

BasicString::BasicString(const BasicString & str)
:myLength(str.myLength),
 myCapacity(myLength+1),
 myChars(new char[myCapacity])
{
	for (int x=0;x<myLength;x++) {
		myChars[x] = str.myChars[x];
	}
	myChars[myLength] = '\0';
}

BasicString::~BasicString()
{
	delete [] myChars;
	myChars = NULL;
}

const BasicString&  BasicString::operator = (const BasicString & str)
{
	if (this != &str) {
		if (myCapacity < str.myLength) {
    		delete [] myChars;
    		myCapacity = str.myLength+1;
    		myChars = new char[myCapacity];    		
  		}
  		myLength = str.myLength;
  		for (int x=0;x<myLength;x++) {
  			myChars[x] = str.myChars[x];
		}
		myChars[myLength] = '\0';
    }
    return *this;
}

//Precondition: c points to a null terminated string or c = NULL
const BasicString & BasicString::operator = (const char* c)

{
	if (c) {
		myLength = 0;
  		while (c[myLength++]);
		myLength--;
		if (myLength+1 > myCapacity) {
			delete[] myChars;
			myCapacity = myLength+1;
			myChars = new char[myCapacity];
   			
		}		
		
		for (int x=0;x<myLength;x++) {
			myChars[x] = c[x];
		}
	}
	else {
		myLength = 0;
	}
	myChars[myLength] = '\0';
	
	return *this;
}
const BasicString & BasicString::operator = (char c)
{
	
	myLength = 1;
	if ( myLength +1> myCapacity)
	{
		delete[] myChars;
		myCapacity = myLength+1;
  		myChars = new char[myCapacity];		
	}
	myChars[0] = c;
	myChars[myLength] = '\0';
	return *this;
}

//Number of logical characters
int BasicString::length() const
{
	return myLength;	
}

//return as C-String
const char* BasicString::cstr() const
{
	return myChars;
}

int BasicString::find(char ta) const
{
	for (int x=0;x<myLength;x++) {
		if (myChars[x] == ta) {
			return x;
		}
	}
	return -1;
}

int BasicString::find(const BasicString & str) const
{
	int x=0;
	int found=0;
	
	if(myLength == 0) {
		return -1;
	}
	
	if(str.myLength == 0) {
		return 0;
	}
	
	while( x<=(myLength-str.myLength) && found<str.myLength ) {
		if(myChars[x] == str.myChars[found]) {
			found++;
		}
		x++;
	}

	if(found == str.myLength) {
		return x-str.myLength;
	} else {
		return -1;
	}
}

BasicString BasicString::substr(int pos,int len) const
{
	if (pos+len>myLength) {
		PANIC("Substring out of range.");
	}
	if (pos < 0) {
		PANIC("Invalid index specified.");
	}
	
	BasicString oup(len);
	
	for (int x=0;x<len;x++) {
		oup += myChars[x+pos];
	}
	
	return oup;
}

BasicString BasicString::toUpper() const
{
	BasicString oup(myLength);
	for (int x=0;x<myLength;x++) {
		oup += toupper(myChars[x]);
	}
	return oup;
}

BasicString BasicString::toLower() const
{
	BasicString oup(myLength);
	for(int x=0;x<myLength;x++) {
		oup+= tolower(myChars[x]);
	}
	return oup;
}

char BasicString::getCharAt(int k) const
{
	if (k >= myLength || k < 0) {
		PANIC("Index out of range.");
	}
	
	return myChars[k];
}

char BasicString::operator[ ]( int k ) const
{
	return getCharAt(k);
}

char& BasicString::operator[ ]( int k )
{
	if (k >= myLength || k< 0) {
		PANIC("Index out of range.");
	}
	return myChars[k];
}


void BasicString::append(const BasicString& str)
{
	if (myCapacity < myLength + str.myLength+1) {
		char* temp = myChars;
		while (myCapacity <myLength + str.myLength+1) {
			ASSERT(myCapacity > 0);
			myCapacity *= 2;
		}
		myChars = new char[myCapacity];
		for (int x=0;x<myLength;x++) {
			myChars[x] = temp[x];
		}
		delete[] temp;
	}
	
	int y=0;
	int x=myLength;
	myLength = myLength + str.myLength;
	while(x<myLength) { 
		myChars[x++] = str.myChars[y++];
	}
	myChars[myLength] = '\0';
}

const BasicString& BasicString::operator += ( const BasicString & str )
{
	append(str);
	
	return *this;
}


void BasicString::append(char ch)
{
	if (myCapacity < myLength + 2) {
		char* temp = myChars;
		ASSERT(myCapacity > 0);
		myCapacity *= 2;
		
		myChars = new char[myCapacity];
		for (int x=0;x<myLength;x++) {
			myChars[x] = temp[x];
		}
		delete[] temp;
	}
	myChars[myLength++] = ch;
	myChars[myLength] = '\0';
}

const BasicString& BasicString::operator += ( char ch )
{
	append(ch);
	return *this;
}

ostream& operator << ( ostream& os, const BasicString& str )
{
	return os << str.cstr();
}

istream& operator >> ( istream& is, BasicString& str )
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
	
istream& getline( istream& is, BasicString& str )
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

int BasicString::compareTo(const BasicString& rhs) const
{
	int maxx= (myLength < rhs.myLength)? myLength : rhs.myLength;
	
	for (int x=0;x<maxx;x++) {
		if (getCharAt(x) < rhs[x]) {
			return -1;
		} else if (getCharAt(x) > rhs[x]) {
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
bool operator == ( const BasicString & lhs, const BasicString & rhs )
{
	return lhs.compareTo(rhs) == 0;
}

bool operator != ( const BasicString & lhs, const BasicString & rhs )
{
	return lhs.compareTo(rhs) != 0;
}

bool operator <  ( const BasicString & lhs, const BasicString & rhs )
{
	return lhs.compareTo(rhs) < 0;
}

bool operator <= ( const BasicString & lhs, const BasicString & rhs )
{
	return lhs.compareTo(rhs) <= 0;
}
bool operator >  ( const BasicString & lhs, const BasicString & rhs )
{
	return lhs.compareTo(rhs) > 0;
}

bool operator >= ( const BasicString & lhs, const BasicString & rhs )
{
	return lhs.compareTo(rhs) >= 0;
}

// concatenation operator +
BasicString operator + ( const BasicString & lhs, const BasicString & rhs )
{
	BasicString oup(lhs.length() + rhs.length());
	oup = lhs;
	oup += rhs;
	return oup;
}

BasicString operator + ( char ch, const BasicString & str )
{
	BasicString oup(1+str.length());
	oup = ch;
	oup += str;
	return oup;
}

BasicString operator + ( const BasicString & str, char ch )
{
	BasicString oup (1+str.length());
	oup = str;
	oup += ch;
	return oup;
}


