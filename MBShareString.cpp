#include "MBShareString.h"

#include <stdlib.h>
#include <ctype.h>

#include "mbtypes.h"
#include "mbdebug.h"
#include "mbassert.h"

void MBShareString::unlinkRef()
{
	if(myRef != NULL) {
		ASSERT(myRef->refCount > 0);
		myRef->refCount--;
		if(myRef->refCount == 0) {
			delete myRef;
		}
		myRef = NULL;
	}
}

void MBShareString::linkRef(StringRef* ref)
{
	if(myRef != NULL) {
		unlinkRef();
	}
	
	myRef = ref;
	if(myRef != NULL) {
		ASSERT(myRef->refCount > 0);
		myRef->refCount++;
		
		//this is an overflow check
		ASSERT(myRef->refCount > 0);
	}
}

void MBShareString::copyRef()
{
	if(myRef == NULL) {
		myRef = new StringRef();
		return;
	}
	
	ASSERT(myRef->refCount > 0);
	
	if(myRef->refCount == 1) {
		return;
	}
	
	StringRef* temp = new StringRef(myRef->str);
	
	unlinkRef();
	
	myRef = temp;
}

char& MBShareString::operator[ ]( int k )
{
	ASSERT(myRef->refCount > 0);
	
	if(myRef->refCount == 1) {
		return myRef->str[k];
	} else {
		copyRef();
		return myRef->str[k];
	}
}

//Append str
const MBShareString& MBShareString::operator += ( const MBShareString & mbstr )
{
	//weird case
	if(length() == 0) {
		return ((*this) = mbstr);
	}
	
	if(mbstr.length() > 0) {
		copyRef();
	}
	
	myRef->str.append(mbstr.myRef->str);
	
	return *this;
}
		
