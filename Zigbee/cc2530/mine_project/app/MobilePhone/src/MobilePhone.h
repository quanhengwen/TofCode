/**************************************************************************************************
Filename:       MobilePhone.h
Revised:        $Date: 2011/05/31 23:47:15 $
Revision:       $Revision: 1.2 $

Description:    This file contains the the Mac Sample Application protypes and definitions

**************************************************************************************************/

#ifndef MOBILEPHONE_H
#define MOBILEPHONE_H

/**************************************************************************************************
* INCLUDES
**************************************************************************************************/
#include "hal_types.h"
#include "MobilePhone_cfg.h"
#include "mac_api.h"
/**************************************************************************************************
* CONSTANTS
**************************************************************************************************/

/* Event IDs */
#define MP_FEEDDOG_EVENT           	 0x0001
#define MP_STOP_AUDIO_EVENT	        0x0002
#define MP_PROBENWK_EVENT              	0x0004
//#ifdef PACKAGE_INFORMATION
#define MP_PACKAGE_INFORM_EVENT   0x0008
//#endif
#define MP_DIALUP_RETRY_EVENT         0x0010
#define MP_ACCEPT_RETRY_EVENT         0x0020
#define MP_CLOSE_RETRY_EVENT         0x0040
#define MP_SCAN_AUDIO_EVENT          0x0080


#define MP_RF_OK     0x55
/**************************************************************************************************
* GLOBALS
**************************************************************************************************/
extern uint8 MP_TaskId;
extern uint8 MP_RFState;
/*********************************************************************
* FUNCTIONS
*/

/*
* Task Initialization for the Mac Sample Application
*/
extern void MP_Init( uint8 task_id );

/*
* Task Event Processor for the Mac Sample Application
*/
extern uint16 MP_ProcessEvent( uint8 task_id, uint16 events );

/*
* Handle power saving
*/
extern void MP_PowerMgr (uint8 mode);

extern void MP_ResetFrameblk(void);

extern void MP_ResetAudioStatus(uint16 timeout);

extern uint8  MP_VersionisGet(void);
extern void MP_GetVersion(uint8* p);
/*********************************************************************
*********************************************************************/

#endif /* MOBILEPHONE_H */