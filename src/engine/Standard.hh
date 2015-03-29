/*

   Standard.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef STANDARD_HH_
#define STANDARD_HH_

#include <XL/xltypes.h>
#include "exception/ExistsException.hh"
#include <list>

class Operation;
class Library;



class Standard
{

public:

	Standard( XLstdid stdid, const std::string& stdName, const XLversion& stdVersion );
	virtual ~Standard();

	const std::string&		getName( void ) const;
	XLversion				getVersion( void ) const;

	virtual void			declareOperation( XLint32 opid ) throw ( ExistsException ) = 0;
	virtual void			declareOperationDomain( XLint32 fromOpid, XLint32 toOpid ) throw ( ExistsException ) = 0;

	virtual void			connectLibrary( Library* library ) = 0;

	virtual Operation*	operation( XLint32 opid ) = 0;
	virtual void			getOperationList( std::list<Operation*>* operationList ) = 0;
	virtual XLstatus		runOp( XLint32 opid, XLpv* arg, XLpv* result ) = 0;


protected:

	XLstdid					_standardId;
	std::string				_name;
	XLversion				_version;



};

#endif
