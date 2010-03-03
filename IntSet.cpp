#include "IntSet.h"

#include "mjbassert.h"

IntSet::IntSet()
{   }

IntSet::IntSet(const IntSet &s)
{
	insertAll(s);
}

bool IntSet::isEmpty() const
{
	return size() == 0;
}

int IntSet::size() const
{
	return values.size();
}

bool IntSet::makeEmpty()
{
	if(isEmpty()) {
		return FALSE;
	}
	
	m.makeEmpty();
	values.makeEmpty();
	
	return TRUE;
}

bool IntSet::contains(int x) const
{
	return m.get(x) == 1;
}

bool IntSet::insert(int x)
{
	bool oup = m.put(x,1);
	
	if(oup) {
		values.push(x);
	}
	
	return oup;
}

bool IntSet::checkInvariants() const
{
	for(int x=0;x<values.length();x++) {
		for(int y=x+1;y<values.length();y++) {
			ASSERT(values[x] != values[y]);
		}
	}
	return TRUE;
}

bool IntSet::remove(int x)
{
	bool oup = m.remove(x);
	
	if(oup) {
		for(int y=0;y<values.length();y++) {
			if(values[y] == x ) {
				values[y] = values[values.length()-1];
				values.shrink();
				return oup;
			}
		}
	}
	
	ASSERT(oup == FALSE);	
	return oup;
}

bool IntSet::insertAll(const IntSet &s)
{
	bool oup = FALSE;
	
	for(int x=0;x<s.values.size();x++) {
		oup |= insert(s.values[x]);
	}
	
	return oup;
}

void IntSet::getValues(MBVector<int> &v) const
{
	values.pushAllTo(v);
}
