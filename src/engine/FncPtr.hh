/*

   FncPtr.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef FNCPTR_HH_
#define FNCPTR_HH_

#include <XL/xltypes.h>


class FncPtr
{

public:

	virtual ~FncPtr() {}

	virtual FncPtr*		clone( void ) const = 0;

	virtual XLstatus		exec( XLpv* arg, XLpv* result ) = 0;



};


#endif
