/*

   Database.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "Database.hh"
#include "FncPtr.hh"
#include "StaticFncPtr.hh"
#include "Standard.hh"
#include "Operation.hh"
#include "Definition.hh"
#include "Function.hh"
#include "EngineStandard.hh"
#include "UserStandard.hh"
#include "UserDefinition.hh"
#include "EngineLibrary.hh"
#include "UserLibrary.hh"
#include <stdlib.h>
#include <gsl/gsl_multifit.h>
#include <sstream>



Database::Database()
{
	stdLut = NULL;
	_libLut = NULL;

	mysql_server_init( 0, NULL, NULL );
	_mySQL = NULL;

}

Database::~Database()
{
	// Store all definitions to the disk database if connected.
	_writeDefsToDB();

	// Disconnect the database if connected.
	if( _mySQL )
	{
		mysql_close( _mySQL );
		_mySQL = NULL;
	}

	// Delete all standards.
	for( std::map<std::string, Standard*>::iterator i = _standards.begin(); i != _standards.end(); ++i )
		delete (*i).second;

	// Delete all libraries.
	for( std::map<std::string, Library*>::iterator i = _libraries.begin(); i != _libraries.end(); ++i )
		delete (*i).second;

	// Free the quick indices as well.
	free( stdLut );
	free( _libLut );

	if( _mySQL != NULL )
		mysql_close( _mySQL );
}

void Database::createEngineStandard( const std::string& name, const XLversion& version, XLstdid* stdid ) throw ( ExistsException )
{
	EngineStandard*		engStd;

	if( _standards.find( name ) != _standards.end() )
		throw ExistsException();

	// Allocate space for a new standard. We do like this to get the stdid.
	_standards[ name ] = NULL;
	*stdid = _standards.size(); // The size represents the last element, which we want.
	engStd = new EngineStandard( *stdid, name, version );
	_standards[ name ] = engStd;

	// Store it in different places. _standards is slightly redundant actually.
	// _lutIndex is used by getStandard(), and stdLut is the main look up table used in run-time.
	_lutIndex[ name ] = *stdid;

	// +1 to reserve index 0 for the special "standard" where the user set up a home made mapping.
	// Maybe we need to reconsider the special "standard".
	stdLut = (Standard**)realloc( stdLut, _standards.size() * sizeof(Standard*) + 1 );
	stdLut[ *stdid ] = engStd;

}

void Database::createUserStandard( const std::string& name, const XLversion& version, XLstdid* stdid ) throw ( ExistsException )
{
	UserStandard*		usrStd;

	if( _standards.find( name ) != _standards.end() )
		throw ExistsException();

	// Allocate space for a new standard. We do like this to get the stdid.
	_standards[ name ] = NULL;
	*stdid = _standards.size(); // The size represents the last element, which we want.
	usrStd = new UserStandard( *stdid, name, version );
	_standards[ name ] = usrStd;

	// Store it in different places. _standards is slightly redundant actually.
	// _lutIndex is used by getStandard(), and stdLut is the main look up table used in run-time.
	_lutIndex[ name ] = *stdid;

	// +1 to reserve index 0 for the special "standard" where the user set up a home made mapping.
	// Maybe we need to reconsider the special "standard".
	stdLut = (Standard**)realloc( stdLut, _standards.size() * sizeof(Standard*) + 1 );
	stdLut[ *stdid ] = usrStd;

}

void Database::getStandard( const std::string& name, XLversion* version, XLstdid* id ) throw ( NoEntryException )
{
	if( _standards.find( name ) == _standards.end() )
		throw NoEntryException();

	if( version != NULL )
		*version = (*_standards.find( name )).second->getVersion();
	if( id != NULL )
		*id = (*_lutIndex.find( name )).second;
}

void Database::declareOperation( XLstdid stdid, XLint32 opid ) throw ( ExistsException )
{
	// Operation overlapping is checked in declareOperation().
	stdLut[stdid]->declareOperation( opid );
}

void Database::declareOperationDomain( XLstdid stdid, XLint32 fromOpid, XLint32 toOpid ) throw ( ExistsException )
{
	// Operation overlapping is checked in declareOperationDomain().
	stdLut[stdid]->declareOperationDomain( fromOpid, toOpid );
}

void Database::createEngineLibrary( const std::string& name, const XLversion& version, XLlibid* libid ) throw ( ExistsException )
{
	EngineLibrary*		engLib;

	if( _libraries.find( name ) != _libraries.end() )
		throw ExistsException();

	// Allocate space for a new library. We do like this to get the libid.
	_libraries[ name ] = NULL;
	*libid = _libraries.size() - 1; // -1 to get the right position in the array.
	engLib = new EngineLibrary( *libid, name, version );
	_libraries[ name ] = engLib;

	_libLut = (Library**)realloc( _libLut, _libraries.size() * sizeof(Library*) );
	_libLut[ *libid ] = engLib;
}

void Database::createUserLibrary( const std::string& name, const XLversion& version, XLlibid* libid ) throw ( ExistsException )
{
	UserLibrary*		usrLib;

	if( _libraries.find( name ) != _libraries.end() )
		throw ExistsException();

	// Allocate space for a new library. We do like this to get the libid.
	_libraries[ name ] = NULL;
	*libid = _libraries.size() - 1; // -1 to get the right position in the array.
	usrLib = new UserLibrary( *libid, name, version );
	_libraries[ name ] = usrLib;

	_libLut = (Library**)realloc( _libLut, _libraries.size() * sizeof(Library*) );
	_libLut[ *libid ] = usrLib;
}

void Database::registerDefinition( XLlibid libid, XLstdid stdid, XLint32 opid, const std::string& defName, const FncPtr& fncptr ) throw ( NoEntryException )
{
	_libLut[libid]->addFunction( stdid, opid, defName, fncptr );
}

void Database::registerDefinition( XLlibid libid, XLstdid stdid, XLint32 opid, const std::string& defName, XLfncptr fncptr ) throw ( NoEntryException )
{
	_libLut[libid]->addFunction( stdid, opid, defName, StaticFncPtr( fncptr ) );
}

Standard* Database::standard( XLstdid stdid )
{
	return stdLut[stdid];
}

Operation* Database::operation( XLstdid stdid, XLint32 opid )
{
	return stdLut[stdid]->operation( opid );
}

Library* Database::library( XLlibid libid )
{
	return _libLut[libid];
}

void Database::connectDatabase( char* server, char* user, char* passwd, char* database )
{
	_mySQL = mysql_init( NULL );

	if( !mysql_real_connect( _mySQL, server, user, passwd, database, 0, NULL, 0 ) )
		fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

	// Following makes sure the disk database is properly set.

	// Create a database called 'libraries' to contain all libraries created.
	if( mysql_query( _mySQL, "CREATE DATABASE IF NOT EXISTS Libraries" ) != 0 )
		fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

	// Create a table for all definitions in each library.
	// Note! This table is only reflecting the base class Definition, and not for example
	// UserDefinition. Moreover, as for now this actually reflects Function better, but
	// the aim is to reflect Definition.
	std::string sqlString( "CREATE TABLE IF NOT EXISTS Libraries.Definitions " );
	sqlString += "(LibName CHAR(255) NOT NULL, FncName CHAR(255) NOT NULL, StdId INT, OpId INT, SpecId INT, PRIMARY KEY(LibName, FncName))";

	if( mysql_query( _mySQL, sqlString.c_str() ) != 0 )
		fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

	sqlString = "CREATE TABLE IF NOT EXISTS Libraries.UserDefinitions ";
	sqlString += "(SpecId INT AUTO_INCREMENT NOT NULL, etocC0 DOUBLE, etocC1 DOUBLE, etocC2 DOUBLE, PRIMARY KEY(SpecId))";

	if( mysql_query( _mySQL, sqlString.c_str() ) != 0 )
		fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

	sqlString = "CREATE TABLE IF NOT EXISTS Libraries.UserDefinitionTimingSamples ";
	sqlString += "(SpecId INT NOT NULL, ArgSize DOUBLE, Time BIGINT)";

	if( mysql_query( _mySQL, sqlString.c_str() ) != 0 )
		fprintf( stderr, "%s\n", mysql_error( _mySQL ) );


	// If we managed to connect then update all definitions.
	_readDefsFromDB();



/*	Test
	if( mysql_query( _mySQL, "SHOW DATABASES" ) != 0 )
	{
		fprintf( stderr, "%s\n", mysql_error( _mySQL ) );
	}

	if( mysql_field_count( _mySQL ) > 0 )
	{
		MYSQL_RES*	result;
		MYSQL_ROW	row, endRow;
		int			nFields;

		result = mysql_store_result( _mySQL );
		nFields = mysql_num_fields( result );

		while( (row = mysql_fetch_row( result )) )
			for( int i = 0; i < nFields; i++ )
				fprintf( stderr, "%s\n", row[i] != NULL ? row[i] : "NULL" );
		mysql_free_result( result );
	}
	*/
}

