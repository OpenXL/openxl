/*

   NoEntryException.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "../../include/engine/exception/NoEntryException.hh"

NoEntryException::NoEntryException()
{
         _name = "No Entry Exception";
}

NoEntryException::NoEntryException( const std::string& description )
{
        _name = "No Entry Exception";
        _message = description;
}


std::ostream& operator <<( std::ostream& out, const NoEntryException& exception )
{
        return (out << "[" << exception._name << "] " << exception._message << std::endl);
}

