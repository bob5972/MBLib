
#include "MBHashSet.h"

#include <stdlib.h>
#include <math.h>
#include <limits.h>

const int DEFAULT_HASH_SIZE =13;
const double DEFAULT_LOAD_FACTOR =1;

template<class itemType>
MBHashSet<itemType>::MBHashSet(int (*hashFunction)(const itemType&))
:myHashFunction(hashFunction),
 mySize(0),
 myLoadFactor(DEFAULT_LOAD_FACTOR),
 myTable(DEFAULT_HASH_SIZE,NULL)
{ }

template<class itemType>
MBHashSet<itemType>::MBHashSet(int defSize,int (*hashFunction)(const itemType&))
:myHashFunction(hashFunction),
 mySize(0),
 myLoadFactor(DEFAULT_LOAD_FACTOR)
{
	myTable.resize(nextPrime(defSize));
	int len=myTable.length();
	for(int x=0;x<len;x++)
	{
		myTable[x]=NULL;
	}
}

template<class itemType>
MBHashSet<itemType>::MBHashSet(int defSize, double loadFactor, int(*hashFunction)(const itemType&))
:myHashFunction(hashFunction),
 mySize(0),
 myLoadFactor(loadFactor)
{
	myTable.resize(nextPrime(defSize));
	int len=myTable.length();
	for(int x=0;x<len;x++)
	{
		myTable[x]=NULL;
	}
}

template<class itemType>
void MBHashSet<itemType>::insert(const itemType& i)
{
	int pos=find(i);
	
	if(myTable[pos] == NULL)
	{
		myTable[pos] = new HashEntry(i);
	}
	else
	{
		HashEntry* cur=myTable[pos];
		bool found=false;
		while(!found && cur->next!=NULL)
		{
			if(cur->data == i)
			{
				//data == i is already true
				//but just in case there is other information
				//copy it over
				cur->data = i;
				found = true;
			}
			else
				cur=cur->next;
		}
		if(!found)
			cur->next=new HashEntry(i);
	}
	mySize++;
	if(load()>maxLoad())
	{
		rehash();
	}
}

template<class itemType>
void MBHashSet<itemType>::remove(const itemType & i)
{
	int pos = find(i);
	
	if(myTable[pos]==NULL)
	{
		//not in table
		return;
	}
	
	if(myTable[pos]->data == i)
	{
		HashEntry* temp = myTable[pos];
		myTable[pos]=temp->next;
		delete temp;
		temp = NULL;
		mySize--;
		return;
	}
	
	HashEntry* parent=myTable[pos];
	HashEntry* cur=parent->next;
	while(cur!=NULL)
	{
		if(cur->data == i)
		{
			parent->next=cur->next;
			delete cur;
			cur=NULL;
			mySize--;
			return;
		}
		parent=cur;
		cur=parent->next;
	}
	//end of list
	//its not here
	return;
}

template<class itemType>
void MBHashSet<itemType>::makeEmpty()
{
	for(int x=0;x<myTable.length();x++)
	{
		if(myTable[x] != NULL)
		{
			myTable[x]->removeAllChildren();
			delete myTable[x];
			myTable[x]=NULL;
		}
	}
	mySize = 0;
}

template<class itemType>
bool MBHashSet<itemType>::isEmpty() const
{
	return mySize == 0;
}

template<class itemType>
bool MBHashSet<itemType>::contains(const itemType& i) const
{
	int x=find(i);
	if(myTable[x] == NULL)
		return false;
	if(myTable[x]->data == i)
		return true;
	HashEntry* parent = myTable[x];
	HashEntry* cur = parent->next;
	while(cur!=NULL)
	{
		if(cur->data==i)
			return true;
		parent=cur;
		cur=parent->next;
	}
	
	return false;
}

template<class itemType>
double MBHashSet<itemType>::load() const
{
	return (mySize)/double(myTable.length());
}

template<class itemType>
double MBHashSet<itemType>::maxLoad() const
{
	return myLoadFactor;
}

template<class itemType>
int MBHashSet<itemType>::size() const
{
	return mySize;
}

template<class itemType>
apvector<itemType> MBHashSet<itemType>::items() const
{
	apvector<itemType> oup(mySize);
	int found=0;
	int pos =0;
	for(int x=0;found<mySize && x<myTable.length();x++)
	{
		if(myTable[x]!= NULL)
		{
			oup[pos++]=myTable[x]->data;
			found++;
			
			HashEntry* parent=myTable[x];
			HashEntry* cur=parent->next;
			while(cur!=NULL)
			{
				oup[pos++]=cur->data;
				found++;
				parent=cur;
				cur=parent->next;
			}
		}
	}
	return oup;
}

template<class itemType>
MBHashSet<itemType>::~MBHashSet()
{
	makeEmpty();
}

template<class itemType>
int MBHashSet<itemType>::hash(const itemType&i) const
{
	int hValue = myHashFunction(i);
	hValue = hValue%myTable.length();
	if(hValue<0)
		hValue+=myTable.length();
	return hValue;
}

template<class itemType>
int MBHashSet<itemType>::find(const itemType&i) const
{
	return hash(i);
}

template <class itemType>
void MBHashSet<itemType>::rehash()
{
	apvector<itemType> stuff = items();
	int oldLength = myTable.length();
	//free memory from old items
	makeEmpty();
	myTable.resize(nextPrime(oldLength*2));
	for(int x=0;x<myTable.length();x++)
	{
		//Clear out any junk in our new array
		myTable[x]=NULL;
	}
	for(int x=0;x<stuff.length();x++)
	{
		insert(stuff[x]);
	}
}

template <class itemType>
int MBHashSet<itemType>::nextPrime(int x)
//Returns the next Prime P >= x
//or -1 if it can't find a larger prime
{
	if(x<=1)
	{
		return 2;
	}
	x++;
	if(x%2==0)
		x++;
	//x is odd
	while((x+1)%30!=0)
	{
		if(isPrime(x))
			return x;
		x+=2;
	}
	//x+1%30 == 0
	for(x=x+1;INT_MAX-x>30;x+=30)
	{
		if(isPrime(x+1))
			return x+1;
		if(isPrime(x+3))
			return x+3;
		if(isPrime(x+5))
			return x+5;
		if(isPrime(x+7))
			return x+7;
		if(isPrime(x+11))
			return x+11;
		if(isPrime(x+13))
			return x+13;
		if(isPrime(x+17))
			return x+17;
		if(isPrime(x+19))
			return x+19;
		if(isPrime(x+23))
			return x+23;
		if(isPrime(x+29))
			return x+29;
	}
	return -1;
	//will crash apvector when used to resize
	//not very nice but oh well
}

template<class itemType>
bool MBHashSet<itemType>::isPrime(int x)
//Purpose: Determines if x is prime
//Precondition:  x>0
//Postcondition:
{
	int y(smallestPrimeFactor(x));
	if (y==x && y!= 0 && y!=1)
		return true;
	return false;
}

template<class itemType>
int MBHashSet<itemType>::smallestPrimeFactor(int x)
//Returns the smallest prime factor of x
{
	if (x==0)
		return 0;	
	if (x<0)
		x = std::abs(x);
	if (x%2==0)
		return 2;
	int max=int(std::sqrt(double(x)));
	for (int y=3;y<=max;y+=2)
	{
		if(x%y==0)
			return y;
	}
	return x;
}
