/*

   ExistsException.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef EXISTSEXCEPTION_HH_
#define EXISTSEXCEPTION_HH_

#include "SignalException.hh"


class ExistsException : public SignalException
{

public:

        friend std::ostream&    operator <<( std::ostream& out, const ExistsException& exception );

        ExistsException();
        ExistsException( const std::string& description );



};



#endif
