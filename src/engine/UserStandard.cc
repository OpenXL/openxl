/*

   UserStandard.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "UserStandard.hh"
#include "UserOperation.hh"
#include "Library.hh"
#include "Function.hh"



UserStandard::UserStandard( XLstdid stdid, const std::string& stdName, const XLversion& stdVersion )
	: Standard( stdid, stdName, stdVersion )
{
}

UserStandard::~UserStandard()
{
	// Delete all operations.
	for( OpMap::iterator i = _operations.begin(); i != _operations.end(); ++i )
		delete (*i).second;
}

void UserStandard::declareOperation( XLint32 opid ) throw ( ExistsException )
{
	// First make sure the operation doesn't overlap any operation declared before.
	if( _operations.find( opid ) != _operations.end() )
		throw ExistsException();

	_operations[opid] = new UserOperation( _standardId, opid );
}

void UserStandard::declareOperationDomain( XLint32 fromOpid, XLint32 toOpid ) throw ( ExistsException )
{
	// First make sure the domain doesn't overlap any operation declared before.
	for( OpMap::iterator i = _operations.begin(); i != _operations.end(); ++i )
		if( (*i).first >= fromOpid && (*i).first <= toOpid )
			throw ExistsException();

	// Declare all the operations in the given domain.
	for( int i = fromOpid; i <= toOpid; i++ )
		_operations[i] = new UserOperation( _standardId, i );
}

void UserStandard::connectLibrary( Library* library )
{
	Function**				functions;
	XLint32						fncListSize;

	fncListSize = library->getFunctions( &functions );
	for( int i = 0; i < fncListSize; i++ )
	{
		// Go through all functions in the library and pick out
		// the ones that belong to this standard.
		if( functions[i]->getStandardId() == _standardId )
			_operations[ functions[i]->getOperationId() ]->registerDefinition( functions[i] );
	}
}

Operation* UserStandard::operation( XLint32 opid )
{
	// Make sure we actually have that operation.
	if( _operations.find( opid ) == _operations.end() )
		return NULL;

	return _operations[opid];
}

void UserStandard::getOperationList( std::list<Operation*>* operationList )
{
	for( OpMap::iterator i = _operations.begin(); i != _operations.end(); ++i )
		operationList->push_back( (*i).second );
}

XLstatus UserStandard::runOp( XLint32 opid, XLpv* arg, XLpv* result )
{
	// As a default we just run the operation.
	return _operations[opid]->run( arg, result );
}



