/*

   Operation.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef OPERATION_HH_
#define OPERATION_HH_

#include <XL/xltypes.h>
#include <list>

class Function;
class Definition;


class Operation
{

public:

	Operation( XLstdid stdid, XLint32 opid );
	virtual ~Operation();


	virtual bool				multipleDefinitionsAllowed( void ) const = 0;
	virtual void				registerDefinition( Function* function ) = 0;
	virtual void				getDefinitionList( std::list<Definition*>* definitionList ) = 0;
	virtual XLstatus			run( XLpv* arg, XLpv* result ) = 0;


protected:

	XLstdid						_standardId;
	XLint32						_operationId;


};

#endif
