/*

   OpMap.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef OPMAP_HH_
#define OPMAP_HH_

#include <XL/xltypes.h>
#include <ext/hash_map>
#include "Operation.hh"


// This is a trick to get the hash_map running.
// Have a look at http://gcc.gnu.org/onlinedocs/libstdc++/faq/
#ifdef __GNUC__
	namespace Sgi = ::__gnu_cxx;	// GCC 3.1 and later
#else
	namespace Sgi = std;
#endif



// Maybe later to be defined as a class.
typedef Sgi::hash_map<XLint32, Operation*>	OpMap;

// Right now this is a hash_map, but we may write our own in the future to speed the calls up.
// We must have some kind of hashing here since the user has the possibility of using any
// 32-bit integer. Could probably be optimized with a smart routine that allocate a chunk of
// memory and provides an offset to the first operation. This of course demands that the
// operation domains are close to each other since the memory in between will be unused.


#endif


/*
class OpMap
{

public:

	typedef Sgi::hash_map<XLint32, Operation*>	OpMap;


	OpMap();
	virtual ~OpMap();

};

#endif
*/