void Database::connectAllLibraries( void )
{
	for( std::map<std::string, Standard*>::iterator i = _standards.begin(); i != _standards.end(); ++i )
		for( std::map<std::string, Library*>::iterator j = _libraries.begin(); j != _libraries.end(); ++j )
			(*i).second->connectLibrary( (*j).second );

	// If a disk database is connected then update all definitions.
	_readDefsFromDB();

}


void Database::_readDefsFromDB( void )
{
	std::stringstream	sqlString;
	MYSQL_RES*			result;
	MYSQL_ROW			row;
	int					nFields;


	if( _mySQL != NULL )
	{
		// Iterate through all the definitions in all operations in all standards.
		for( std::map<std::string, Standard*>::iterator i = _standards.begin(); i != _standards.end(); ++i )
		{
			std::list<Operation*>	opList;
			(*i).second->getOperationList( &opList );

			for( std::list<Operation*>::iterator j = opList.begin(); j != opList.end(); ++j )
			{
				std::list<Definition*>	defList;
				(*j)->getDefinitionList( &defList );

				for( std::list<Definition*>::iterator k = defList.begin(); k != defList.end(); ++k )
				{
					UserDefinition*	userDef;

					// This is a quick-and-dirty solution. Should be solved with an interface.
					userDef = dynamic_cast<UserDefinition*>( (*k) );
					if( userDef == NULL )
						continue;

					// Look for previously stored etoc data for this definition.
					sqlString.str( "" );
					sqlString << "SELECT Libraries.UserDefinitions.etocC0, Libraries.UserDefinitions.etocC1, Libraries.UserDefinitions.etocC2";
					sqlString << " FROM Libraries.Definitions, Libraries.UserDefinitions WHERE Libraries.Definitions.LibName = '";
					sqlString << _libLut[ userDef->getFunction()->getLibraryId() ]->getName() << "'";
					sqlString << " AND Libraries.Definitions.FncName = '" << userDef->getFunction()->getName() << "'";
					sqlString << " AND Libraries.UserDefinitions.SpecId = Libraries.Definitions.SpecId";

					if( mysql_query( _mySQL, sqlString.str().c_str() ) != 0 )
						fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

					// Step through the MySQL result.
					if( (result = mysql_store_result( _mySQL )) == NULL )
						fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

					// If the number of rows is more than 0 it means the entry exists in the databse.
					if( mysql_num_rows( result ) > 0 )
					{
						row = mysql_fetch_row( result );
						userDef->setEtoc( strtod( row[0], NULL ), strtod( row[1], NULL ), strtod( row[2], NULL ) );

						mysql_free_result( result );
					}
					else
					{
						// If it's not in the database then make a new entry for it.

						// First create the user definition to generate the autoincrementing number.
						sqlString.str( "" );
						sqlString << "INSERT INTO Libraries.UserDefinitions (etocC0, etocC1, etocC2) VALUES (0.0, 0.0, 0.0)";

						if( mysql_query( _mySQL, sqlString.str().c_str() ) != 0 )
							fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

						// Then retrieve the autoincrementing number...
						sqlString.str( "" );
						sqlString << "SELECT last_insert_id() FROM Libraries.UserDefinitions";

						if( mysql_query( _mySQL, sqlString.str().c_str() ) != 0 )
							fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

						if( (result = mysql_store_result( _mySQL )) == NULL )
							fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

						row = mysql_fetch_row( result );

						// ... and create the main definition entry with that number.
						sqlString.str( "" );
						sqlString << "INSERT INTO Libraries.Definitions (LibName, FncName, StdId, OpId, SpecId)";
						sqlString << " VALUES ('" << _libLut[ userDef->getFunction()->getLibraryId() ]->getName() << "'";
						sqlString << ", '" << userDef->getFunction()->getName() << "'";
						sqlString << ", " << userDef->getStandardId();
						sqlString << ", " << userDef->getOperationId();
						sqlString << ", " << row[0] << ")";

						// Now when we've used row[0] we can free this.
						mysql_free_result( result );

						// Execute the creation of the insert.
						if( mysql_query( _mySQL, sqlString.str().c_str() ) != 0 )
							fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

					}

				}
			}
		}
	}
}

