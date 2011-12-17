#ifndef MBShareString_H_201001091354
#define MBShareString_H_201001091354

#include <istream>
#include <ostream>

#include "MBString.h"
#include "mbutil.h"

using namespace std;

//This is an experimental attempt to use shared references
// between strings.  It has not been thoroughly tested,
// and preliminary tests show it to be slower than the
// original (at least for an mbc test suite).

class MBShareString
{
    public:
    	//Construct an empty string
    	MBShareString()
    	:myRef(NULL)
		{
			myRef = new StringRef();
		}

        MBShareString(int size, char fill)
        :myRef(NULL)
		{
			myRef = new StringRef(size, fill);
		}

        MBShareString(int size)
        :myRef(NULL)
        {
			myRef = new StringRef(size);
		}

        MBShareString(char x)
        :myRef(NULL)
		{
			myRef = new StringRef(x);
		}

	    //Precondition: c is a null-terminated string, or c = NULL
        MBShareString(const char * c)
	    :myRef(NULL)
	    {	
	    	myRef = new StringRef(c);	
	    }

        MBShareString(const MBShareString & str)
        :myRef(NULL)
  	    {	
	    	ASSERT(str.myRef != NULL);
		    linkRef(str.myRef);
		    ASSERT(myRef != NULL);
	    }

        MBShareString(const MBString & bstr)
        :myRef(NULL) 
	    {	
	         myRef = new StringRef(bstr);
	    }

	    ~MBShareString()
        {
	         unlinkRef();
        }

        const MBShareString&  operator = (const MBShareString & str)
        {
			if (this != &str) {
				linkRef(str.myRef);
			}
			
			return *this;
		}

        const MBShareString&  operator = (const MBString & bstr)
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
        const MBShareString & operator = (const char* c)
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
        
        const MBShareString & operator = (char c)
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
        int length() const
		{
			return myRef->str.length();	
		}

        //return as C-String
        const char* cstr() const
		{
			return myRef->str.cstr();
		}
        
        int find(char ta) const
		{
			return myRef->str.find(ta);
		}
        
        int find(const MBShareString & mbstr) const
		{
			return myRef->str.find(mbstr.myRef->str);
		}

        MBShareString substr(int pos,int len) const
		{
			//you could get some extra mileage out of this
			//  if you converted MBString into a pos,len model
			//  and shared substrs with a single StringRef
			
			ASSERT(len >= 0);
			
			MBShareString oup(len);
			
			oup.myRef->str = myRef->str.substr(pos,len);
			
			return oup;
		}
        
        MBShareString toUpper() const
		{
			MBShareString oup(length());
			
			oup.myRef->str = myRef->str.toUpper();
			
			return oup;
		}
        
        MBShareString toLower() const
		{
			MBShareString oup(length());
			
			oup.myRef->str = myRef->str.toLower();
			
			return oup;
		}
        
        char operator[ ]( int k ) const
		{
			return myRef->str[k];
		}
     	//Range-checked indexing
     	//  Using this will assume you are writing to it
     	//  and trigger a copy if this is a shared reference
     	char & operator[ ]( int k );
   		
        //Append str
        const MBShareString & operator += ( const MBShareString & str );
        
        //Append char
        const MBShareString& operator += ( char ch )
		{
			myRef->str.append(ch);
			
			return *this;
		}


        
        //like java's
        //  return >0 if this > rhs
        //  return  0 if this == rhs
        //  return <0 if this < rhs
        int compareTo(const MBShareString& rhs) const
		{
			return myRef->str.compareTo(rhs.myRef->str);
		}

    private:
    	class StringRef
    	{
    		public:
    			StringRef()
    			:refCount(1),
    			str()
				{  }

    			StringRef(int size, char fill)
    			:refCount(1),
    			str(size, fill)
				{  }

    			StringRef(char x)
    			:refCount(1),
    			str(x)
				{  }

		        StringRef(int size)
		        :refCount(1),
                str(size)
				{  }

                StringRef(const char* cstr)
                :refCount(1),
                str(cstr)
                {  }

    			StringRef(const MBString& bstr)
    			:refCount(1),
    			str(bstr)
				{  }
    		
    			int refCount;
    			MBString str;
    	};
    	
    	//decrements the ref count, and sets myRef to NULL
    	//  deletes it if necessary
		void unlinkRef();

		//increments the ref count, and sets myRef to ref
		void linkRef(StringRef* ref);
		
		//Creates a new reference if necessary
		// (ie, if we need to COW)
		void copyRef();
    	
    	StringRef* myRef;
    	
};

//IO Functions

//IO Member Functions
static INLINE ostream& operator << ( ostream& os,
                                     const MBShareString& str )
{
	return os << str.cstr();
}

//precondition:  input stream is open for reading
//postcondition: the next string from input stream is has been read
//               and stored in str

static INLINE istream& operator >> ( istream& is,
                                     MBShareString& str )
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
static INLINE istream& getline( istream& is,
                                MBShareString& str )
{

    MBString bstr;
    
    getline(is, bstr);
    
    str = bstr;
    
    return is;
}

// comparison operators:
static INLINE bool operator == ( const MBShareString & lhs,
                                 const MBShareString & rhs )
{
	return lhs.compareTo(rhs) == 0;
}

static INLINE bool operator != ( const MBShareString & lhs,
                                 const MBShareString & rhs )
{
	return lhs.compareTo(rhs) != 0;
}

static INLINE bool operator <  ( const MBShareString & lhs,
                                 const MBShareString & rhs )
{
	return lhs.compareTo(rhs) < 0;
}

static INLINE bool operator <= ( const MBShareString & lhs,
                                 const MBShareString & rhs )
{
	return lhs.compareTo(rhs) <= 0;
}

static INLINE bool operator >  ( const MBShareString & lhs,
                                 const MBShareString & rhs )
{
	return lhs.compareTo(rhs) > 0;
}

static INLINE bool operator >= ( const MBShareString & lhs,
                                 const MBShareString & rhs )
{
	return lhs.compareTo(rhs) >= 0;
}


// concatenation operator +
static INLINE MBShareString operator + ( const MBShareString & lhs,
                                         const MBShareString & rhs )
{
	MBShareString oup(lhs.length() + rhs.length());
	
	oup += lhs;
	oup += rhs;
	
	return oup;
}

static INLINE MBShareString operator + ( char ch,
                                         const MBShareString & str )
{
	MBShareString oup(1+str.length());
	oup = ch;
	oup += str;
	return oup;
}

static INLINE MBShareString operator + ( const MBShareString & str,
                                         char ch )
{
	MBShareString oup (1+str.length());
	oup += str;
	oup += ch;
	return oup;
}


#endif //MBShareString_H_201001091354
