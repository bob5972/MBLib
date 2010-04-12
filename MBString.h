#ifndef MBSTRING_H_201001091354
#define MBSTRING_H_201001091354

#include <istream>
#include <ostream>

#include "BasicString.h"

using namespace std;

class MBString
{
    public:
    	//Construct an empty string
    	MBString();
    	
    	//constructs an empty string, with the specified buffer size
    	explicit MBString(int size);
    	
    	//construct a string of length size, filled with fill
        MBString(int size, char fill);

        //Construct from C-String
        MBString(const char * s);
        
        MBString(const BasicString & bstr);
       	MBString(const MBString & mbstr);
        MBString(char x);

        ~MBString();


    	const MBString & operator = (const MBString & str);
    	const MBString & operator = (const BasicString & bstr);
        const MBString & operator = (const char* s);
        const MBString & operator = (char c);


   	    //Number of Characters
    	int length() const;

   	    //return as C-String
   	    //The returned string must NOT be freed by the caller
   	    //The returned string may change if the
   	    //    MBString is modified
   	    const char * cstr() const;

   	    //returns index of leftmost instance of x
   	    int find(char x) const;
   	    //returns index of leftmost instance of x
   	    int find(const MBString & str) const;

		//These return new strings
   	    MBString substr(int pos, int len) const;
   	    MBString toUpper() const;
   	    MBString toLower() const;

    	//Range-checked indexing
    	char   operator[ ]( int k ) const;
     	
     	//Range-checked indexing
     	//  Using this will assume you are writing to it
     	//  and trigger a copy if this is a shared reference
     	char & operator[ ]( int k );
   		

        
        //Append str
        const MBString & operator += ( const MBString & str );
        
        //Append char
        const MBString & operator += ( char ch );        
        
        //like java's
        //  return >0 if this > rhs
        //  return  0 if this == rhs
        //  return <0 if this < rhs
        int compareTo(const MBString& rhs) const;


	//Utility Functions
    	static MBString toString(int x);

    private:
    	class StringRef
    	{
    		public:
    			StringRef();
    			StringRef(int size, char fill);
    			StringRef(int size);
    			StringRef(char x);
    			StringRef(const char* cstr);
    			StringRef(const BasicString& bstr);
    		
    			int refCount;
    			BasicString str;
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
ostream& operator << ( ostream& os, const MBString& str );
istream& operator >> ( istream& is, MBString& str );
istream& getline( istream& is, MBString& str );

// comparison operators:
bool operator == ( const MBString & lhs, const MBString & rhs );
bool operator != ( const MBString & lhs, const MBString & rhs );
bool operator <  ( const MBString & lhs, const MBString & rhs );
bool operator <= ( const MBString & lhs, const MBString & rhs );
bool operator >  ( const MBString & lhs, const MBString & rhs );
bool operator >= ( const MBString & lhs, const MBString & rhs );

// concatenation operator +
MBString operator + ( const MBString & lhs, const MBString & rhs );
MBString operator + ( char ch, const MBString & str );
MBString operator + ( const MBString & str, char ch );



#endif //MBSTRING_H_201001091354
