/*

   UserStandard.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef USERSTANDARD_HH_
#define USERSTANDARD_HH_

#include "OpMap.hh"
#include "Standard.hh"


class UserStandard : public Standard
{

public:

	UserStandard( XLstdid stdid, const std::string& stdName, const XLversion& stdVersion );
	virtual ~UserStandard();


	virtual void			declareOperation( XLint32 opid ) throw ( ExistsException );
	virtual void			declareOperationDomain( XLint32 fromOpid, XLint32 toOpid ) throw ( ExistsException );

	virtual void			connectLibrary( Library* library );

	virtual Operation*	operation( XLint32 opid );
	virtual void			getOperationList( std::list<Operation*>* operationList );
	virtual XLstatus		runOp( XLint32 opid, XLpv* arg, XLpv* result );


private:

	OpMap				_operations;


};

#endif
