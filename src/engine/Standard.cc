/*

   Standard.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "Standard.hh"



Standard::Standard( XLstdid stdid, const std::string& stdName, const XLversion& stdVersion )
	: _standardId(stdid), _name(stdName), _version(stdVersion)
{
}

Standard::~Standard()
{
}

const std::string& Standard::getName( void ) const
{
	return _name;
}

XLversion Standard::getVersion( void ) const
{
	return _version;
}

