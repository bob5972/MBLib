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

typedef enum {
	BITVECTOR_WRITE_RESET,
	BITVECTOR_WRITE_SET,
	BITVECTOR_WRITE_FLIP,
} BitVectorWriteType;

void BitVector_Create(BitVector *b);
void BitVector_Destroy(BitVector *b);
void BitVector_Copy(BitVector *dest, const BitVector *src);

//This is a weird function.
//It empties dest, copies over everything from src,
//  and then leaves src empty.
//Fill is left unchanged in both.
void BitVector_Consume(BitVector *dest, BitVector *src);

void BitVector_Resize(BitVector *b, int size);

// Helper function for fills
void BitVectorSetRangeGeneric(BitVector *b, int first, int last);
void BitVectorResetRangeGeneric(BitVector *b, int first, int last);
void BitVectorFlipRangeGeneric(BitVector *b, int first, int last);

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

static INLINE void
BitVectorWrite(BitVector *b, int x, BitVectorWriteType type)
{
	switch (type) {
		case BITVECTOR_WRITE_SET:
			BitVector_Set(b, x);
			break;
		case BITVECTOR_WRITE_RESET:
			BitVector_Reset(b, x);
			break;
		case BITVECTOR_WRITE_FLIP:
			BitVector_Flip(b, x);
			break;
	}
}

static INLINE void
BitVectorWriteRangeGenericDispatch(BitVector *b,
                                   int first, int last,
                                   BitVectorWriteType type)
{
	switch (type) {
		case BITVECTOR_WRITE_SET:
			BitVectorSetRangeGeneric(b, first, last);
			break;
		case BITVECTOR_WRITE_RESET:
			BitVectorResetRangeGeneric(b, first, last);
			break;
		case BITVECTOR_WRITE_FLIP:
			BitVectorFlipRangeGeneric(b, first, last);
			break;
	}
}

static INLINE void
BitVectorWriteRangeOptimized(BitVector *b,
                             int first, int last,
                             BitVectorWriteType type)
{
	const uint8 alignment = 8;
	
	ASSERT(b != NULL);
	ASSERT(first % alignment == 0);
	ASSERT(last % alignment == (alignment - 1));
	ASSERT(last != first);
	
	int x;
	uint8 fillByte;
	uint8 *myBytes;
	int numBytes;
	uint32 startByte;
	
	numBytes = (last - first) / alignment + 1;
	myBytes = (uint8 *) b->bits;
	startByte = first / alignment;
	myBytes += startByte;
	
	ASSERT(startByte + numBytes <= b->arrSize * sizeof(b->bits[0]));
	
	switch (type) {
		case BITVECTOR_WRITE_SET:
		case BITVECTOR_WRITE_RESET:	
			if (type == BITVECTOR_WRITE_SET) {
				fillByte = 0xFF;
			} else {
				fillByte = 0x00;
			}
			
			memset(myBytes, fillByte, numBytes);
			break;
		case BITVECTOR_WRITE_FLIP:
			for (x = 0; x < numBytes; x++) {
				myBytes[x] = ~myBytes[x];
			}
			break;
	}
}

static INLINE void
BitVectorWriteRangeGenericImpl(BitVector *b,
                               int first, int last,
                               BitVectorWriteType type)
{
	ASSERT(b != NULL);
	ASSERT(first >= 0);
	ASSERT((uint) first < b->size);
	ASSERT(last >= 0);
	ASSERT((uint) last < b->size);
	ASSERT(first <= last);
	
	int x;
	const uint8 alignment = 8;
	int alignedLast;
	int alignedFirst;
	
	if (last - first + 1 <= alignment * 2) {
		for (x = first; x <= last; x++) {
			BitVectorWrite(b, x, type);
		}
		return;
	}	

	if (first % alignment != 0) {
		alignedFirst = first + (alignment - (first % alignment));
		ASSERT(first <= alignedFirst);
		x = first;
		while (x < alignedFirst) {
			ASSERT(x >= first);
			ASSERT(x <= last);
			BitVectorWrite(b, x, type);
			x++;
		}
		
		first = alignedFirst;
		ASSERT(first <= last);
	}
	
	if (last % alignment != alignment - 1) {
		alignedLast = last - (last % alignment) - 1;
		ASSERT(alignedLast >= 0);
		ASSERT(alignedLast >= first);
	
		x = alignedLast + 1;
		while (x <= last) {
			ASSERT(x >= first);
			ASSERT(x <= last);
			BitVectorWrite(b, x, type);
			x++;
		}

		ASSERT(alignedLast <= last);
		last = alignedLast;
	}
	
	ASSERT(first < last);	
	ASSERT(first % alignment == 0);
	ASSERT(last % alignment == (alignment - 1));
	BitVectorWriteRangeOptimized(b, first, last, type);
	return;
}

static INLINE void
BitVectorWriteRange(BitVector *b,
                    int first, int last,
                    BitVectorWriteType type)
{
	ASSERT(b != NULL);
	ASSERT(first >= 0);
	ASSERT((uint) first < b->size);
	ASSERT(last >= 0);
	ASSERT((uint) last < b->size);
	ASSERT(first <= last);
	
	if (CONSTANT(type) && CONSTANT(first) && CONSTANT(last)) {
		BitVectorWriteRangeGenericImpl(b, first, last, type);
		return;
	} else {
		BitVectorWriteRangeGenericDispatch(b, first, last, type);
	}
}

static INLINE void
BitVector_SetAll(BitVector *b)
{
	int first = 0;
	int last = b->size - 1;
	
	BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_SET);
}

static INLINE void
BitVector_ResetAll(BitVector *b)
{
	int first = 0;
	int last = b->size - 1;
	
	BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_RESET);
}

static INLINE void
BitVector_FlipAll(BitVector *b)
{
	int first = 0;
	int last = b->size - 1;
	
	BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_FLIP);
}

static INLINE void
BitVector_SetRange(BitVector *b, int first, int last)
{
	BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_SET);
}

static INLINE void
BitVector_ResetRange(BitVector *b, int first, int last)
{
	BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_RESET);
}

static INLINE void
BitVector_FlipRange(BitVector *b, int first, int last)
{
	BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_FLIP);
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

static INLINE int
BitVector_PopCount(const BitVector *b)
{
	int x;
	int size;
	int cellSize;
	int sum;
	
	int strayBitCount;
	uint64 strayBitMask;
	
	ASSERT(b != NULL);
	
	sum = 0;
	size = b->size;
	cellSize = size / BVUNITBITS;
	
	for (x = 0; x < cellSize; x++) {
		sum += popcountl(b->bits[x]);
	}
	
	strayBitCount = size % BVUNITBITS;	
	strayBitMask = ( ((uint64)1) << strayBitCount) - 1;
		
	ASSERT(cellSize >= 0);
	ASSERT((uint) cellSize < b->arrSize);
	sum += popcountl(b->bits[cellSize] & strayBitMask);
	
	return sum;
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
