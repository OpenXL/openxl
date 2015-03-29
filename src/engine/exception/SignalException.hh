/*

   SignalException.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef SIGNALEXCEPTION_HH_
#define SIGNALEXCEPTION_HH_

#include <iostream>
#include <string>


class SignalException
{

public:

        friend std::ostream&    operator <<( std::ostream& out, const SignalException& exception );


protected:

        SignalException();


        std::string                             _name;
        std::string                             _message;


};



#endif /*SIGNALEXCEPTION_HH_*/
