/*

   StaticFncPtr.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "StaticFncPtr.hh"


FncPtr* StaticFncPtr::clone( void ) const
{
	return new StaticFncPtr( _fnc );
}

XLstatus StaticFncPtr::exec( XLpv* arg, XLpv* result )
{
	return _fnc( arg, result );
}

XLfncptr StaticFncPtr::getFncPtr( void )
{
	return _fnc;
}

