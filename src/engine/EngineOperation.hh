/*

   EngineOperation.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef ENGINEOPERATION_HH_
#define ENGINEOPERATION_HH_

#include "Operation.hh"

class EngineDefinition;


class EngineOperation : public Operation
{

public:

	EngineOperation( XLstdid stdid, XLint32 opid );
	virtual ~EngineOperation();

	virtual bool				multipleDefinitionsAllowed( void ) const;
	virtual void				registerDefinition( Function* function );
	virtual void				getDefinitionList( std::list<Definition*>* definitionList );
	virtual XLstatus			run( XLpv* arg, XLpv* result );


private:

	EngineDefinition**		_definitions;
	XLint32						_defListSize;


};

#endif
