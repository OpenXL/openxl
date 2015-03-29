/*

   Function.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "Function.hh"

Function::Function( XLlibid libid, XLstdid stdid, XLint32 opid, const std::string& name )
	: _libraryId(libid), _standardId(stdid), _operationId(opid), _name(name)
{
}

Function::~Function()
{
}

XLlibid Function::getLibraryId( void ) const
{
	return _libraryId;
}

XLstdid Function::getStandardId( void ) const
{
	return _standardId;
}

XLint32 Function::getOperationId( void ) const
{
	return _operationId;
}

const std::string& Function::getName( void ) const
{
	return _name;
}
