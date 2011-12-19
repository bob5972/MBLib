#ifndef MBVECTOR_H_201001091353
#define MBVECTOR_H_201001091353

template<class itemType>
class MBVector
{
	public:

    //Constructors
		MBVector();
		MBVector(const MBVector& vec);

		//Default Vector of length size
		explicit MBVector(int size);
		
		//Vector of length size initialized to fillValue
		MBVector(int size, const itemType & fillValue);

	//Destructor
		~MBVector();
		
	//Assignment
		const MBVector & operator = (const MBVector & vec);
		
	//Accessors
		//return the leftmost index of item
		int find(const itemType & item) const;

		int length() const
        {
            return mySize;
        }

		int size() const
        {
        	return mySize;
        }

		bool isEmpty() const;
		void makeEmpty();
		
	//Indexing
		const itemType & get(int index) const;
		itemType& get(int index);
		itemType & operator [ ] (int index);
		const itemType & operator [ ] (int index) const;
		
		const itemType &last() const;
		itemType &last();
		const itemType &first() const;
		itemType &first();
		
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
		void grow(int howMuch);

		//Decreases vector size by 1
		void shrink()
        {
	        shrink(1);
        }


		//Decreases vector size by howMuch
		//Calling shrink past size 0 results in an error
		void shrink(int howMuch);

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
		const itemType & pop();

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
#endif //MBVECTOR_H_201001091353
