#ifndef BitVector_H_201001192326
#define BitVector_H_201001192326

#include "mbtypes.h"
#include "mbutil.h"
#include "mbassert.h"
#include <string.h>

#ifdef __cplusplus
	#define BitVector BitVectorData

	extern "C" {
#endif

typedef struct {
	uint64 *bits;
	uint size;
	uint arrSize;
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
void BitVector_FlipRange(BitVector *b, int first, int last);

int BitVector_PopCount(const BitVector *b);

/*
 * Inline Functions
 */
#define BVUNITBITS 64
#define BVINDEX(x) (x / BVUNITBITS)
#define BVMASK(x)  ( ((uint64)1) << (x % BVUNITBITS))
 
static INLINE bool
BitVector_GetRaw(int i, const uint64 *bits)
{
#if defined(__GNUC__) && (defined(ARCH_AMD64) || defined(ARCH_x86))
	if (!CONSTANT(i)) {
		uint32 tmp;

		asm("btl  %2, (%1); "
		    "sbbl %0, %0"
		    : "=r" (tmp)
		    : "r" (bits), "r" (i)
		    : "cc");
		return tmp;
	}
#endif

	return (bits[BVINDEX(i)] & BVMASK(i)) != 0;	
}

static INLINE void
BitVector_SetRaw(int i, uint64 *bits)
{
#if defined(__GNUC__) && (defined(ARCH_AMD64) || defined(ARCH_x86))
	if (!CONSTANT(i)) {
		asm volatile("btsl %1, (%0)"
    	             :: "r" (bits), "r" (i)
    	             : "cc", "memory");
     
     	return;
     }
#endif	
	bits[BVINDEX(i)] |= BVMASK(i);
}

static INLINE void
BitVector_ResetRaw(int i, uint64 *bits)
{
#if defined(__GNUC__) && (defined(ARCH_AMD64) || defined(ARCH_x86))
	if (!CONSTANT(i)) {
		asm volatile("btrl %1, (%0)"
	                 :: "r" (bits), "r" (i)
	                 : "cc", "memory");
         return;
     }
#endif
	bits[BVINDEX(i)] &= ~BVMASK(i);
}

static INLINE void
BitVector_FlipRaw(int i, uint64 *bits)
{	
#if defined(__GNUC__) && (defined(ARCH_AMD64) || defined(ARCH_x86))
	if (!CONSTANT(i)) {
		asm volatile("btcl %1, (%0)"
	                 :: "r" (bits), "r" (i)
	                 : "cc", "memory");
         
		return;
	}
#endif
	bits[BVINDEX(i)] ^= BVMASK(i);	
}


static INLINE bool
BitVector_Get(const BitVector *b, int x)
{
	ASSERT(b != NULL);
	ASSERT(x >= 0);
	ASSERT((uint) x < b->size);
	
	return BitVector_GetRaw(x, b->bits);
}


static INLINE void
BitVector_Put(BitVector *b, int x, bool v)
{
	ASSERT(b != NULL);
	ASSERT(x >= 0);
	ASSERT((uint) x < b->size);
	
	if (v) {
		BitVector_SetRaw(x, b->bits);
	} else {
		BitVector_ResetRaw(x, b->bits);
	}
}

static INLINE void
BitVector_Set(BitVector *b, int x)
{
	ASSERT(b != NULL);
	ASSERT(x >= 0);
	ASSERT((uint) x < b->size);

	BitVector_SetRaw(x, b->bits);
}

static INLINE void
BitVector_Reset(BitVector *b, int x)
{
	ASSERT(b != NULL);
	ASSERT(x >= 0);
	ASSERT((uint) x < b->size);
	
	BitVector_ResetRaw(x, b->bits);
}


static INLINE void
BitVector_Flip(BitVector *b, int x)
{
	ASSERT(b != NULL);
	ASSERT(x >= 0);
	ASSERT((uint) x < b->size);
	
	BitVector_FlipRaw(x, b->bits);
}

static INLINE bool
BitVector_TestAndSet(BitVector *b, int x)
{
	bool oup;
	ASSERT(b != NULL);
	ASSERT(x >= 0);
	ASSERT((uint) x < b->size);

	oup = BitVector_GetRaw(x, b->bits);
	BitVector_SetRaw(x, b->bits);
	return oup;
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


static INLINE void
BitVector_SetAll(BitVector *b)
{
	ASSERT(b != NULL);
	uint32 byteLength;
	
	byteLength = (b->size + 7) / 8;
	ASSERT(byteLength / sizeof(b->bits[0]) <= b->arrSize);
	
	memset(b->bits, 0xFF, byteLength);
}

static INLINE void
BitVector_ResetAll(BitVector *b)
{
	ASSERT(b != NULL);
	uint32 byteLength;
	
	byteLength = (b->size + 7) / 8;
	ASSERT(byteLength / sizeof(b->bits[0]) <= b->arrSize);
	
	memset(b->bits, 0x00, byteLength);
}

static INLINE void
BitVector_FlipAll(BitVector *b)
{
	ASSERT(b != NULL);
	uint32 validCellCount;
	uint x;
	
	validCellCount = b->size / sizeof(b->bits[0]);
	validCellCount++;
	ASSERT(validCellCount < b->arrSize);
	
	for (x = 0; x < validCellCount; x++) {
		b->bits[x] = ~b->bits[x];
	}
}

static INLINE bool 
BitVector_GetFillValue(const BitVector *b)
{
	ASSERT(b != NULL);
	return b->fill;
}

static INLINE void
BitVector_SetFillValue(BitVector *b, bool f)
{
	ASSERT(b != NULL);
	b->fill = f;
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
		
		bool testAndSet(int x) {
			return BitVector_TestAndSet(&b, x);
		}
		
		void setRange(int first, int last) {
			BitVector_SetRange(&b, first, last);
		}
		
		void resetRange(int first, int last) {
			BitVector_ResetRange(&b, first, last);
		}
		
		void flipRange(int first, int last) {
			BitVector_FlipRange(&b, first, last);
		}
		
		void setAll() {
			BitVector_SetAll(&b);
		}
		
		void resetAll() {
			BitVector_ResetAll(&b);
		}
		
		void flipAll() {
			BitVector_FlipAll(&b);
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
		
		int popcount() const {
			return BitVector_PopCount(&b);
		}
	
	private:
		BitVectorData b;
};

#endif //__cplusplus


#endif //BitVector_H_201001192326
