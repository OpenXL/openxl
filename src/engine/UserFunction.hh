/*

   UserFunction.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef USERFUNCTION_HH_
#define USERFUNCTION_HH_

#include "Function.hh"

class FncPtr;


class UserFunction : public Function
{

public:

	UserFunction( XLlibid libid, XLstdid stdid, XLint32 opid, const std::string& name, const FncPtr& fncptr );
	virtual ~UserFunction();

	virtual XLstatus				exec( XLpv* arg, XLpv* result );



private:

	FncPtr*						_dxlFnc;
	XLfncptr						_function;

};

#endif
