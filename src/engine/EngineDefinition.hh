/*

   EngineDefinition.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef ENGINEDEFINITION_HH_
#define ENGINEDEFINITION_HH_

#include "Definition.hh"

class Function;


class EngineDefinition : public Definition
{

public:

	EngineDefinition( XLstdid stdid, XLint32 opid, Function* function );
	virtual ~EngineDefinition();

	virtual Function*			getFunction( void ) const;

	virtual XLstatus				exec( XLpv* arg, XLpv* result );


private:

	Function*					_function;


};


#endif
