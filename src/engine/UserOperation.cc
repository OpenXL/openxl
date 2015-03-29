/*

   UserOperation.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "UserOperation.hh"
#include "UserDefinition.hh"
#include <XL/xldefs.h>

#include <stdlib.h>


UserOperation::UserOperation( XLstdid stdid, XLint32 opid )
	: Operation( stdid, opid ), _definitions(NULL), _defListSize(0)
{
	_argGenerator = NULL;
	_argAnalyzer = NULL;

	_defListSize = 0;
	_enabledDefListSize = 0;
	_definitions = NULL;
	_enabledDefinitions = NULL;
}

UserOperation::~UserOperation()
{
	for( int i = 0; i < _defListSize; i++ )
		delete _definitions[i];
	free( _definitions );

	free( _enabledDefinitions );
}

bool UserOperation::multipleDefinitionsAllowed( void ) const
{
	return true;
}

void UserOperation::registerDefinition( Function* function )
{
	UserDefinition*	usrDef;

	// Add the new definition. Remember, we can have multiple definitions for
	// the same declaration (op).
	usrDef = new UserDefinition( _standardId, _operationId, function );

	// Set the argument analyzer if any.
	usrDef->setArgumentAnalyzer( _argAnalyzer );

	_defListSize++;
	_definitions = (UserDefinition**)realloc( _definitions, _defListSize * sizeof(UserDefinition*) );
	_definitions[_defListSize-1] = usrDef;

	// Automatically enable it.
	_enabledDefListSize++;
	_enabledDefinitions = (UserDefinition**)realloc( _enabledDefinitions, _enabledDefListSize * sizeof(UserDefinition*) );
	_enabledDefinitions[_enabledDefListSize-1] = usrDef;
}

void UserOperation::getDefinitionList( std::list<Definition*>* definitionList )
{
	for( int i = 0; i < _defListSize; i++ )
		definitionList->push_back( _definitions[i] );
}

XLstatus UserOperation::run( XLpv* arg, XLpv* result )
{
	XLpv				argSize;
	XLint64				etoc;
	XLint64				bestEtoc;
	UserDefinition*	bestEtocDef;


	// Set the best etoc definition so far to be the first in the list.
	bestEtocDef = _enabledDefinitions[0];

	// Since performance is everything here there aint much of error check
	// on input arguments.

	if( _enabledDefListSize > 1 )
	{
		// To be able to choose which definition to use for this
		// calculation then we here select on attribute fastest completion time.
		// The time is precalculated as an x^2 curve with respect to argument size,
		// therefore we can fast get an ETOC by giving the argument size.
		// The ETOC is then compared between the different definitions
		// to get the fastest one.

		// Get the argument size.
		_argAnalyzer( arg, &argSize );

		// Check the ETOC of the first definition of this operation.
		bestEtoc = bestEtocDef->getEtoc( argSize.value.real32 );

		// Go through the rest of the definitions.
		for( int i = 1; i < _enabledDefListSize; i++ )
		{
			etoc = _enabledDefinitions[i]->getEtoc( argSize.value.real32 );
			if( etoc < bestEtoc )
			{
				bestEtoc = etoc;
				bestEtocDef = _enabledDefinitions[i];
			}
		}
	}


	// Call the function.

	// If the argument size is new we should clock it here to keep statistics.
	bestEtocDef->exec( arg, result );

	return XL_STATUS_NO_ERROR;
}

void UserOperation::registerArgGenerator( XLfncptr fnc )
{
	_argGenerator = fnc;
}

void UserOperation::registerArgAnalyzer( XLfncptr fnc )
{
	_argAnalyzer = fnc;

	// Go through all definitions and set their argument analyzer.
	for( int i = 0; i < _defListSize; i++ )
		_definitions[i]->setArgumentAnalyzer( fnc );
}

void UserOperation::enableDefinitions( const std::list<Definition*>& definitionList )
{
	for( std::list<Definition*>::const_iterator i = definitionList.begin(); i != definitionList.end(); ++i )
	{
		UserDefinition*		usrDef;

		// First make sure it's a UserDefinition. If it's not then we should generate a hard error since
		// then the definition doesn't belong to this operation at all.
		if( (usrDef = dynamic_cast<UserDefinition*>( (*i) )) == 0 )
			fprintf( stderr, "UserOperation::enableDefinitions  Serious error!\n" );

		// Check so it's not already enabled.
		int j;
		for( j = 0; j < _enabledDefListSize; j++ )
			if( _enabledDefinitions[j] == usrDef )
				break;
		if( j != _enabledDefListSize )
			continue;

		// Otherwise add it to the enabled definitions list.
		_enabledDefListSize++;
		_enabledDefinitions = (UserDefinition**)realloc( _enabledDefinitions, _enabledDefListSize * sizeof(UserDefinition*) );
		_enabledDefinitions[_enabledDefListSize-1] = usrDef;
	}

}

void UserOperation::disableDefinitions( const std::list<Definition*>& definitionList )
{
	UserDefinition**		newEnabledDefinitions = NULL;
	int						newEnabledDefListSize = 0;
	int						nMatch = 0;


	if( _enabledDefListSize > 0 )
	{
		// All definitions in the disabled list found in the enabled list are set to NULL to
		// later be removed completely.
		for( int i = 0; i < _enabledDefListSize; i++ )
		{
			for( std::list<Definition*>::const_iterator j = definitionList.begin(); j != definitionList.end(); ++j )
			{
				if( (*j) == _enabledDefinitions[i] )
				{
					_enabledDefinitions[i] = NULL;
					nMatch++;
					break;
				}
			}
		}

		newEnabledDefListSize = _enabledDefListSize - nMatch;
		newEnabledDefinitions = (UserDefinition**)malloc( newEnabledDefListSize * sizeof(UserDefinition*) );

		// Transfer all data to a new data area, i.e. remove all definitions previously set to NULL.
		for( int i = 0, j = 0; i < _enabledDefListSize; i++ )
			if( _enabledDefinitions[i] != NULL )
				newEnabledDefinitions[j++] = _enabledDefinitions[i];

		free( _enabledDefinitions );

		_enabledDefinitions = newEnabledDefinitions;
		_enabledDefListSize = newEnabledDefListSize;
	}

}

void UserOperation::calculateAllDefinitionsEtoc( void )
{
	// Go through all definitions and calculate the ETOC for each of them.
	for( int i = 0; i < _defListSize; i++ )
		_definitions[i]->calculateEtoc( _argGenerator );
}

XLstatus UserOperation::getEtoc( XLpv* arg, XLpv* result )
{
	XLpv		argSize;
	XLint64		etoc;
	XLint64		bestEtoc;


	// Get the argument size.
	_argAnalyzer( arg, &argSize );

	// Check the ETOC of the first enabled definition of this operation.
	bestEtoc = _enabledDefinitions[0]->getEtoc( argSize.value.real32 );

	// Go through the rest of the definitions.
	for( int i = 1; i < _enabledDefListSize; i++ )
	{
		etoc = _enabledDefinitions[i]->getEtoc( argSize.value.real32 );
		if( etoc < bestEtoc )
			bestEtoc = etoc;
	}

	// Micro seconds.
	result[0].value.int32 = (XLint32)bestEtoc % 1000000L;

	// Seconds.
	bestEtoc /= 1000000;
	result[1].value.int32 = (XLint32)bestEtoc % 60;

	// Minutes.
	bestEtoc /= 60;
	result[2].value.int32 = (XLint32)bestEtoc % 60;

	// Hours.
	bestEtoc /= 60;
	result[3].value.int32 = (XLint32)bestEtoc;


	return XL_STATUS_NO_ERROR;
}



