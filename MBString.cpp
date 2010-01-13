#include "MBString.h"

#include <stdlib.h>
#include <ctype.h>

#include "mjbtypes.h"
#include "mjbdebug.h"
#include "mjbassert.h"

MBString::MBString()
:myLength(0),
 myCapacity(1),
 myChars(new char[myCapacity])
{
	myChars[myLength] = '\0';
}


MBString::MBString(int size, char fill)
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

MBString::MBString(int size)
:myLength(size),
 myCapacity(size+1),
 myChars(new char[myCapacity])
{
	myChars[myLength] = '\0';
}

MBString::MBString(char x)
:myLength(1),
 myCapacity(2),
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

MBString::MBString(const MBString & str)
:myLength(str.myLength),
 myCapacity(myLength+1),
 myChars(new char[myCapacity])
{
	for (int x=0;x<myLength;x++) {
		myChars[x] = str.myChars[x];
	}
	myChars[myLength] = '\0';
}

MBString::~MBString()
{
	delete [] myChars;
}

const MBString&  MBString::operator = (const MBString & str)
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
const MBString & MBString::operator = (const char* c)

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
const MBString & MBString::operator = (char c)
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

MBString MBString::substr(int pos,int len) const
{
	if (pos+len>myLength) {
		PANIC("Substring out of range.");
	}
	if (pos < 0) {
		PANIC("Invalid index specified.");
	}
	
	MBString oup(len);
	
	for (int x=0;x<len;x++) {
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

char MBString::operator[ ]( int k ) const
{
	if (k >= myLength || k < 0) {
		PANIC("Index out of range.");
	}
	
	return myChars[k];
}

char& MBString::operator[ ]( int k )
{
	if (k >= myLength || k< 0) {
		PANIC("Index out of range.");
	}
	return myChars[k];
}

//Append str
const MBString& MBString::operator += ( const MBString & str )
{
	if (myCapacity < myLength + str.myLength+1) {
		char* temp = myChars;
		while (myCapacity <myLength + str.myLength) {
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
	return *this;
}
		

//Append char
const MBString& MBString::operator += ( char ch )
{
	if (myCapacity < myLength + 2) {
		char* temp = myChars;
		myCapacity *= 2;
		
		myChars = new char[myCapacity];
		for (int x=0;x<myLength;x++) {
			myChars[x] = temp[x];
		}
		delete[] temp;
	}
	myChars[myLength++] = ch;
	myChars[myLength] = '\0';
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

// comparison operators:
bool operator == ( const MBString & lhs, const MBString & rhs )
{
	if (lhs.length() == rhs.length()) {
		int s=lhs.length();
  		for (int x=0;x<s;x++) {
  			if (lhs[x] != rhs[x])
  				return false;
		}
		return true;
	}
	return false;
}

bool operator != ( const MBString & lhs, const MBString & rhs )
{
	return !(lhs == rhs);
}

bool operator <  ( const MBString & lhs, const MBString & rhs )
{
	int maxx= (lhs.length() < rhs.length())? lhs.length() : rhs.length();
	for (int x=0;x<maxx;x++) {
		if (lhs[x] < rhs[x]) {
			return true;
		} else if (lhs[x] > rhs[x]) {
			return false;
		}
	}
	
	if (lhs.length() < rhs.length()) {
		return true;
	}
	
	return false;
}

bool operator <= ( const MBString & lhs, const MBString & rhs )
{
	int maxx= (lhs.length() < rhs.length())? lhs.length() : rhs.length();
	for (int x=0;x<maxx;x++) {
		if (lhs[x] < rhs[x]) {
			return true;
		} else if (lhs[x] > rhs[x]) {
			return false;
		}
	}
	if (lhs.length() <= rhs.length()) {
		return true;
	}
	
	return false;
}
bool operator >  ( const MBString & lhs, const MBString & rhs )
{
	return !(lhs <= rhs);
}

bool operator >= ( const MBString & lhs, const MBString & rhs )
{
	return !(lhs < rhs);
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
