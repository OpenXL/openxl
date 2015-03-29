/*

   EngineOperation.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "EngineOperation.hh"
#include "EngineDefinition.hh"

#include <stdlib.h>


EngineOperation::EngineOperation( XLstdid stdid, XLint32 opid )
	: Operation( stdid, opid ), _definitions(NULL), _defListSize(0)
{
}

EngineOperation::~EngineOperation()
{
	for( int i = 0; i < _defListSize; i++ )
		delete _definitions[i];
	free( _definitions );
}

bool EngineOperation::multipleDefinitionsAllowed( void ) const
{
	return false;
}

void EngineOperation::registerDefinition( Function* function )
{
	EngineDefinition*	engDef;

	// Add the new definition. Remember, we can have multiple definitions for
	// the same declaration (op).
	// We keep the structure for multiple definitions in case we want to
	// experiment in the future.
	engDef = new EngineDefinition( _standardId, _operationId, function );

	_defListSize++;
	_definitions = (EngineDefinition**)realloc( _definitions, _defListSize * sizeof(EngineDefinition*) );
	_definitions[_defListSize-1] = engDef;

}

void EngineOperation::getDefinitionList( std::list<Definition*>* definitionList )
{
	for( int i = 0; i < _defListSize; i++ )
		definitionList->push_back( _definitions[i] );
}

XLstatus EngineOperation::run( XLpv* arg, XLpv* result )
{
	// Since EngineOperation doesn't allow multiple definitions we just
	// take the first definition in the list.
	return _definitions[0]->exec( arg, result );
}
