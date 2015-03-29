/*

   Library.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "Library.hh"


Library::Library( XLlibid libid, const std::string& libName, const XLversion& libVersion )
	: _libraryId(libid), _name(libName), _version(libVersion)
{
}

Library::~Library()
{
}

XLlibid Library::getLibraryId( void ) const
{
	return _libraryId;
}

const std::string& Library::getName( void ) const
{
	return _name;
}
