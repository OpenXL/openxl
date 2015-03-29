/*

   XL.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include <XL/xl.h>
#include <XL/xldefs.h>
#include "DXLEngine.hh"
#include "DXLStandard.hh"

DXLEngine					engine;
DXLStandard**&				stdLut = engine.database.stdLut;


extern XLfncptr				xlStandards[];
extern XLfncptr				xlLibraries[];
extern XLfncptr				xlSetups[];



// Engine Initialization Part.
XLstatus XLAPI xlOpen( XLpv* params )
{
	// Maybe the status returned should be better processed.
	XLstatus	status;


	// First load all user standards.
	// Here all standards are created and their operations are declared.
	for( int i = 0; ; i++ )
	{
		if( xlStandards[i] == NULL )
			break;
		if( (status = xlStandards[i]( NULL, NULL )) != XL_STATUS_NO_ERROR )
			return status;
	}

	// Then load all user libraries.
	// Here all operations are defined.
	for( int i = 0; ; i++ )
	{
		if( xlLibraries[i] == NULL )
			break;
		if( (status = xlLibraries[i]( NULL, NULL )) != XL_STATUS_NO_ERROR )
			return status;
	}

	// Then connect all libraries to the standards. This part could actually
	// be something the user could customize himself.
	engine.connectAllLibraries();


	// Then load all extras.
	// That could for instance be engine specific things like setting the
	// argument generator.
	for( int i = 0; ; i++ )
	{
		if( xlSetups[i] == NULL )
			break;
		if( (status = xlSetups[i]( NULL, NULL )) != XL_STATUS_NO_ERROR )
			return status;
	}

	return XL_STATUS_NO_ERROR;
}


XLstatus XLAPI xlClose()
{
	return engine.xlClose();
}


// XLstatus XLAPI xlQuery()?



// Standard Creation Part.
XLstatus XLAPI xlCreateStd( char* name, XLversion* version, XLstdid* stdid )
{
	return engine.xlCreateStd( name, version, stdid );
}

/*XLstatus XLAPI xlExtendStd( char* name, XLversion* newVersion, XLstdid* stdid )
{
	return XL_STATUS_NO_ERROR;
}*/

XLstatus XLAPI xlGetStd( char* name, XLversion* version, XLstdid* stdid )
{
	return engine.xlGetStd( name, version, stdid );
}

/*XLstatus XLAPI xlGetVer( XLstdid stdid, XLversion* version )
{
	return XL_STATUS_NO_ERROR;
}*/

// Standard Declaration Part. Declare Domain.
XLstatus XLAPI xlDec( XLstdid stdid, XLint32 opid )
{
	return engine.xlDec( stdid, opid );
}

XLstatus XLAPI xlDecDom( XLstdid stdid, XLint32 fromOpid, XLint32 toOpid )
{
	return engine.xlDecDom( stdid, fromOpid, toOpid );
}



// Library Definition Part.
XLstatus XLAPI xlCreateLib( char* name, XLversion* version, XLlibid* libid )
{
	return engine.xlCreateLib( name, version, libid );
}

XLstatus XLAPI xlRegDef( XLlibid libid, XLstdid stdid, XLint32 opid, char* defName, XLfncptr def )
{
	return engine.xlRegDef( libid, stdid, opid, defName, def );
}


XLstatus XLAPI xlOp( XLstdid stdid, XLint32 opid, XLpv* arg, XLpv* result )
{
	return stdLut[stdid]->runOp( opid, arg, result );
}




