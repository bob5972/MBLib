#ifndef MBString_H_201001091354
#define MBString_H_201001091354

#include <string.h>
#include <istream>
#include <ostream>

#include "mbassert.h"
#include "mbutil.h"

using namespace std;

class MBString
{
    public:
    	//constructs an empty string
    	MBString()
    	:myLength(0),
		myCapacity(8),
		myChars(new char[myCapacity])
		{
		    myChars[myLength] = '\0';
		}

    	//construct a string of length size, filled with fill
    	MBString(int size, char fill)
    	:myLength(size),
		myCapacity(size + 1),
		myChars(new char[myCapacity])
		{
			memset(myChars, fill, myLength);
			myChars[myLength] = '\0';
		}

		//constructs an empty string, with the specified buffer size    	
    	explicit MBString(int size)
    	:myLength(size),
		myCapacity(size + 1),
		myChars(new char[myCapacity])
		{
			myChars[myLength] = '\0';
		}
        

        //Construct from C-String
        MBString(const char * c)
        :myLength(0),
		myCapacity(0),
		myChars(null)
		{	
			if (c) {
				myLength = strlen(c);
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
	
       	MBString(const MBString & str)
   	    :myLength(str.myLength),
		myCapacity(myLength+1),
		myChars(new char[myCapacity])
		{
			memcpy(myChars, str.myChars, str.myLength);
			myChars[myLength] = '\0';
		}

       	MBString(char x)
       	:myLength(1),
		myCapacity(8),
		myChars(new char[myCapacity])
		{
			myChars[0] = x;
		    myChars[myLength] = '\0';
		}

        ~MBString() {
        	delete [] myChars;
        	myChars = NULL;
    	}

        const MBString& operator = (const MBString & str)
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
        const MBString& operator = (const char* c)
        {
			int newLength;
			if (c) {
				newLength = strlen(c);
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

        const MBString & operator = (char c)
        {
			myLength = 0;
			ensureCapacity(2);
			myLength = 1;
			myChars[0] = c;
			myChars[myLength] = '\0';
			return *this;
		}

    	//Number of Characters
    	int length() const {
    		return myLength;
		}
   	    
   	    //return as C-String
   	    //The returned string must NOT be freed by the caller
   	    //The returned string may change if the
   	    //    MBString is modified
   	    const char * cstr() const {
   	    	return myChars;
    	}
   	    
   	    //returns index of leftmost instance of x
   	    int find(char x) const;
   	    //returns index of leftmost instance of str
   	    int find(const MBString & str) const;

		//These return new strings
		
		/*
		 * return len chars starting at pos
		 */
   	    MBString substr(int pos, int len) const;
   	    MBString toUpper() const;
   	    MBString toLower() const;

    	//indexing
    	char operator[ ]( int k ) const {
    		return getCharAt(k);
		}
		
    	char getCharAt(int k) const {
			ASSERT(k < myLength);
			ASSERT(k >= 0);	
			return myChars[k];
		}
     	
     	//Range-checked indexing
     	char & operator[ ]( int k ) {
     		ASSERT(k < myLength);
			ASSERT(k >= 0);	
			return myChars[k];
		}
   		

        //Append str
        void append(const MBString& str)
		{
			ensureCapacity(myLength + str.myLength + 1);
	
			memcpy(&myChars[myLength], str.myChars, str.myLength);
			myLength = myLength + str.myLength;
			myChars[myLength] = '\0';
		}

        const MBString & operator += ( const MBString & str )
		{
			append(str);	
			return *this;
		}


        //Append char
        void append(char ch)
		{
			ensureCapacity(myLength + 2);
			myChars[myLength] = ch;
			myLength++;
			myChars[myLength] = '\0';
		}


        const MBString & operator += ( char ch )
        {
			append(ch);
			return *this;
		}
        
        //like java's
        //  return >0 if this > rhs
        //  return  0 if this == rhs
        //  return <0 if this < rhs
        int compareTo(const MBString& rhs) const;
        
    //Utility Functions
        static MBString toString(int x);

    private:
    	int myLength;
    	int myCapacity;
    	char * myChars;
    	
    	void ensureCapacity(int cap);
};


//IO Functions
static INLINE ostream& operator << ( ostream& os,
                                     const MBString& str )
{
	return os << str.cstr();
}


istream& operator >> ( istream& is, MBString& str );
istream& getline( istream& is, MBString& str );

// comparison operators:
static INLINE bool operator == ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compareTo(rhs) == 0;
}

static INLINE bool operator != ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compareTo(rhs) != 0;
}

static INLINE bool operator <  ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compareTo(rhs) < 0;
}

static INLINE bool operator <= ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compareTo(rhs) <= 0;
}

static INLINE bool operator >  ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compareTo(rhs) > 0;
}

static INLINE bool operator >= ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compareTo(rhs) >= 0;
}

// concatenation operator +
static INLINE MBString operator + ( const MBString & lhs,
                                    const MBString & rhs )
{
	MBString oup(lhs.length() + rhs.length());
	oup = lhs;
	oup += rhs;
	return oup;
}

static INLINE MBString operator + ( char ch,
                                    const MBString & str )
{
	MBString oup(1+str.length());
	oup = ch;
	oup += str;
	return oup;
}

static INLINE MBString operator + ( const MBString & str,
                                    char ch )
{
	MBString oup (1+str.length());
	oup = str;
	oup += ch;
	return oup;
}

#endif //MBString_H_201001091354
