/*

   UserFunction.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "UserFunction.hh"
#include "FncPtr.hh"
#include "StaticFncPtr.hh"


UserFunction::UserFunction( XLlibid libid, XLstdid stdid, XLint32 opid, const std::string& name, const FncPtr& fncptr )
	: Function( libid, stdid, opid, name ), _function(NULL)
{
	// Copy the object completely since fncptr may be on the caller's stack.
	_dxlFnc = fncptr.clone();

	// This is a small optimization.
	// If it's a static function pointer then we can avoid one function call
	// by extracting it and use it directly in exec().
	StaticFncPtr* sfp = dynamic_cast<StaticFncPtr*>(_dxlFnc);
	if( sfp != NULL )
		_function = sfp->getFncPtr();

}

UserFunction::~UserFunction()
{
	delete _dxlFnc;
}

XLstatus UserFunction::exec( XLpv* arg, XLpv* result )
{
	// A small optimization.
	if( _function )
		return _function( arg, result );

	return _dxlFnc->exec( arg, result );
}
