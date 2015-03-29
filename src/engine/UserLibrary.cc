/*

   UserLibrary.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "UserLibrary.hh"
#include "UserFunction.hh"
#include "FncPtr.hh"


UserLibrary::UserLibrary( XLlibid libid, const std::string& libName, const XLversion& libVersion )
	: Library( libid, libName, libVersion ), _functions(NULL), _fncListSize(0)
{
}

UserLibrary::~UserLibrary()
{
	for( int i = 0; i < _fncListSize; i++ )
		delete _functions[i];
	free( _functions );
}

void UserLibrary::addFunction( XLstdid stdid, XLint32 opid, const std::string& defName, const FncPtr& function ) throw ( NoEntryException )
{
	UserFunction*	usrFnc;


	// Note! In libraries we can have functions for all kinds of standards, therefore
	// we don't need to check for compability here.
	usrFnc = new UserFunction( _libraryId, stdid, opid, defName, function );

	_fncListSize++;
	_functions = (UserFunction**)realloc( _functions, _fncListSize * sizeof(UserFunction*) );
	_functions[_fncListSize-1] = usrFnc;

}

XLint32 UserLibrary::getFunctions( Function*** fncs )
{
	// This is the same thing. Maybe we should solve this in a nicer way.
	*fncs = reinterpret_cast<Function**>(_functions);
	return _fncListSize;
}
