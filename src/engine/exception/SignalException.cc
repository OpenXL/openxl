/*

   SignalException.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "../../include/engine/exception/SignalException.hh"


SignalException::SignalException()
{
        //_name = "(No name)";
        //_message = "DI Unknown Signal Exception:\n   File unknown\n   Line unknown\n";
}

std::ostream& operator <<( std::ostream& out, const SignalException& exception )
{
        return (out << "[" << exception._name << "] " << exception._message << std::endl);
}
