/*

   CriticalException.cc

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#include "../../include/engine/exception/CriticalException.hh"


CriticalException::CriticalException( const char* fileName, int line, const std::string& description )
{
        _message.clear();
        _message << "DI Critical Exception:" << std::endl <<
                                "   File " << fileName << "   Line " << line << std::endl <<
                                "   \"" << description << "\"" << std::endl;
}


std::ostream& operator <<( std::ostream& out, const CriticalException& exception )
{
        return (out << exception._message.str());
}

