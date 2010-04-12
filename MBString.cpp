#include "MBString.h"

#include <stdlib.h>
#include <ctype.h>

#include "mjbtypes.h"
#include "mjbdebug.h"
#include "mjbassert.h"

MBString::StringRef::StringRef()
:refCount(1),
 str()
{  }

MBString::StringRef::StringRef(int size, char fill)
:refCount(1),
 str(size, fill)
{  }

MBString::StringRef::StringRef(char x)
:refCount(1),
 str(x)
{  }

MBString::StringRef::StringRef(int size)
:refCount(1),
 str(size)
{  }

MBString::StringRef::StringRef(const char* cstr)
:refCount(1),
 str(cstr)
{  }

MBString::StringRef::StringRef(const BasicString& bstr)
:refCount(1),
 str(bstr)
{  }

MBString::MBString()
:myRef(NULL)
{
	myRef = new StringRef();
}


MBString::MBString(int size, char fill)
:myRef(NULL)
{
	myRef = new StringRef(size, fill);
}

MBString::MBString(int size)
:myRef(NULL)
{
	myRef = new StringRef(size);
}

MBString::MBString(char x)
:myRef(NULL)
{
	myRef = new StringRef(x);
}

//Precondition: c is a null-terminated string, or c = NULL
MBString::MBString(const char * c)
:myRef(NULL)
{	
	myRef = new StringRef(c);	
}

MBString::MBString(const MBString & str)
:myRef(NULL)
{	
	ASSERT(str.myRef != NULL);
	linkRef(str.myRef);
	ASSERT(myRef != NULL);
}

MBString::MBString(const BasicString & bstr)
:myRef(NULL)
{	
	myRef = new StringRef(bstr);
}

MBString::~MBString()
{
	unlinkRef();
}


void MBString::unlinkRef()
{
	if(myRef != NULL) {
		ASSERT(myRef->refCount > 0);
		myRef->refCount--;
		if(myRef->refCount == 0) {
			delete myRef;
		}
		myRef = NULL;
	}
}

void MBString::linkRef(StringRef* ref)
{
	if(myRef != NULL) {
		unlinkRef();
	}
	
	myRef = ref;
	if(myRef != NULL) {
		ASSERT(myRef->refCount > 0);
		myRef->refCount++;
		
		//this is an overflow check
		ASSERT(myRef->refCount > 0);
	}
}

void MBString::copyRef()
{
	if(myRef == NULL) {
		myRef = new StringRef();
		return;
	}
	
	ASSERT(myRef->refCount > 0);
	
	if(myRef->refCount == 1) {
		return;
	}
	
	StringRef* temp = new StringRef(myRef->str);
	
	unlinkRef();
	
	myRef = temp;
}

const MBString&  MBString::operator = (const MBString & str)
{
	if (this != &str) {
		linkRef(str.myRef);
    }
    
    return *this;
}

const MBString&  MBString::operator = (const BasicString & bstr)
{
	if(myRef->refCount == 1) {
		//reuse buffer if we have our own
		myRef->str = bstr;
	} else {
		unlinkRef();
		myRef = new StringRef(bstr);
	}
    
    return *this;
}

//Precondition: c points to a null terminated string or c = NULL
const MBString & MBString::operator = (const char* c)

{
	ASSERT(myRef != NULL);
	
	if(myRef->refCount > 1) {
		unlinkRef();
		myRef = new StringRef(c);
	} else {
		ASSERT(myRef->refCount == 1);
		myRef->str = c;
	}
	
	return *this;
}
const MBString & MBString::operator = (char c)
{
	ASSERT(myRef != NULL);
	
	if(myRef->refCount > 1) {
		unlinkRef();
		myRef = new StringRef(c);
	} else {
		ASSERT(myRef->refCount == 1);
		myRef->str = c;
	}
	
	return *this;
}

//Number of logical characters
int MBString::length() const
{
	return myRef->str.length();	
}

//return as C-String
const char* MBString::cstr() const
{
	return myRef->str.cstr();
}

int MBString::find(char ta) const
{
	return myRef->str.find(ta);
}

int MBString::find(const MBString & mbstr) const
{
	return myRef->str.find(mbstr.myRef->str);
}

MBString MBString::substr(int pos,int len) const
{
	//you could get some extra mileage out of this
	//  if you converted BasicString into a pos,len model
	//  and shared substrs with a single StringRef
	
	ASSERT(len >= 0);
	
	MBString oup(len);
	
	oup.myRef->str = myRef->str.substr(pos,len);
	
	return oup;
}

MBString MBString::toUpper() const
{
	MBString oup(length());
	
	oup.myRef->str = myRef->str.toUpper();
}

MBString MBString::toLower() const
{
	MBString oup(length());
	
	oup.myRef->str = myRef->str.toLower();
}

char MBString::operator[ ]( int k ) const
{
	return myRef->str[k];
}

char& MBString::operator[ ]( int k )
{
	ASSERT(myRef->refCount > 0);
	
	if(myRef->refCount == 1) {
		return myRef->str[k];
	} else {
		copyRef();
		return myRef->str[k];
	}
}

//Append str
const MBString& MBString::operator += ( const MBString & mbstr )
{
	//weird case
	if(length() == 0) {
		return ((*this) = mbstr);
	}
	
	if(mbstr.length() > 0) {
		copyRef();
	}
	
	myRef->str.append(mbstr.myRef->str);
	
	return *this;
}
		

//Append char
const MBString& MBString::operator += ( char ch )
{
	myRef->str.append(ch);
	
	return *this;
}

ostream& operator << ( ostream& os, const MBString& str )
{
	return os << str.cstr();
}

//precondition:  input stream is open for reading
//postcondition: the next string from input stream is has been read
//               and stored in str

istream& operator >> ( istream& is, MBString& str )
{
	//This should really be done with a member function
	// to avoid a string copy, but I was feeling lazy
    BasicString bstr;
    
    is >> bstr;
    
    str = bstr;
    
    return is;
}
	
//description:   reads a line from input stream is into the string str
//precondition:  input stream is open for reading
//postcondition: chars from input stream is up to '\n' have been read
istream& getline( istream& is, MBString& str )
{

    BasicString bstr;
    
    getline(is, bstr);
    
    str = bstr;
    
    return is;
}

int MBString::compareTo(const MBString& rhs) const
{
	return myRef->str.compareTo(rhs.myRef->str);
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
	
	oup += lhs;
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
	oup += str;
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
