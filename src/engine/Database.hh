/*

   Database.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef DATABASE_HH_
#define DATABASE_HH_

#include <XL/xltypes.h>
#include "exception/ExistsException.hh"
#include "exception/NoEntryException.hh"
#include <map>
#include <string>

#include <mysql/mysql.h>


class FncPtr;
class Standard;
class Operation;
class Library;



class Database
{

public:

	Standard**									stdLut;	// To make fast lookups.


	Database();
	virtual ~Database();

	// OpenXL related

	void			createEngineStandard( const std::string& name, const XLversion& version, XLstdid* stdid ) throw ( ExistsException );
	void			createUserStandard( const std::string& name, const XLversion& version, XLstdid* stdid ) throw ( ExistsException );
	void			getStandard( const std::string& name, XLversion* version, XLstdid* id ) throw ( NoEntryException );
	void			declareOperation( XLstdid stdid, XLint32 opid ) throw ( ExistsException );
	void			declareOperationDomain( XLstdid stdid, XLint32 fromOpid, XLint32 toOpid ) throw ( ExistsException );

	void			createEngineLibrary( const std::string& name, const XLversion& version, XLlibid* libid ) throw ( ExistsException );
	void			createUserLibrary( const std::string& name, const XLversion& version, XLlibid* libid ) throw ( ExistsException );
	void			registerDefinition( XLlibid libid, XLstdid stdid, XLint32 opid, const std::string& defName, const FncPtr& fncptr ) throw ( NoEntryException );
	void			registerDefinition( XLlibid libid, XLstdid stdid, XLint32 opid, const std::string& defName, XLfncptr fncptr ) throw ( NoEntryException );

	Standard*	standard( XLstdid stdid );
	Operation*	operation( XLstdid stdid, XLint32 opid );
	Library*		library( XLlibid libid );


	// This might be a standard in the future.
	//void		createMap( XLmapid* id );
	//void		mapOperation( XLmapid mapid, XLstdid stdid, XLint32 opid, XLint32 pos ) throw ( NoEntryException, ExistsException );
	//void		mapOperationDomain( XLmapid mapid, XLstdid stdid, XLint32 fromOpid, XLint32 toOpid, XLint32 startPos ) throw ( NoEntryException, ExistsException );
	//void		mapStandard( XLmapid mapid, XLstdid stdid ) throw ( ExistsException );

	//void		setLibraryMap( XLlibid libid, XLmapid mapid );


	// Performance Monitor Standard.
	void			connectDatabase( char* server, char* user, char* passwd, char* database );


	// Setup Engine Standard.
	void			connectAllLibraries( void );



private:

	std::map<std::string, Standard*>									_standards;
	std::map<std::string, XLstdid>										_lutIndex;
	std::map<std::string, Library*>									_libraries;
	Library**														_libLut;

	MYSQL*																_mySQL;


	void			_readDefsFromDB( void );
	void			_writeDefsToDB( void );




};

#endif




