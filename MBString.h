#ifndef MBSTRING_H_201001091354
#define MBSTRING_H_201001091354

#include <iostream.h>

class MBString
{
    public:
    //Constructors
    	
    	MBString();
    	
        MBString(int size, char fill);        
        //Construct from C-String
        MBString(const char * s);        
       	MBString(const MBString & str);
        MBString(char x);
	//Destructor
        ~MBString();
        
    
    //Assignment
    	const MBString & operator = (const MBString & str);
        const MBString & operator = (const char* s);
        const MBString & operator = (char c);
    
    //Accessors
    	int length() const;
   	    //Number of Characters
   	    const char * cstr() const;
   	    //return as C-String
   	    //returns index of leftmost instance of str
   	    int find(char x) const;
   	    int find(const MBString & str) const;
   	    
		//These return new strings
   	    MBString substr(int pos, int len) const;
   	    MBString toUpper() const;
   	    MBString toLower() const;
   	    
    //Indexing
    	char   operator[ ]( int k ) const;
     	//Range-checked indexing
     	char & operator[ ]( int k );
   		//Range-checked indexing
  		
    //Modifiers
        const MBString & operator += ( const MBString & str );
        //Append str
        const MBString & operator += ( char ch );
        //Append char
    
    private:
    	//size-only constructor
    	explicit MBString(int size); 
    	
    	int myLength;
    	int myCapacity;
    	char * myChars;
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
