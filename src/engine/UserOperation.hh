/*

   UserOperation.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef USEROPERATION_HH_
#define USEROPERATION_HH_

#include "Operation.hh"

class UserDefinition;


class UserOperation : public Operation
{

public:

	UserOperation( XLstdid stdid, XLint32 opid );
	virtual ~UserOperation();

	virtual bool				multipleDefinitionsAllowed( void ) const;
	virtual void				registerDefinition( Function* function );
	virtual void				getDefinitionList( std::list<Definition*>* definitionList );
	virtual XLstatus			run( XLpv* arg, XLpv* result );


	// User Operation Specific.

	void						registerArgGenerator( XLfncptr fnc );
	void						registerArgAnalyzer( XLfncptr fnc );

	void						enableDefinitions( const std::list<Definition*>& definitionList );
	void						disableDefinitions( const std::list<Definition*>& definitionList );

	void						calculateAllDefinitionsEtoc( void );

	XLstatus					getEtoc( XLpv* arg, XLpv* result );


private:

	UserDefinition**			_definitions;
	XLint32						_defListSize;
	UserDefinition**			_enabledDefinitions;
	XLint32						_enabledDefListSize;


	// These are used to help the definition decision engine.
	XLfncptr					_argGenerator;
	XLfncptr					_argAnalyzer;


};

#endif
