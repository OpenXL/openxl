/*

   ExistsException.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "../../include/engine/exception/ExistsException.hh"

ExistsException::ExistsException()
{
        _name = "Exists Exception";
}

ExistsException::ExistsException( const std::string& description )
{
        _name = "Exists Exception";
        _message = description;
}


std::ostream& operator <<( std::ostream& out, const ExistsException& exception )
{
        return (out << "[" << exception._name << "] " << exception._message << std::endl);
}
