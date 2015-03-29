/*

   xl.h

   Copyright (c) 2006 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef XL_H_
#define XL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <XL/xltypes.h>
#include <XL/xldefs.h>


XLstatus XLAPI xlOpen( XLpv* params );
XLstatus XLAPI xlClose();

XLstatus XLAPI xlCreateStd( char* name, XLversion* version, XLstdid* stdid );
XLstatus XLAPI xlGetStd( char* name, XLversion* version, XLstdid* stdid );

XLstatus XLAPI xlDec( XLstdid stdid, XLint32 opid );
XLstatus XLAPI xlDecDom( XLstdid stdid, XLint32 fromOpid, XLint32 toOpid );

XLstatus XLAPI xlCreateLib( char* name, XLversion* version, XLlibid* libid );
XLstatus XLAPI xlRegDef( XLlibid libid, XLstdid stdid, XLint32 opid, char* defName, XLfncptr def );

XLstatus XLAPI xlCtrl( XLstdid stdid, XLint32 opid, XLpv* arg, XLpv* result );

XLstatus XLAPI xlOp( XLstdid stdid, XLint32 opid, XLpv* arg, XLpv* result );


#ifdef __cplusplus
}
#endif


#endif /* XL_H_ */

