/*

   Definition.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef DEFINITION_HH_
#define DEFINITION_HH_

#include <XL/xltypes.h>

class Function;


class Definition
{

public:

	Definition( XLstdid stdid, XLint32 opid );
	virtual ~Definition();

	XLstdid							getStandardId( void ) const;
	XLint32							getOperationId( void ) const;

	virtual Function*			getFunction( void ) const = 0;

	virtual XLstatus				exec( XLpv* arg, XLpv* result ) = 0;


private:

	XLstdid							_standardId;
	XLint32							_operationId;


};

#endif
