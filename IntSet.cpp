#include "IntSet.hpp"

#include "mbassert.h"

bool IntSet::makeEmpty()
{
	if(isEmpty()) {
		return FALSE;
	}

	m.makeEmpty();
	values.makeEmpty();

	return TRUE;
}

bool IntSet::insert(int x)
{
	bool oup = !m.containsKey(x);

	m.put(x,1);

	if (oup) {
        ASSERT(values.find(x) == -1);
		values.push(x);
	} else {
        ASSERT(values.find(x) != -1);
    }

	return oup;
}


bool IntSet::checkInvariants() const
{
	for(int x=0;x<values.size();x++) {
		for(int y=x+1;y<values.size();y++) {
			ASSERT(values[x] != values[y]);
		}
	}
	return TRUE;
}

bool IntSet::remove(int x)
{
	bool oup = m.remove(x);

	if(oup) {
		for(int y=0;y<values.size();y++) {
			if(values[y] == x ) {
				values[y] = values[values.size()-1];
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
