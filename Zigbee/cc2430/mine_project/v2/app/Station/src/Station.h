/**************************************************************************************************
Filename:       CR.h
Revised:        $Date: 2010/11/20 23:45:01 $
Revision:       $Revision: 1.2 $

Description:    This file contains the the Mac EndDevice Application protypes and definitions
**************************************************************************************************/

#ifndef STATION_H
#define STATION_H


/**************************************************************************************************
* INCLUDES
**************************************************************************************************/
#include "hal_types.h"
#include "hal_defs.h"
#include "App_cfg.h"
#include "app_protocol.h"
/**************************************************************************************************
*                                        User's  Defines
**************************************************************************************************/  

#ifdef DEBUG
#define STATION_KEY_INT_ENABLED     FALSE       
#else
#define STATION_KEY_INT_ENABLED     FALSE        
#endif
/*
* FALSE = Key Polling
* TRUE  = Key interrupt
*
* Notes: Key interrupt will not work well with 2430 EB because
*        all the operations using up/down/left/right switch will
*        no longer work. Normally S1 + up/down/left/right is used
*        to invoke the switches but on the 2430 EB board,  the
*        GPIO for S1 is used by the LCD.
*/


#define APS_DST_ENDPOINT      STATION_ENDPOINT //should be the same to the full-stack app setting, now is MINE_APP_ENDPOINT.
#define APS_SRC_ENDPOINT      0x20  //FIXME:Get from sniffer, If not so, the full stack node will reject the pkt.
#define APS_CLUSTER_ID          LOCNODE_CLUSTERID //should be the same to the full-stack app setting, now is MINE_APP_CLUSTERID.
#define APS_PROFILE_ID           STATION_APS_PROFID //should be the same to the full-stack setting.


/**************************************************************************************************
* CONSTANTS
**************************************************************************************************/

/* Event IDs */
//#define CR_KEY_EVENT      0x0001
#define STATION_UART_READ0_EVENT     0x0001
#define STATION_UART_READ1_EVENT     0x0002
#define STATION_UART_WRITE_EVENT   0x0004

#define STATION_STATUS_REPORT_EVENT         0x0010
#define STATION_WDG_RESTART_EVENT            0x0100
#if (defined WATCHDOG) &&(WATCHDOG==TRUE)
#define STATION_FEEDWATCHDOG_EVENT    	0x4000
#endif


/**************************************************************************************************
* GLOBALS
**************************************************************************************************/
extern uint8 Station_TaskId;

/*********************************************************************
* FUNCTIONS
*/

/*
* Task Initialization for the Mac Sample Application
*/
extern void Station_Init( uint8 task_id );

/*
* Task Event Processor for the Mac Sample Application
*/
extern uint16 Station_ProcessEvent( uint8 task_id, uint16 events );

/*
* Handle keys
*/
extern void Station_HandleKeys( uint16 keys, uint8 shift );

/*
* Handle power saving
*/
// extern void CR_PowerMgr (uint8 mode);

/*********************************************************************
*********************************************************************/


#endif /* MACENDDEVICEAPP_H */
