/*

   EngineDefinition.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "EngineDefinition.hh"
#include "Function.hh"

EngineDefinition::EngineDefinition( XLstdid stdid, XLint32 opid, Function* function )
	: Definition( stdid, opid ), _function(function)
{
}

EngineDefinition::~EngineDefinition()
{
	// Do not delete _function here.
}

Function* EngineDefinition::getFunction( void ) const
{
	return _function;
}

XLstatus EngineDefinition::exec( XLpv* arg, XLpv* result )
{
	return _function->exec( arg, result );
}