void Database::_writeDefsToDB( void )
{
	std::stringstream	sqlString;
	MYSQL_RES*			result;
	MYSQL_ROW			row;


	if( _mySQL != NULL )
	{
		// Iterate through all the definitions in all operations in all standards.
		for( std::map<std::string, Standard*>::iterator i = _standards.begin(); i != _standards.end(); ++i )
		{
			std::list<Operation*>	opList;
			(*i).second->getOperationList( &opList );

			for( std::list<Operation*>::iterator j = opList.begin(); j != opList.end(); ++j )
			{
				std::list<Definition*>	defList;
				(*j)->getDefinitionList( &defList );

				for( std::list<Definition*>::iterator k = defList.begin(); k != defList.end(); ++k )
				{
					UserDefinition*	userDef;

					// This is a quick-and-dirty solution. Should be solved with an interface.
					userDef = dynamic_cast<UserDefinition*>( (*k) );
					if( userDef == NULL )
						continue;

					// Get the SpecId for this definition to be used when storing timings for this definition.
					sqlString.str( "" );
					sqlString << "SELECT SpecId FROM Libraries.Definitions WHERE";
					sqlString << " Libraries.Definitions.LibName = '" << _libLut[ userDef->getFunction()->getLibraryId() ]->getName() << "'";
					sqlString << " AND Libraries.Definitions.FncName = '" << userDef->getFunction()->getName() << "'";

					if( mysql_query( _mySQL, sqlString.str().c_str() ) != 0 )
						fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

					if( (result = mysql_store_result( _mySQL )) == NULL )
						fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

					row = mysql_fetch_row( result );
					int specId = atoi( row[0] );
					mysql_free_result( result );

					// Store todays timing samples.
					std::vector<std::pair<XLreal32, XLint64> >		timings;
					userDef->getTimings( &timings );

					for( int k = 0; k < timings.size(); k++ )
					{
						sqlString.str( "" );
						sqlString << "INSERT INTO Libraries.UserDefinitionTimingSamples";
						sqlString << " (Libraries.UserDefinitionTimingSamples.SpecId";
						sqlString << ", Libraries.UserDefinitionTimingSamples.ArgSize";
						sqlString << ", Libraries.UserDefinitionTimingSamples.Time) VALUES";
						sqlString << " (" << specId << ", " << timings[k].first << ", " << timings[k].second << ")";

						if( mysql_query( _mySQL, sqlString.str().c_str() ) != 0 )
							fprintf( stderr, "%s\n", mysql_error( _mySQL ) );
					}

					// Now retrieve all historical timing samples and create an etoc curve out of it.
					sqlString.str( "" );
					sqlString << "SELECT ArgSize, Time FROM Libraries.UserDefinitionTimingSamples WHERE SpecId = " << specId;

					if( mysql_query( _mySQL, sqlString.str().c_str() ) != 0 )
						fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

					// Step through the MySQL result.
					if( (result = mysql_store_result( _mySQL )) == NULL )
						fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

					// We need at least 3 samples before GSL can make a curve out of the samples.
					if( mysql_num_rows( result ) >= 3 )
					{
						int									n;
						double								chiSq;
						gsl_matrix*							A;
						gsl_vector*							c;
						gsl_vector*							y;
						gsl_matrix*							cov;

						// Use Least Square Fitting method to calculate an x^2 curve that will be
						// used for future estimations (Ac=y). 3 for the polynomial of size 2 +1 for the constant.
						n = mysql_num_rows( result );
						A = gsl_matrix_alloc( n, 3 );
						c = gsl_vector_alloc( 3 );
						y = gsl_vector_alloc( n );
						cov = gsl_matrix_alloc( 3, 3 );

						// Build the input data by row.
						for( int i = 0; i < n; i++ )
						{
							row = mysql_fetch_row( result );

							double xi = strtod( row[0], NULL );	// Row[0] is argument size.
							gsl_matrix_set( A, i, 0, 1.0 ); gsl_matrix_set( A, i, 1, xi ); gsl_matrix_set( A, i, 2, xi*xi );

							gsl_vector_set( y, i, strtod( row[1], NULL ) );	// row[1] is processing time.
						}

						mysql_free_result( result );

						// Do the calculation.
						gsl_multifit_linear_workspace* work = gsl_multifit_linear_alloc( n, 3 );
						gsl_multifit_linear( A, y, c, cov, &chiSq, work );
						gsl_multifit_linear_free( work );


						// Here we get the coefficients we are interested in.
						// These coefficients explains the ETOC curve for the operation.
						// Store the etoc curve to the database.
						sqlString.str( "" );
						sqlString << "UPDATE Libraries.Definitions, Libraries.UserDefinitions";
						sqlString << " SET Libraries.UserDefinitions.etocC0 = " << gsl_vector_get( c, 0 );
						sqlString << ", Libraries.UserDefinitions.etocC1 = " << gsl_vector_get( c, 1 );
						sqlString << ", Libraries.UserDefinitions.etocC2 = " << gsl_vector_get( c, 2 );
						sqlString << " WHERE Libraries.Definitions.LibName = '" << _libLut[ userDef->getFunction()->getLibraryId() ]->getName() << "'";
						sqlString << " AND Libraries.Definitions.FncName = '" << userDef->getFunction()->getName() << "'";
						sqlString << " AND Libraries.UserDefinitions.SpecId = Libraries.Definitions.SpecId";

						if( mysql_query( _mySQL, sqlString.str().c_str() ) != 0 )
							fprintf( stderr, "%s\n", mysql_error( _mySQL ) );

						gsl_matrix_free( A );
						gsl_vector_free( c );
						gsl_vector_free( y );
						gsl_matrix_free( cov );

					}

				}
			}
		}
	}
}



