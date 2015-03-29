/*

   UserLibrary.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef USERLIBRARY_HH_
#define USERLIBRARY_HH_

#include "Library.hh"

class UserFunction;


class UserLibrary : public Library
{

public:

	UserLibrary( XLlibid libid, const std::string& libName, const XLversion& libVersion );
	virtual ~UserLibrary();


	virtual void			addFunction( XLstdid stdid, XLint32 opid, const std::string& defName, const FncPtr& function ) throw ( NoEntryException );
	virtual XLint32			getFunctions( Function*** fncs );


private:

	UserFunction**		_functions;
	XLint32					_fncListSize;


};

#endif
