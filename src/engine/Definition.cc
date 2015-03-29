/*

   Definition.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "Definition.hh"

Definition::Definition( XLstdid stdid, XLint32 opid )
	: _standardId(stdid), _operationId(opid)
{
}

Definition::~Definition()
{
}

XLstdid Definition::getStandardId( void ) const
{
	return _standardId;
}

XLint32 Definition::getOperationId( void ) const
{
	return _operationId;
}
