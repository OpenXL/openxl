/*

   EngineLibrary.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "EngineLibrary.hh"
#include "EngineFunction.hh"
#include "FncPtr.hh"


EngineLibrary::EngineLibrary( XLlibid libid, const std::string& libName, const XLversion& libVersion )
	: Library( libid, libName, libVersion ), _functions(NULL), _fncListSize(0)
{
}

EngineLibrary::~EngineLibrary()
{
	for( int i = 0; i < _fncListSize; i++ )
		delete _functions[i];
	free( _functions );
}

void EngineLibrary::addFunction( XLstdid stdid, XLint32 opid, const std::string& defName, const FncPtr& function ) throw ( NoEntryException )
{
	EngineFunction*	engFnc;


	// Note! In libraries we can have functions for all kinds of standards, therefore
	// we don't need to check for compability here.
	engFnc = new EngineFunction( _libraryId, stdid, opid, defName, function );

	_fncListSize++;
	_functions = (EngineFunction**)realloc( _functions, _fncListSize * sizeof(EngineFunction*) );
	_functions[_fncListSize-1] = engFnc;

}

XLint32 EngineLibrary::getFunctions( Function*** fncs )
{
	// This is the same thing. Maybe we should solve this in a nicer way.
	*fncs = reinterpret_cast<Function**>(_functions);
	return _fncListSize;
}
