/*

   UserDefinition.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "UserDefinition.hh"
#include "Function.hh"
#include "XL/xldefs.h"
#include "dixle.h"

#include <sys/time.h>
#include <sys/resource.h>
#include <gsl/gsl_multifit.h>
#include <vector>


UserDefinition::UserDefinition( XLstdid stdid, XLint32 opid, Function* function )
	: Definition( stdid, opid ), _function(function), _etocC0(0.0), _etocC1(0.0), _etocC2(0.0)
{
	_argumentSize.clear();
	_processingTime.clear();
	_argumentAnalyzer = NULL;
	_keepStatistics = true;
}

UserDefinition::~UserDefinition()
{
	// Do not delete _function here.
}

Function* UserDefinition::getFunction( void ) const
{
	return _function;
}

XLstatus UserDefinition::exec( XLpv* arg, XLpv* result )
{
	XLpv				argSize;
	XLstatus			status;
	struct rusage		cpuUsageStart, cpuUsageStop;
	XLint64				uSecs;


	if( _keepStatistics )
	{
		// Get the argument size.
		_argumentAnalyzer( arg, &argSize );

		// Clock the operation.
		getrusage( RUSAGE_SELF, &cpuUsageStart );
	}

	// Run the operation.
	status = _function->exec( arg, result );

	if( _keepStatistics && status == XL_STATUS_NO_ERROR )
	{
		// End of timing.
		getrusage( RUSAGE_SELF, &cpuUsageStop );

		uSecs = (cpuUsageStop.ru_utime.tv_sec * 1000000L + cpuUsageStop.ru_utime.tv_usec) -
				(cpuUsageStart.ru_utime.tv_sec * 1000000L + cpuUsageStart.ru_utime.tv_usec);

		_argumentSize.push_back( argSize.value.real32 );
		_processingTime.push_back( uSecs );
	}

	return status;
}

void UserDefinition::setArgumentAnalyzer( XLfncptr fnc )
{
	_argumentAnalyzer = fnc;
	// We can't keep any statistics unless we have some kind of argument analyzer.
	if( fnc == NULL )
		_keepStatistics = false;
	else
		_keepStatistics = true;
		// This may be changed in the future.
}

void UserDefinition::keepStatistics( bool ks )
{
	// We can't keep any statistics unless we have some kind of argument analyzer.
	// Maybe we should give a warning here somehow.
	if( _argumentAnalyzer == NULL )
		return;
	_keepStatistics = ks;
}

void UserDefinition::getTimings( std::vector<std::pair<XLreal32, XLint64> >* timings ) const
{
	for( int i = 0; i < _argumentSize.size(); i++ )
		timings->push_back( std::pair<XLreal32, XLint64>( _argumentSize[i], _processingTime[i] ) );
}

void UserDefinition::calculateEtocFromStatistics( void )
{
	int									n;
	double								chiSq;
	gsl_matrix*							A;
	gsl_vector*							c;
	gsl_vector*							y;
	gsl_matrix*							cov;


fprintf( stderr, "UserDefinition::calculateEtocFromStatistics\n" );

	// GSL cannot handle this calculation with less than three values.
	if( _argumentSize.size() < 3 )
		return;

	// Use Least Square Fitting method to calculate an x^2 curve that will be
	// used for future estimations (Ac=y). 3 for the polynomial of size 2 +1 for the constant.
	n = _argumentSize.size();
	A = gsl_matrix_alloc( n, 3 );
	c = gsl_vector_alloc( 3 );
	y = gsl_vector_alloc( n );
	cov = gsl_matrix_alloc( 3, 3 );

	// Build the input data by row.
	for( int i = 0; i < n; i++ )
	{
		double xi = _argumentSize[i];
		gsl_matrix_set( A, i, 0, 1.0 ); gsl_matrix_set( A, i, 1, xi ); gsl_matrix_set( A, i, 2, xi*xi );

		gsl_vector_set( y, i, _processingTime[i] );
	}

	// Do the calculation.
	gsl_multifit_linear_workspace* work = gsl_multifit_linear_alloc( n, 3 );
	gsl_multifit_linear( A, y, c, cov, &chiSq, work );
	gsl_multifit_linear_free( work );

	// Here we get the coefficients we are interested in.
	// These coefficients explains the ETOC curve for the operation.
	_etocC0 = gsl_vector_get( c, 0 );
	_etocC1 = gsl_vector_get( c, 1 );
	_etocC2 = gsl_vector_get( c, 2 );

	gsl_matrix_free( A );
	gsl_vector_free( c );
	gsl_vector_free( y );
	gsl_matrix_free( cov );


}

void UserDefinition::calculateEtoc( XLfncptr argGenerator )
{
	XLpv								argGenArg;
	XLpv								argGenRes[2];

	std::vector<XLreal32>				argSize;
	std::vector<XLint64>				time;

	struct rusage						cpuUsage;
	XLint64								uSecs;

	int									nSamples = 48;

	int									n;
	double								chiSq;
	gsl_matrix*							A;
	gsl_vector*							c;
	gsl_vector*							y;
	gsl_matrix*							cov;


	// The argument generator shall take the param SIZE and return SIZE + an argument of
	// that size. If NULL is passed as SIZE then a new good benchmarking argument + SIZE is
	// generated each call (preferably increasing in size).


	// This resets the generator and makes it start generating arguments from size 0.0.
	// Actually the generator itself chooses the min value.
	argGenArg.param = DXE_ARG_GEN_INIT;
	argGenArg.value.real32 = 0.0;
	argGenerator( &argGenArg, NULL );

	// Now tell the generator that the rest of the arguments shall be generated
	// with that size on.
	argGenArg.param = DXE_ARG_GEN_GENERATE;

	// First cue the size up until we get a size that takes more than uCueTime.
	XLint64 uCueTime = 0;
	while( uCueTime < 100000L )
	{
		argGenerator( &argGenArg, argGenRes );
		getrusage( RUSAGE_SELF, &cpuUsage );
		uCueTime = cpuUsage.ru_utime.tv_sec * 1000000L + cpuUsage.ru_utime.tv_usec;

		// Run the implementation but skip the result.
		_function->exec( argGenRes[1].value.pPv, NULL );

		getrusage( RUSAGE_SELF, &cpuUsage );
		uCueTime = cpuUsage.ru_utime.tv_sec * 1000000L + cpuUsage.ru_utime.tv_usec - uCueTime;
	}

	// Clock the function for all different arguments in the test.
	for( int i = 0; i < nSamples; i++ )
	{
		// Generate an argument. We expect two XLpv back. The first is SIZE, and the
		// second is the argument. The previous argument will be freed automatically.
		argGenerator( &argGenArg, argGenRes );
		// Save the size. We're gonna use it together with the time later.
		argSize.push_back( argGenRes[0].value.real32 );

		// Clock the operation.
		getrusage( RUSAGE_SELF, &cpuUsage );
		uSecs = cpuUsage.ru_utime.tv_sec * 1000000L + cpuUsage.ru_utime.tv_usec;
		time.push_back( uSecs );

		// Run the implementation with the argument we got from the generator.
		// We don't care about the result (NULL). 1 = the argument, 0 = the size.
		_function->exec( argGenRes[1].value.pPv, NULL );

		// End of timing.
		getrusage( RUSAGE_SELF, &cpuUsage );
		uSecs = cpuUsage.ru_utime.tv_sec * 1000000L + cpuUsage.ru_utime.tv_usec;
		time[i] = uSecs - time[i];

printf( "UserDefinition:: Test value %d\n", i );
	}

	// Free the last argument. When we use XL_ARG_GEN_FREE_LAST_ARG we free the last
	// argument generated. It's kept track of in the generator.
	argGenArg.param = DXE_ARG_GEN_FREE_LAST_ARG;
	argGenerator( &argGenArg, NULL );


	//for( int i = 0; i < nSamples; i++ )
		//printf( "%d  %d\n", (XLint32)argSize[i], time[i] );

	// Use Least Square Fitting method to calculate an x^2 curve that will be
	// used for future estimations (Ac=y). 3 for the polynomial of size 2 +1 for the constant.
	n = argSize.size();
	A = gsl_matrix_alloc( n, 3 );
	c = gsl_vector_alloc( 3 );
	y = gsl_vector_alloc( n );
	cov = gsl_matrix_alloc( 3, 3 );

	// Build the input data by row.
	for( int i = 0; i < n; i++ )
	{
		double xi = argSize[i];
		gsl_matrix_set( A, i, 0, 1.0 ); gsl_matrix_set( A, i, 1, xi ); gsl_matrix_set( A, i, 2, xi*xi );

		gsl_vector_set( y, i, time[i] );
	}

	// Do the calculation.
	gsl_multifit_linear_workspace* work = gsl_multifit_linear_alloc( n, 3 );
	gsl_multifit_linear( A, y, c, cov, &chiSq, work );
	gsl_multifit_linear_free( work );

	// Here we get the coefficients we are interested in.
	// These coefficients explains the ETOC curve for the operation.
	_etocC0 = gsl_vector_get( c, 0 );
	_etocC1 = gsl_vector_get( c, 1 );
	_etocC2 = gsl_vector_get( c, 2 );

	gsl_matrix_free( A );
	gsl_vector_free( c );
	gsl_vector_free( y );
	gsl_matrix_free( cov );

	printf( "UserDefinition:: Best fit: Y = %g + %g X + %g X^2\n", _etocC0, _etocC1, _etocC2 );

}

XLint64 UserDefinition::getEtoc( XLreal64 argSize ) const
{
	// This represents the curve C0 + C1*x + C2*x^2.
	return static_cast<XLint64>(_etocC0 + _etocC1 * argSize + _etocC2 * argSize * argSize);
}

XLint64 UserDefinition::getEtoc( XLpv* arg ) const
{
	XLpv		argSize;

	_argumentAnalyzer( arg, &argSize );
	return getEtoc( argSize.value.real32 );
}

void UserDefinition::getEtoc( XLpv* arg, XLint32* h, XLint32* m, XLint32* s, XLint32* us ) const
{
	XLpv		argSize;
	XLint64		etoc;


	_argumentAnalyzer( arg, &argSize );
	etoc = getEtoc( argSize.value.real32 );

	// Micro seconds.
	*us = (XLint32)etoc % 1000000L;

	// Seconds.
	etoc /= 1000000;
	*s = (XLint32)etoc % 60;

	// Minutes.
	etoc /= 60;
	*m = (XLint32)etoc % 60;

	// Hours.
	etoc /= 60;
	*h = (XLint32)etoc;
}

XLreal64 UserDefinition::getEtocC0( void ) const
{
	return _etocC0;
}

XLreal64 UserDefinition::getEtocC1( void ) const
{
	return _etocC1;
}

XLreal64 UserDefinition::getEtocC2( void ) const
{
	return _etocC2;
}

void UserDefinition::setEtoc( XLreal64 etocC0, XLreal64 etocC1, XLreal64 etocC2 )
{
	_etocC0 = etocC0;
	_etocC1 = etocC1;
	_etocC2 = etocC2;
}



