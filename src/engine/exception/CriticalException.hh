/*

   CriticalException.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef CRITICALEXCEPTION_HH_
#define CRITICALEXCEPTION_HH_


#include <iostream>
#include <string>
#include <sstream>


class CriticalException
{

public:

        CriticalException( const char* fileName, int line, const std::string& description );

        friend std::ostream&    operator <<( std::ostream& out, const CriticalException& exception );


protected:

        std::stringstream               _message;


};


#endif /*CRITICALEXCEPTION_HH_*/
