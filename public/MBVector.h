#ifndef MBVECTOR_H_201001091353
#define MBVECTOR_H_201001091353

#include "mbassert.h"

#ifdef __cplusplus

#define MBVector MBVectorData
    extern "C" {
#endif

#if 0
typedef struct {
    int size;
    int capacity;
    int itemSize;
    void *items;
} MBVector;

static INLINE void MBVector_EnsureCapacity(MBVector *vector, int capacity)
{
    int newCap;
    ASSERT(vector->capacity > 0);
    ASSERT(vector->size > 0);
    ASSERT(capacity > 0);
    ASSERT(vector->itemSize > 0);

    if (vector->capacity >= capacity) {
        return;
    }

    newCap = vector->capacity;
    while (newCap < capacity) {
        newCap *= 2;
    }
    ASSERT(newCap > vector->capacity);

    vector->items = realloc(vector->items, vector->itemSize * newCap);
}

static INLINE void MBVector_Create(MBVector *vector, int itemSize,
                                   int size, int capacity)
{
    vector->size = size;
    vector->capacity = capacity;
    vector->itemSize = itemSize;

    ASSERT(itemSize > 0);
    ASSERT(capacity >= size);
    ASSERT(capacity > 0);
    ASSERT(size >= 0);

    vector->items = malloc(itemSize * vector->capacity);
}

static INLINE void MBVector_CreateEmpty(MBVector *vector, int itemSize)
{
    MBVector_Create(vector, itemSize, 0, 1);
}

static INLINE void MBVector_CreateWithSize(MBVector *vector, int itemSize,
                                           int size)
{
    MBVector_Create(vector, itemSize, size, size);
}

static INLINE void MBVector_Destroy(MBVector *vector)
{
    free(vector->items);
    vector->items = NULL;
}

static INLINE int MBVector_Size(const MBVector *vector)
{
    ASSERT(vector->size >= 0);
    return vector->size;
}

static INLINE void MBVector_Resize(MBVector *vector, int size)
{
    ASSERT(size >= 0);

    MBVector_EnsureCapacity(vector, size);
    vector->size = size;
}

static INLINE void MBVector_GrowBy(MBVector *vector, int increment)
{
    ASSERT(increment >= 0);
    MBVector_Resize(vector, vector->size + increment);
}

static INLINE void MBVector_ShrinkBy(MBVector *vector, int decrement)
{
    ASSERT(decrement >= 0);
    MBVector_Resize(vector, vector->size - decrement);
}

static INLINE bool MBVector_IsEmpty(const MBVector *vector)
{
    return MBVector_Size(vector) == 0;
}

static INLINE void MBVector_MakeEmpty(MBVector *vector)
{
    MBVector_Resize(vector, 0);
}

static INLINE void *MBVector_Get(MBVector *vector, int index)
{
    ASSERT(index >= 0);
    ASSERT(index < vector->size);

    return ((uint8 *)vector->items) + (index * vector->itemSize);
}

static INLINE void MBVector_Copy(MBVector *dest, const MBVector *src)
{
    ASSERT(dest->itemSize == src->itemSize);

    MBVector_Resize(dest, 0);
    MBVector_EnsureCapacity(dest, src->size);
    memcpy(dest->items, src->items, src->itemSize * src->size);
}
#endif


#ifdef __cplusplus
    } // extern "C"

    #undef MBVector

template<class itemType>
class MBVector
{
	public:

    //Constructors
		MBVector()
        :mySize(0),
         myCapacity(1),
         myItems(new itemType[1])
        {
	        ASSERT(myCapacity > 0);
        }

		MBVector(const MBVector& vec);

		//Default Vector of length size
		explicit MBVector(int size)
        {
	        mySize = size;
	        ASSERT(mySize >= 0);

	        if (mySize >= 1) {
		        myCapacity = mySize;
	        } else {
		        myCapacity = 1;
	        }

	        myItems = new itemType[myCapacity];

	        ASSERT(myCapacity > 0);
        }


		//Vector of length size initialized to fillValue
		MBVector(int size, const itemType & fillValue);

	//Destructor
		~MBVector()
        {
	        delete [] myItems;
        }

	//Assignment
		const MBVector & operator = (const MBVector & vec);

	//Accessors
		//return the leftmost index of item
		int find(const itemType & item) const;

		int size() const
        {
        	return mySize;
        }

		bool isEmpty() const
        {
	        return size() == 0;
        }

		void makeEmpty()
        {
     	    resize(0);
	        ASSERT(myCapacity > 0);
        }


	//Indexing
        itemType & get (int x)
        {
        	ASSERT(x < mySize);
            ASSERT(x >= 0);

    	    return myItems[x];
        }


        const itemType & get (int x) const
        {
        	ASSERT(x < mySize);
            ASSERT(x >= 0);

        	return myItems[x];
        }

        itemType & operator [] (int x)
        {
        	return get(x);
        }

        const itemType & operator [] (int x) const
        {
        	return get(x);
        }


		const itemType &last() const
        {
	        ASSERT(mySize > 0);
	        return myItems[mySize - 1];
        }

		itemType &last()
        {
	        ASSERT(mySize > 0);
	        return myItems[mySize - 1];
        }

		const itemType &first() const
        {
	        ASSERT(mySize > 0);
	        return myItems[0];
        }

		itemType &first()
        {
	        ASSERT(mySize > 0);
	        return myItems[0];
        }


    //Modifiers
		//Resizes vector to length newSize
		void resize (int newSize);

		//Resize with new items initialized to fill value
		void resize (int newSize, const itemType &fill);

		//Increases the vector size by 1
		void grow()
        {
        	grow(1);
        }

		//Increases the vector size by howMuch
		void grow(int howMuch)
        {
        	ASSERT(howMuch >= 0);
        	if (mySize + howMuch > myCapacity) {
        		ensureCapacity(mySize + howMuch);
        	}

        	mySize += howMuch;
        	ASSERT(mySize >= 0);
        	ASSERT(mySize <= myCapacity);
        }


		//Decreases vector size by 1
		void shrink()
        {
	        shrink(1);
        }


		//Decreases vector size by howMuch
		//Calling shrink past size 0 results in an error
		void shrink(int howMuch)
        {
	        ASSERT(howMuch >= 0);
        	ASSERT(mySize - howMuch >= 0);
        	mySize -= howMuch;
        }

		//Adds item to the end of the vector
		//returns new largest valid index (old size)
		int push(const itemType & item)
        {
        	grow();
        	myItems[mySize-1] = item;
        	return mySize-1;
        }


		//returns the item on the end of the vector and decreases the size by 1
		//ie (the last item pushed)
		const itemType & pop()
        {
        	ASSERT(mySize >= 0);

        	shrink();
	        return myItems[mySize];
        }


		//This is a weird function.
		//It empties this vector, copies over everything from
		//  v, and then empties v.
		void consume(MBVector<itemType> &v);

		//pushes all the elements of v into this vector
		void pushAllFrom(const MBVector<itemType> & v);

		//pushes all the elements of this vector into v
		void pushAllTo(MBVector<itemType> & v) const;

		//frees excess array locations
		//(ie more available memory)
		//post: myCapacity == mySize
		//returns the number of freed indexes
		//(ie how many itemTypes were deallocated)
		int trim();

		//does NOT resize the vector
		//merely increases the size of the
		// underlying array, so that future
		// growing will be faster
		void ensureCapacity(int c);

	private:
		int mySize;
		int myCapacity;
		itemType * myItems;
};

#include "../MBVector.cpp"
#endif //__cplusplus

#endif //MBVECTOR_H_201001091353
