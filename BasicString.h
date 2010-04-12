#ifndef BasicString_H_201001091354
#define BasicString_H_201001091354

#include <istream>
#include <ostream>

using namespace std;

class BasicString
{
    public:
    	//constructs an empty string
    	BasicString();

    	//construct a string of length size, filled with fill
    	BasicString(int size, char fill);

		//constructs an empty string, with the specified buffer size    	
    	explicit BasicString(int size);
        

        //Construct from C-String
        BasicString(const char * s);
       	BasicString(const BasicString & str);
        BasicString(char x);

        ~BasicString();

    	const BasicString & operator = (const BasicString & str);
        const BasicString & operator = (const char* s);
        const BasicString & operator = (char c);

    	//Number of Characters
    	inline int length() const;
   	    
   	    //return as C-String
   	    //The returned string must NOT be freed by the caller
   	    //The returned string may change if the
   	    //    BasicString is modified
   	    const char * cstr() const;
   	    
   	    //returns index of leftmost instance of x
   	    int find(char x) const;
   	    //returns index of leftmost instance of str
   	    int find(const BasicString & str) const;

		//These return new strings
   	    BasicString substr(int pos, int len) const;
   	    BasicString toUpper() const;
   	    BasicString toLower() const;

    	//Range-checked indexing
    	inline char   operator[ ]( int k ) const;
    	inline char getCharAt(int k) const;
     	
     	//Range-checked indexing
     	inline char & operator[ ]( int k );
   		

        //Append str
        void append(const BasicString& str);
        const BasicString & operator += ( const BasicString & str );

        //Append char
        void append(char ch);
        const BasicString & operator += ( char ch );
        
        //like java's
        //  return >0 if this > rhs
        //  return  0 if this == rhs
        //  return <0 if this < rhs
        int compareTo(const BasicString& rhs) const;


    private:
    	int myLength;
    	int myCapacity;
    	char * myChars;
};


//IO Functions
ostream& operator << ( ostream& os, const BasicString& str );
istream& operator >> ( istream& is, BasicString& str );
istream& getline( istream& is, BasicString& str );

// comparison operators:
bool operator == ( const BasicString & lhs, const BasicString & rhs );
bool operator != ( const BasicString & lhs, const BasicString & rhs );
bool operator <  ( const BasicString & lhs, const BasicString & rhs );
bool operator <= ( const BasicString & lhs, const BasicString & rhs );
bool operator >  ( const BasicString & lhs, const BasicString & rhs );
bool operator >= ( const BasicString & lhs, const BasicString & rhs );

// concatenation operator +
BasicString operator + ( const BasicString & lhs, const BasicString & rhs );
BasicString operator + ( char ch, const BasicString & str );
BasicString operator + ( const BasicString & str, char ch );



#endif //BasicString_H_201001091354
