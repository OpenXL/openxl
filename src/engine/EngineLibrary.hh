/*

   EngineLibrary.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef ENGINELIBRARY_HH_
#define ENGINELIBRARY_HH_

#include "Library.hh"

class EngineFunction;


class EngineLibrary : public Library
{

public:

	EngineLibrary( XLlibid libid, const std::string& libName, const XLversion& libVersion );
	virtual ~EngineLibrary();


	virtual void			addFunction( XLstdid stdid, XLint32 opid, const std::string& defName, const FncPtr& function ) throw ( NoEntryException );
	virtual XLint32			getFunctions( Function*** fncs );


private:

	EngineFunction**		_functions;
	XLint32					_fncListSize;


};


#endif
