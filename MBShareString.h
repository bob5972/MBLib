#ifndef MBShareString_H_201001091354
#define MBShareString_H_201001091354

#include <istream>
#include <ostream>

#include "MBString.h"

using namespace std;

class MBShareString
{
    public:
    	//Construct an empty string
    	MBShareString();
    	
    	//constructs an empty string, with the specified buffer size
    	explicit MBShareString(int size);
    	
    	//construct a string of length size, filled with fill
        MBShareString(int size, char fill);

        //Construct from C-String
        MBShareString(const char * s);
        
        MBShareString(const MBString & bstr);
       	MBShareString(const MBShareString & mbstr);
        MBShareString(char x);

        ~MBShareString();


    	const MBShareString & operator = (const MBShareString & str);
    	const MBShareString & operator = (const MBString & bstr);
        const MBShareString & operator = (const char* s);
        const MBShareString & operator = (char c);


   	    //Number of Characters
    	int length() const;

   	    //return as C-String
   	    //The returned string must NOT be freed by the caller
   	    //The returned string may change if the
   	    //    MBShareString is modified
   	    const char * cstr() const;

   	    //returns index of leftmost instance of x
   	    int find(char x) const;
   	    //returns index of leftmost instance of x
   	    int find(const MBShareString & str) const;

		//These return new strings
   	    MBShareString substr(int pos, int len) const;
   	    MBShareString toUpper() const;
   	    MBShareString toLower() const;

    	//Range-checked indexing
    	char   operator[ ]( int k ) const;
     	
     	//Range-checked indexing
     	//  Using this will assume you are writing to it
     	//  and trigger a copy if this is a shared reference
     	char & operator[ ]( int k );
   		

        
        //Append str
        const MBShareString & operator += ( const MBShareString & str );
        
        //Append char
        const MBShareString & operator += ( char ch );        
        
        //like java's
        //  return >0 if this > rhs
        //  return  0 if this == rhs
        //  return <0 if this < rhs
        int compareTo(const MBShareString& rhs) const;

    private:
    	class StringRef
    	{
    		public:
    			StringRef();
    			StringRef(int size, char fill);
    			StringRef(int size);
    			StringRef(char x);
    			StringRef(const char* cstr);
    			StringRef(const MBString& bstr);
    		
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
ostream& operator << ( ostream& os, const MBShareString& str );
istream& operator >> ( istream& is, MBShareString& str );
istream& getline( istream& is, MBShareString& str );

// comparison operators:
bool operator == ( const MBShareString & lhs, const MBShareString & rhs );
bool operator != ( const MBShareString & lhs, const MBShareString & rhs );
bool operator <  ( const MBShareString & lhs, const MBShareString & rhs );
bool operator <= ( const MBShareString & lhs, const MBShareString & rhs );
bool operator >  ( const MBShareString & lhs, const MBShareString & rhs );
bool operator >= ( const MBShareString & lhs, const MBShareString & rhs );

// concatenation operator +
MBShareString operator + ( const MBShareString & lhs, const MBShareString & rhs );
MBShareString operator + ( char ch, const MBShareString & str );
MBShareString operator + ( const MBShareString & str, char ch );



#endif //MBShareString_H_201001091354