// This might be a standard in the future.
/*
void Database::createMap( XLmapid* id )
{
	_opMaps.push_back( new OpMap() );
	*id = _opMaps.size() - 1;
}

void Database::mapOperation( XLmapid mapid, XLstdid stdid, XLint32 opid, XLint32 pos ) throw ( NoEntryException, ExistsException )
{
	OpMap*		opMap = _opMaps[mapid];

	// First make sure we don't overlap any previous operation.
	if( opMap->find( pos ) != opMap->end() )
		throw ExistsException();

	// Then make sure we actually have that operation we want to map.
	if( stdLut[stdid]->operations.find( opid ) == stdLut[stdid]->operations.end() )
		throw NoEntryException();

	(*opMap)[pos] = stdLut[stdid]->operations[opid];
}

void Database::mapOperationDomain( XLmapid mapid, XLstdid stdid, XLint32 fromOpid, XLint32 toOpid, XLint32 startPos ) throw ( NoEntryException, ExistsException )
{
	// Loop through the specified domain.
	for( XLint32 opid = fromOpid, i = 0; opid <= toOpid; opid++, i++ )
		mapOperation( mapid, stdid, opid, startPos+i );
}

void Database::mapStandard( XLmapid mapid, XLstdid stdid ) throw ( ExistsException )
{
	OpMap*		opMap = _opMaps[mapid];

	// Go through all operations in the standard and add them to the opMap.
	// We don't use mapOperation() to do this since that one also go through the
	// same map which could confuse the loop.
	for( OpMap::iterator i = stdLut[stdid]->operations.begin(); i != stdLut[stdid]->operations.end(); ++i )
	{
		// Make sure we don't overlap any previous operation.
		if( opMap->find( (*i).first ) != opMap->end() )
			throw ExistsException();

		(*opMap)[(*i).first] = (*i).second;
	}
}

void Database::setLibraryMap( XLlibid libid, XLmapid mapid )
{
}
*/





















