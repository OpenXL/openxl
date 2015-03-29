/*

   Engine.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef ENGINE_HH_
#define ENGINE_HH_

#include "estd/PerformanceMonitor.hh"
#include "estd/Scheduler.hh"
#include "estd/FpgaScheduler.hh"
#include "estd/ConsolePort.hh"
#include <XL/xltypes.h>

#include <string>
#include "Database.hh"


// Maybe  and Engine could be merged for better performance.
class Engine
{

public:

	// This should be renamed later to DatabaseServer
	Database					database;


	Engine();
	virtual ~Engine();

	// OpenXL related

	XLstatus	xlClose( void );

	XLstatus	xlCreateStd( char* name, XLversion* version, XLstdid* stdid );
	XLstatus	xlGetStd( char* name, XLversion* version, XLstdid* stdid );
	XLstatus	xlDec( XLstdid stdid, XLint32 opid );
	XLstatus	xlDecDom( XLstdid stdid, XLint32 fromOpid, XLint32 toOpid );

	XLstatus	xlCreateLib( char* name, XLversion* version, XLlibid* libid );
	XLstatus	xlRegDef( XLlibid libid, XLstdid stdid, XLint32 opid, char* defName, XLfncptr def );


	// Setup Engine Standard. (Only used by xlOpen so far)
	void		connectAllLibraries( void );

	// Console Port Engine Standard. (Only internal)
	void		registerConsolePortCallback( const std::string& stdName, XLint32 op, const FncPtr& callbackFnc );


private:

	PerformanceMonitor		_perfMon;
	Scheduler				_scheduler;
	FpgaScheduler			_fpga;
	ConsolePort				_consolePort;






};

#endif
