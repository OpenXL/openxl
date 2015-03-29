/*

   Library.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef LIBRARY_HH_
#define LIBRARY_HH_

#include <XL/xltypes.h>
#include "exception/NoEntryException.hh"
#include <string>

class FncPtr;
class Function;


class Library
{

public:

	Library( XLlibid libid, const std::string& libName, const XLversion& libVersion );
	virtual ~Library();

	XLlibid					getLibraryId( void ) const;
	const std::string&		getName( void ) const;

	virtual void			addFunction( XLstdid stdid, XLint32 opid, const std::string& defName, const FncPtr& function ) throw ( NoEntryException ) = 0;
	virtual XLint32			getFunctions( Function*** fncs ) = 0;


protected:

	XLlibid					_libraryId;
	std::string				_name;
	XLversion				_version;



};

#endif
