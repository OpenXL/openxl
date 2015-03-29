/*

   ObjFncPtrTemplate.hh

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef OBJFNCPTRTEMPLATE_HH_
#define OBJFNCPTRTEMPLATE_HH_

#include "FncPtr.hh"


template< class Object >
class ObjFncPtrTemplate : public FncPtr
{
public:

	typedef XLstatus (Object::* Fnc)(XLpv*,XLpv*);

	ObjFncPtrTemplate( Object* obj, Fnc fnc ) : _object(obj), _fnc(fnc) {}
	virtual ~ObjFncPtrTemplate() {}

	virtual FncPtr*		clone( void ) const;

	virtual XLstatus		exec( XLpv* arg, XLpv* result );


private:

	Object*		_object;
	Fnc		_fnc;

};

template< class Object >
FncPtr* ObjFncPtrTemplate<Object>::clone( void ) const
{
	return new ObjFncPtrTemplate<Object>( _object, _fnc );
}

template< class Object >
XLstatus ObjFncPtrTemplate<Object>::exec( XLpv* arg, XLpv* result )
{
	return (_object->*_fnc)( arg, result );
}



#endif
