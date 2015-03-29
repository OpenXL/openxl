/*

   UserDefinition.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef USERDEFINITION_HH_
#define USERDEFINITION_HH_

#include <vector>
#include "Definition.hh"

class Function;


class UserDefinition : public Definition
{

public:


	UserDefinition( XLstdid stdid, XLint32 opid, Function* function );
	virtual ~UserDefinition();

	virtual Function*			getFunction( void ) const;

	virtual XLstatus				exec( XLpv* arg, XLpv* result );

	void							setArgumentAnalyzer( XLfncptr fnc );
	void							keepStatistics( bool ks );
	void							getTimings( std::vector<std::pair<XLreal32, XLint64> >* timings ) const;
	void							calculateEtocFromStatistics( void );
	void							calculateEtoc( XLfncptr argGenerator );
	XLint64							getEtoc( XLreal64 argSize ) const;
	XLint64							getEtoc( XLpv* arg ) const;
	void							getEtoc( XLpv* arg, XLint32* h, XLint32* m, XLint32* s, XLint32* us ) const;

	XLreal64						getEtocC0( void ) const;
	XLreal64						getEtocC1( void ) const;
	XLreal64						getEtocC2( void ) const;

	void							setEtoc( XLreal64 etocC0, XLreal64 etocC1, XLreal64 etocC2 );



private:

	Function*					_function;


	// These coefficients represent the curve C0 + C1*x + C2*x^2.
	// That's used to give a hint of this definition's ETOC.
	XLreal64						_etocC0;
	XLreal64						_etocC1;
	XLreal64						_etocC2;

	// For keeping statistics.
	bool							_keepStatistics;
	XLfncptr						_argumentAnalyzer;
	std::vector<XLreal32>			_argumentSize;
	std::vector<XLint64>			_processingTime;



};


#endif
