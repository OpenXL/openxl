/*

   EngineStandard.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef ENGINESTANDARD_HH_
#define ENGINESTANDARD_HH_

#include "OpMap.hh"
#include "Standard.hh"


class EngineStandard : public Standard
{

public:

	EngineStandard( XLstdid stdid, const std::string& stdName, const XLversion& stdVersion );
	virtual ~EngineStandard();


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
