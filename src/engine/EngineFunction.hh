/*

   EngineFunction.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef ENGINEFUNCTION_HH_
#define ENGINEFUNCTION_HH_

#include "Function.hh"

class FncPtr;


class EngineFunction : public Function
{

public:

	EngineFunction( XLlibid libid, XLstdid stdid, XLint32 opid, const std::string& name, const FncPtr& fncptr );
	virtual ~EngineFunction();

	virtual XLstatus				exec( XLpv* arg, XLpv* result );



private:

	FncPtr*						_dxlFnc;
	XLfncptr						_function;


};

#endif
