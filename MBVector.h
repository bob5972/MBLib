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
		int length() const;
		int size() const;
		bool isEmpty() const;
		void makeEmpty();
		
	//Indexing
		itemType & operator [ ] (int index);
		const itemType & operator [ ] (int index) const;
		
    //Modifiers
		//Resizes vector to length newSize
		void resize (int newSize);
		
		//Increases the vector size by 1
		void grow();

		//Increases the vector size by howMuch
		void grow(int howMuch);

		//Decreases vector size by 1
		void shrink();

		//Decreases vector size by howMuch
		//Calling shrink past size 0 results in an error
		void shrink(int howMuch);

		//Adds item to the end of the vector
		//returns new largest valid index (old size)
		int push(const itemType & item);
		
		//returns the item on the end of the vector and decreases the size by 1
		//ie (the last item pushed)
		const itemType & pop();
		
		//This is a weird function.
		//It empties this vector, copies over everything from
		//  v, and then empties v.
		void consume(MBVector<itemType> &v);
		

		
		//frees excess array locations
		//(ie more available memory)
		//post: myCapacity == mySize
		//returns the number of freed indexes
		//(ie how many itemTypes were deallocated)
		int trim();		
	
	private:
		int mySize,myCapacity;
		itemType * myItems;
};

#include "MBVector.cpp"
#endif //MBVECTOR_H_201001091353
