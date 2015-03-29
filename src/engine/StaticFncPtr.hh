/*

   StaticFncPtr.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef STATICFNCPTR_HH_
#define STATICFNCPTR_HH_

#include "FncPtr.hh"


class StaticFncPtr : public FncPtr
{

public:

	StaticFncPtr( XLfncptr fnc ) : _fnc(fnc) {}
	virtual ~StaticFncPtr() {}

	virtual FncPtr*		clone( void ) const;

	virtual XLstatus		exec( XLpv* arg, XLpv* result );

	XLfncptr				getFncPtr( void );


private:

	XLfncptr	_fnc;

};


#endif
