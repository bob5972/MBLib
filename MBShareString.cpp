#include "MBShareString.h"

#include <stdlib.h>
#include <ctype.h>

#include "mjbtypes.h"
#include "mjbdebug.h"
#include "mjbassert.h"

MBShareString::StringRef::StringRef()
:refCount(1),
 str()
{  }

MBShareString::StringRef::StringRef(int size, char fill)
:refCount(1),
 str(size, fill)
{  }

MBShareString::StringRef::StringRef(char x)
:refCount(1),
 str(x)
{  }

MBShareString::StringRef::StringRef(int size)
:refCount(1),
 str(size)
{  }

MBShareString::StringRef::StringRef(const char* cstr)
:refCount(1),
 str(cstr)
{  }

MBShareString::StringRef::StringRef(const MBString& bstr)
:refCount(1),
 str(bstr)
{  }

MBShareString::MBShareString()
:myRef(NULL)
{
	myRef = new StringRef();
}


MBShareString::MBShareString(int size, char fill)
:myRef(NULL)
{
	myRef = new StringRef(size, fill);
}

MBShareString::MBShareString(int size)
:myRef(NULL)
{
	myRef = new StringRef(size);
}

MBShareString::MBShareString(char x)
:myRef(NULL)
{
	myRef = new StringRef(x);
}

//Precondition: c is a null-terminated string, or c = NULL
MBShareString::MBShareString(const char * c)
:myRef(NULL)
{	
	myRef = new StringRef(c);	
}

MBShareString::MBShareString(const MBShareString & str)
:myRef(NULL)
{	
	ASSERT(str.myRef != NULL);
	linkRef(str.myRef);
	ASSERT(myRef != NULL);
}

MBShareString::MBShareString(const MBString & bstr)
:myRef(NULL)
{	
	myRef = new StringRef(bstr);
}

MBShareString::~MBShareString()
{
	unlinkRef();
}


void MBShareString::unlinkRef()
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

void MBShareString::linkRef(StringRef* ref)
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

void MBShareString::copyRef()
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

const MBShareString&  MBShareString::operator = (const MBShareString & str)
{
	if (this != &str) {
		linkRef(str.myRef);
    }
    
    return *this;
}

const MBShareString&  MBShareString::operator = (const MBString & bstr)
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
const MBShareString & MBShareString::operator = (const char* c)

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
const MBShareString & MBShareString::operator = (char c)
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
int MBShareString::length() const
{
	return myRef->str.length();	
}

//return as C-String
const char* MBShareString::cstr() const
{
	return myRef->str.cstr();
}

int MBShareString::find(char ta) const
{
	return myRef->str.find(ta);
}

int MBShareString::find(const MBShareString & mbstr) const
{
	return myRef->str.find(mbstr.myRef->str);
}

MBShareString MBShareString::substr(int pos,int len) const
{
	//you could get some extra mileage out of this
	//  if you converted MBString into a pos,len model
	//  and shared substrs with a single StringRef
	
	ASSERT(len >= 0);
	
	MBShareString oup(len);
	
	oup.myRef->str = myRef->str.substr(pos,len);
	
	return oup;
}

MBShareString MBShareString::toUpper() const
{
	MBShareString oup(length());
	
	oup.myRef->str = myRef->str.toUpper();
	
	return oup;
}

MBShareString MBShareString::toLower() const
{
	MBShareString oup(length());
	
	oup.myRef->str = myRef->str.toLower();
	
	return oup;
}

char MBShareString::operator[ ]( int k ) const
{
	return myRef->str[k];
}

char& MBShareString::operator[ ]( int k )
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
const MBShareString& MBShareString::operator += ( const MBShareString & mbstr )
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
const MBShareString& MBShareString::operator += ( char ch )
{
	myRef->str.append(ch);
	
	return *this;
}

ostream& operator << ( ostream& os, const MBShareString& str )
{
	return os << str.cstr();
}

//precondition:  input stream is open for reading
//postcondition: the next string from input stream is has been read
//               and stored in str

istream& operator >> ( istream& is, MBShareString& str )
{
	//This should really be done with a member function
	// to avoid a string copy, but I was feeling lazy
    MBString bstr;
    
    is >> bstr;
    
    str = bstr;
    
    return is;
}
	
//description:   reads a line from input stream is into the string str
//precondition:  input stream is open for reading
//postcondition: chars from input stream is up to '\n' have been read
istream& getline( istream& is, MBShareString& str )
{

    MBString bstr;
    
    getline(is, bstr);
    
    str = bstr;
    
    return is;
}

int MBShareString::compareTo(const MBShareString& rhs) const
{
	return myRef->str.compareTo(rhs.myRef->str);
}

// comparison operators:
bool operator == ( const MBShareString & lhs, const MBShareString & rhs )
{
	return lhs.compareTo(rhs) == 0;
}

bool operator != ( const MBShareString & lhs, const MBShareString & rhs )
{
	return lhs.compareTo(rhs) != 0;
}

bool operator <  ( const MBShareString & lhs, const MBShareString & rhs )
{
	return lhs.compareTo(rhs) < 0;
}

bool operator <= ( const MBShareString & lhs, const MBShareString & rhs )
{
	return lhs.compareTo(rhs) <= 0;
}
bool operator >  ( const MBShareString & lhs, const MBShareString & rhs )
{
	return lhs.compareTo(rhs) > 0;
}

bool operator >= ( const MBShareString & lhs, const MBShareString & rhs )
{
	return lhs.compareTo(rhs) >= 0;
}

// concatenation operator +
MBShareString operator + ( const MBShareString & lhs, const MBShareString & rhs )
{
	MBShareString oup(lhs.length() + rhs.length());
	
	oup += lhs;
	oup += rhs;
	
	return oup;
}

MBShareString operator + ( char ch, const MBShareString & str )
{
	MBShareString oup(1+str.length());
	oup = ch;
	oup += str;
	return oup;
}

MBShareString operator + ( const MBShareString & str, char ch )
{
	MBShareString oup (1+str.length());
	oup += str;
	oup += ch;
	return oup;
}
