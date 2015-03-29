/*

   Function.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef FUNCTION_HH_
#define FUNCTION_HH_

#include <XL/xltypes.h>
#include <string>


class Function
{

public:

	Function( XLlibid libid, XLstdid stdid, XLint32 opid, const std::string& name );
	virtual ~Function();

	XLlibid							getLibraryId( void ) const;
	XLstdid							getStandardId( void ) const;
	XLint32							getOperationId( void ) const;
	const std::string&				getName( void ) const;

	virtual XLstatus				exec( XLpv* arg, XLpv* result ) = 0;


protected:

	XLlibid							_libraryId;
	XLstdid							_standardId;
	XLint32							_operationId;
	std::string						_name;

};

#endif
