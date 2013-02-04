#ifndef MBString_H_201001091354
#define MBString_H_201001091354

#include <string.h>
#include <stdlib.h>

#include "mbassert.h"
#include "mbutil.h"

#ifdef __cplusplus

    #include <istream>
    #include <ostream>
    #define MBString MBStringData

    extern "C" {
#endif

typedef struct {
    int length;
    int capacity;
    char *chars;
} MBString;

void MBString_Create(MBString *str);
void MBString_Destroy(MBString *str);
void MBString_Resize(MBString *str, int size);

void MBString_Copy(MBString *dest, const MBString *src);
void MBString_CopyCStr(MBString *dest, const char *cstr);
int MBString_Length(const MBString *str);
bool MBString_IsEmpty(const MBString *str);
const char *MBString_GetCStr(const MBString *str);

int MBString_FindChar(const MBString *str, char x);
int MBString_FindStr(const MBString *str, const MBString *substr);

void MBString_CopySubstr(MBString *dest, const MBString *str,
                         int start, int len);

void MBString_ToUpper(MBString *str);
void MBString_ToLower(MBString *str);

char MBString_GetChar(const MBString *str, int x);
void MBString_SetChar(MBString *str, int x, char c);
void MBString_FillChar(MBString *str, char c, int pos, int len);

void MBString_AppendChar(MBString *str, char c);
void MBString_AppendStr(MBString *str, const MBString *suffix);
void MBString_PrependChar(MBString *str, char c);
void MBString_PrependStr(MBString *str, const MBString *prefix);
void MBString_Consume(MBString *consumer, MBString *strData);

int MBString_Compare(const MBString *lhs, const MBString *rhs);
void MBString_IntToString(MBString *str, int x);

#ifdef __cplusplus
    } // extern "C"

    #undef MBString

class MBString
{
    public:
    	//constructs an empty string
    	MBString()
        {
    	    MBString_Create(&data);
        }

    	//construct a string of length size, filled with fill
    	MBString(int size, char fill)
    	{
    	    MBString_Create(&data);
    	    MBString_Resize(&data, size);
    	    MBString_FillChar(&data, fill, 0, size);
    	}

        //Construct from C-String
        MBString(const char * c)
        {
            MBString_Create(&data);
            MBString_CopyCStr(&data, c);
        }

       	MBString(const MBString & str)
       	{
       	    MBString_Create(&data);
       	    MBString_Copy(&data, &str.data);
       	}

       	MBString(char c)
       	{
       	    MBString_Create(&data);
       	    MBString_AppendChar(&data, c);
       	}

        ~MBString()
        {
            MBString_Destroy(&data);
        }

        const MBString& operator = (const MBString & str) {
            if (this != &str) {
                MBString_Copy(&data, &str.data);
            }
            return *this;
        }

        const MBString& operator = (const char* cstr)
        {
            MBString_CopyCStr(&data, cstr);
            return *this;
        }

        const MBString & operator = (char c)
        {
            MBString_Resize(&data, 1);
            MBString_SetChar(&data, 0, c);
            return *this;
        }

    	int length() const
    	{
    	    return MBString_Length(&data);
    	}

        bool isEmpty() const
        {
            return MBString_IsEmpty(&data);
        }


        /*
         * Return this string as a null-terminated C-String.
         *
         * The returned string must NOT be freed by the caller.
         * The returned string is not guaranteed to be valid if the
         * string is modified.
         */
        const char * CStr() const
        {
            return MBString_GetCStr(&data);
        }


        int find(char c) const
        {
            return MBString_FindChar(&data, c);
        }

        int find(const MBString & substr) const
        {
            return MBString_FindStr(&data, &substr.data);
        }

        /*
         * return len chars starting at pos
         */
        MBString substr(int pos, int len) const
        {
            MBString oup;

            MBString_CopySubstr(&oup.data, &data, pos, len);
            return oup;
        }

        MBString toUpper() const
        {
            MBString oup;

            MBString_Copy(&oup.data, &data);
            MBString_ToUpper(&oup.data);
            return oup;
        }

        MBString toLower() const
        {
            MBString oup;

            MBString_Copy(&oup.data, &data);
            MBString_ToLower(&oup.data);
            return oup;
        }

    	char getChar(int k) const {
    	    return MBString_GetChar(&data, k);
        }

    	void setChar(int k, char c) {
            MBString_SetChar(&data, k, c);
        }

        void append(const MBString& str)
        {
            MBString_AppendStr(&data, &str.data);
        }

        void prepend(const MBString &str)
        {
            MBString_PrependStr(&data, &str.data);
        }


        void consume(MBString &str)
        {
            MBString_Consume(&data, &str.data);
        }

        const MBString & operator += ( const MBString & str )
        {
                append(str);
                return *this;
        }


        //Append char
        void append(char c)
        {
            MBString_AppendChar(&data, c);
        }

        const MBString & operator += ( char c )
        {
            append(c);
            return *this;
        }

        int compare(const MBString &rhs) const
        {
            return MBString_Compare(&data, &rhs.data);
        }

        static MBString toString(int x)
        {
            MBString oup;

            MBString_Create(&oup.data);
            MBString_IntToString(&oup.data, x);
            return oup;
        }

    private:
    	MBStringData data;
};


//IO Functions
static INLINE std::ostream& operator << ( std::ostream& os,
                                          const MBString& str )
{
	return os << str.CStr();
}


std::istream& operator >> ( std::istream& is, MBString& str );
std::istream& getline( std::istream& is, MBString& str );

static INLINE bool
MBString_GetLine( std::istream& is, MBString &str)
{
	getline(is, str);
	return !str.isEmpty();
}


// comparison operators:
static INLINE bool operator == ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compare(rhs) == 0;
}

static INLINE bool operator != ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compare(rhs) != 0;
}

static INLINE bool operator <  ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compare(rhs) < 0;
}

static INLINE bool operator <= ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compare(rhs) <= 0;
}

static INLINE bool operator >  ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compare(rhs) > 0;
}

static INLINE bool operator >= ( const MBString & lhs,
                                 const MBString & rhs )
{
	return lhs.compare(rhs) >= 0;
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

#endif // __cplusplus

#endif //MBString_H_201001091354
