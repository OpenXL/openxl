/*

   NoEntryException.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef NOENTRYEXCEPTION_HH_
#define NOENTRYEXCEPTION_HH_


#include "SignalException.hh"


class NoEntryException : public SignalException
{

public:

        friend std::ostream&    operator <<( std::ostream& out, const NoEntryException& exception );

        NoEntryException();
        NoEntryException( const std::string& description );

};


#endif /*NOENTRYEXCEPTION_HH_*/
