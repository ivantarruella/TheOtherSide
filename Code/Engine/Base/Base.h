//-------------------------------------
// includes básicos
//-------------------------------------
#ifndef _BASE_H
#define _BASE_H

#include "Utils/Types.h"
#include "Core.h"
#include <assert.h>

#if defined( _DEBUG ) && !defined(__DONT_INCLUDE_MEM_LEAKS__)
	#include "MemLeaks.h"
#endif


//--- BASIC INCLUDES ---------
#define CHECKED_DELETE(x)		if(x) delete x; x=0;
#define CHECKED_RELEASE(x)	if(x) x->Release(); x=0;
#define CHECKED_DELETE_ARRAY(x)	if(x) delete [] x; x=0;
//------------------------------

#define CORE CCore::GetSingletonPtr()

#endif //_BASE_H