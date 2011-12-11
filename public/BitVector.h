#ifndef BitVector_H_201001192326
#define BitVector_H_201001192326

#include "mbtypes.h"
#include "mbutil.h"
#include "mbassert.h"

#ifdef __cplusplus
	#define BitVector BitVectorData

	extern "C" {
#endif

typedef struct {
	uint32 *bits;
	int size;
	int arrSize;
	bool fill;
} BitVector;

void BitVector_Create(BitVector *b);
void BitVector_Destroy(BitVector *b);
void BitVector_Copy(BitVector *dest, const BitVector *src);

//This is a weird function.
//It empties dest, copies over everything from src,
//  and then leaves src empty.
//Fill is left unchanged in both.
void BitVector_Consume(BitVector *dest, BitVector *src);

void BitVector_Resize(BitVector *b, int size);

//affects the bits from [first..last] inclusive
void BitVector_SetRange(BitVector *b, int first, int last);
void BitVector_ResetRange(BitVector *b, int first, int last);
void BitVector_SetAll(BitVector *b);
void BitVector_ResetAll(BitVector *b);

bool BitVector_GetFillValue(const BitVector *b);
void BitVector_SetFillValue(BitVector *b, bool fill);

/*
 * Inline Functions
 */
 
static INLINE bool
BitVector_GetRaw(int i, const uint32 *bits)
{
	const int UNIT_SIZE = sizeof(*bits)*8;
	
	return (bits[i/UNIT_SIZE] & (1<<(i%UNIT_SIZE))) != 0;
}

static INLINE void
BitVector_SetRaw(int i, uint32 *bits)
{
	const int UNIT_SIZE = sizeof(*bits)*8;
	
	bits[i/UNIT_SIZE] |= (1<<(i%UNIT_SIZE));
}

static INLINE void
BitVector_ResetRaw(int i, uint32 *bits)
{
	const int UNIT_SIZE = sizeof(*bits)*8;
	
	bits[i/UNIT_SIZE] &= ~(1<<(i%UNIT_SIZE));
}


static INLINE bool
BitVector_Get(const BitVector *b, int x)
{
	ASSERT(b != NULL);
	ASSERT(x >= 0);
	ASSERT(x < b->size);
	
	return BitVector_GetRaw(x, b->bits);
}


static INLINE void
BitVector_Put(BitVector *b, int i, bool v)
{
	ASSERT(b != NULL);
	ASSERT(i >= 0);
	ASSERT(i < b->size);
	
	if(v) {
		BitVector_SetRaw(i, b->bits);
	} else {
		BitVector_ResetRaw(i, b->bits);
	}
}

static INLINE void
BitVector_Set(BitVector *b, int i)
{
	ASSERT(b != NULL);
	ASSERT(i >= 0);
	ASSERT(i < b->size);

	BitVector_SetRaw(i, b->bits);
}

static INLINE void
BitVector_Reset(BitVector *b, int i)
{
	ASSERT(b != NULL);
	ASSERT(i >= 0);
	ASSERT(i < b->size);
	
	BitVector_ResetRaw(i, b->bits);
}


static INLINE void
BitVector_Flip(BitVector *b, int i)
{
	ASSERT(b != NULL);
	ASSERT(i >= 0);
	ASSERT(i < b->size);
	
	bool value = BitVector_GetRaw(i, b->bits);
	
	if (value) {
		BitVector_ResetRaw(i, b->bits);
	} else {
		BitVector_SetRaw(i, b->bits);
	}
}

static INLINE void
BitVector_MakeEmpty(BitVector *b)
{
	ASSERT(b != NULL);
	b->size = 0;
}

static INLINE int
BitVector_Size(const BitVector *b)
{
	ASSERT(b != NULL);
	return b->size;
}

#ifdef __cplusplus
	}
	
	#undef BitVector

class BitVector
{
	public:
		BitVector() {
			BitVector_Create(&b);
		}
		
		BitVector(const BitVector &a) {
			BitVector_Create(&b);
			BitVector_Copy(&b, &a.b);
		}
		
		explicit BitVector(int size) {
			BitVector_Create(&b);
			BitVector_Resize(&b, size);
		}
		
		BitVector(int size, bool initial) {
			BitVector_Create(&b);
			BitVector_SetFillValue(&b, initial);
			BitVector_Resize(&b, size);
		}
		
		~BitVector() {
			BitVector_Destroy(&b);
		}
		
		bool getFillValue() const {
			return BitVector_GetFillValue(&b);
		}
		
		void setFillValue(bool f) {
			BitVector_SetFillValue(&b, f);
		}
		
		bool get(int x) const {
			return BitVector_Get(&b, x);
		}
		
		void put(int x, bool v) {
			BitVector_Put(&b, x, v);
		}
		
		void set(int x) {
			BitVector_Set(&b, x);
		}
		
		void reset(int x) {
			BitVector_Reset(&b, x);
		}
		
		void flip(int x) {
			BitVector_Flip(&b, x);
		}
		
		void setRange(int first, int last) {
			BitVector_SetRange(&b, first, last);
		}
		
		void resetRange(int first, int last) {
			BitVector_ResetRange(&b, first, last);
		}
		
		void setAll() {
			BitVector_SetAll(&b);
		}
		
		void resetAll() {
			BitVector_ResetAll(&b);
		}
		
		int size() const {
			return BitVector_Size(&b);
		}
		
		void resize(int size) {
			BitVector_Resize(&b, size);
		}
		
		void makeEmpty() {
			BitVector_MakeEmpty(&b);
		}
		
		bool operator [] (int x) const {
			return BitVector_Get(&b, x);
		}
		

		void consume(BitVector &src) {
			BitVector_Consume(&b, &src.b);
		}
	
	private:
		BitVectorData b;
};

#endif //__cplusplus


#endif //BitVector_H_201001192326
