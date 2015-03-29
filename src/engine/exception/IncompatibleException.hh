/*

   IncompatibleException.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef INCOMPATIBLEEXCEPTION_HH_
#define INCOMPATIBLEEXCEPTION_HH_

#include "SignalException.hh"


class IncompatibleException : public SignalException
{

public:

        friend std::ostream&    operator <<( std::ostream& out, const IncompatibleException& exception );

        IncompatibleException();
        IncompatibleException( const std::string& description );



};



#endif
