/*

   IncompatibleException.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "../../include/engine/exception/IncompatibleException.hh"


IncompatibleException::IncompatibleException()
{
        _name = "Incompatible Exception";
}

IncompatibleException::IncompatibleException( const std::string& description )
{
        _name = "Incompatible Exception";
        _message = description;
}


std::ostream& operator <<( std::ostream& out, const IncompatibleException& exception )
{
        return (out << "[" << exception._name << "] " << exception._message << std::endl);
}
