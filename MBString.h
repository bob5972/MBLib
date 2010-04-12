#ifndef MBString_H_201001091354
#define MBString_H_201001091354

#include <istream>
#include <ostream>

using namespace std;

class MBString
{
    public:
    	//constructs an empty string
    	MBString();

    	//construct a string of length size, filled with fill
    	MBString(int size, char fill);

		//constructs an empty string, with the specified buffer size    	
    	explicit MBString(int size);
        

        //Construct from C-String
        MBString(const char * s);
       	MBString(const MBString & str);
        MBString(char x);

        ~MBString();

    	const MBString & operator = (const MBString & str);
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
   	    //returns index of leftmost instance of str
   	    int find(const MBString & str) const;

		//These return new strings
   	    MBString substr(int pos, int len) const;
   	    MBString toUpper() const;
   	    MBString toLower() const;

    	//Range-checked indexing
    	char   operator[ ]( int k ) const;
    	char getCharAt(int k) const;
     	
     	//Range-checked indexing
     	char & operator[ ]( int k );
   		

        //Append str
        void append(const MBString& str);
        const MBString & operator += ( const MBString & str );

        //Append char
        void append(char ch);
        const MBString & operator += ( char ch );
        
        //like java's
        //  return >0 if this > rhs
        //  return  0 if this == rhs
        //  return <0 if this < rhs
        int compareTo(const MBString& rhs) const;


    private:
    	int myLength;
    	int myCapacity;
    	char * myChars;
};


//IO Functions
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



#endif //MBString_H_201001091354
