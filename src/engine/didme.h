/*

   didme.h

   Copyright (c) 2015 Daniel Markus
   All rights reserved.

________________________________________________________________________________
*/

#ifndef DIDME_H_
#define DIDME_H_


#ifdef __cplusplus
extern "C" {
#endif


enum DIDMEPerformanceMonitorStandardEnum
{
	DXE_PERFMON_CMD_BEGINNING			= 0,

	DXE_PERFMON_OPEN_CONSOLE_PORT		= 1,
	DXE_PERFMON_CONNECT_DATABASE		= 2,

	DXE_PERFMON_REG_ARG_GENERATOR		= 3,
	DXE_PERFMON_REG_ARG_ANALYSER		= 4,

	DXE_PERFMON_CALC_DEF_ETOCS			= 5,

	DXE_PERFMON_GET_ETOC				= 6,

	DXE_PERFMON_CMD_END					= 7
};

enum DIDMESchedulingStandardEnum
{
	DXE_SCHEDULER_CMD_BEGINNING			= 0,

	DXE_SCHEDULER_PREPARE_OP			= 1,
	DXE_SCHEDULER_ENQUEUE_OP			= 2,
	DXE_SCHEDULER_RECEIVE_OP_RESULTS	= 3,
	DXE_SCHEDULER_WAIT_FOR_GUI			= 4,

	DXE_SCHEDULER_CMD_END				= 5
};



// This may be moved and reconfigured, but for now
enum DIDMEArgumentGeneratorEnum
{
	DXE_ARG_GEN_INIT					= 0,
	DXE_ARG_GEN_GENERATE				= 1,
	DXE_ARG_GEN_FREE_LAST_ARG			= 2
};


#ifdef __cplusplus
}
#endif


#endif
