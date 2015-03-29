/*

   Component.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef COMPONENT_HH_
#define COMPONENT_HH_

class Engine;


class Component
{

public:

	virtual ~Component() {}

	virtual void			init( Engine* engine ) = 0;


};

#endif
