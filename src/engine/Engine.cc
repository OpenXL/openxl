/*

   Engine.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "Engine.hh"
#include <XL/xldefs.h>


Engine::Engine()
{
	_perfMon.init( this );
	_scheduler.init( this );
	_fpga.init( this );

}

Engine::~Engine()
{
}

XLstatus Engine::xlClose( void )
{
	_consolePort.closePort( NULL, NULL );
	return XL_STATUS_NO_ERROR;
}

XLstatus Engine::xlCreateStd( char* name, XLversion* version, XLstdid* stdid )
{
	// Engine standards ain't created with xlCreateStd here.
	try { database.createUserStandard( name, *version, stdid ); }
	catch( ExistsException& e )
	{
		return XL_STATUS_EXISTS;
	}

	return XL_STATUS_NO_ERROR;
}

XLstatus Engine::xlGetStd( char* name, XLversion* version, XLstdid* stdid )
{
	try { database.getStandard( name, version, stdid ); }
	catch( NoEntryException& e )
	{
		return XL_STATUS_NO_ENTRY;
	}

	return XL_STATUS_NO_ERROR;
}

XLstatus Engine::xlDec( XLstdid stdid, XLint32 opid )
{
	try { database.declareOperation( stdid, opid ); }
	catch( ExistsException& e )
	{
		return XL_STATUS_EXISTS;
	}

	return XL_STATUS_NO_ERROR;
}

XLstatus Engine::xlDecDom( XLstdid stdid, XLint32 fromOpid, XLint32 toOpid )
{
	try { database.declareOperationDomain( stdid, fromOpid, toOpid ); }
	catch( ExistsException& e )
	{
		return XL_STATUS_EXISTS;
	}

	return XL_STATUS_NO_ERROR;
}

XLstatus Engine::xlCreateLib( char* name, XLversion* version, XLlibid* libid )
{
	// Engine libraries ain't created with xlCreateLib here.
	try { database.createUserLibrary( name, *version, libid ); }
	catch( ExistsException& e )
	{
		return XL_STATUS_EXISTS;
	}

	return XL_STATUS_NO_ERROR;
}

XLstatus Engine::xlRegDef( XLlibid libid, XLstdid stdid, XLint32 opid, char* defName, XLfncptr def )
{
	try { database.registerDefinition( libid, stdid, opid, defName, def ); }
	catch( NoEntryException& e )
	{
		return XL_STATUS_NO_ENTRY;
	}

	return XL_STATUS_NO_ERROR;
}

void Engine::connectAllLibraries( void )
{
	database.connectAllLibraries();
}

// Console Port Engine Standard. (Only internal)
void Engine::registerConsolePortCallback( const std::string& stdName, XLint32 op, const FncPtr& callbackFnc )
{
	_consolePort.registerConsolePortCallback( stdName, op, callbackFnc );
}


