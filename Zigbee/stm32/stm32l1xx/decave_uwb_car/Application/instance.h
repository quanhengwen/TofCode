// -------------------------------------------------------------------------------------------------------------------
//
//  File: instance.h - DecaWave header for application level instance
//
//  Copyright (c) 2008 - DecaWave Ltd, Dublin, Ireland.
//
//  All rights reserved.
//
//  Author: Billy Verso, October 2008
//
// -------------------------------------------------------------------------------------------------------------------

#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#ifdef __cplusplus
extern "C" {
#endif

//#include "instance_sws.h"
#include "deca_types.h"
#include "deca_device_api.h"
#include "app_protocol.h"


#define NUM_INST            1
#define SPEED_OF_LIGHT      (299702547.0)     // in m/s in air
#define MASK_40BIT			(0x00FFFFFFFFFF)  // MP counter is 40 bits
#define MASK_TXDTS			(0x00FFFFFFFE00)  //The TX timestamp will snap to 8 ns resolution - mask lower 9 bits. 

/******************************************************************************************************************
********************* NOTES on DW (MP) features/options ***********************************************************
*******************************************************************************************************************/
#define DEEP_SLEEP (1)//To enable deep-sleep set this to 1
//DEEP_SLEEP mode can be used, for example, by a Tag instance to put the DW1000 into low-power deep-sleep mode, there are two cases:
// 1. when the Anchor is sending the range report back to the Tag, the Tag will enter sleep after a ranging exchange is finished
// once it receives a report or times out, before the next poll message is sent (before next ranging exchange is started).
// 2. when the Anchor is not sending the report the Tag will go automatically to sleep after the final message is sent

#define DEEP_SLEEP_XTAL_ON (0)
//NOTE: on the EVK1000 the DEEPSLEEP is not actually putting the DW1000 into full DEEPSLEEP mode as XTAL is kept on 


#define ENABLE_AUTO_ACK		(1)		//To enable auto-ack feature set this to 1, frame filtering also needs to be set (to allow ACK frames)
#define ACK_RESPONSE_TIME	(5)     //ACK response time is 5 us (5 symb.), the instance receiving an ACK request will send the ACK after this delay.
#define WAIT_FOR_RESPONSE_DLY	(0) //Tx to Rx delay time, the instance waiting for response will delay turning the receiver on by this amount
// The receiver will be enabled automatically after frame transmission if DWT_RESPONSE_EXPECTED is set in dwt_starttx.
// The instance requesting an ACK, can also program a delay in turning on its receiver, if it knows that the ACK will be sent after particular delay.
// Here it is set to 0 us, which will enable the receiver a.s.a.p so it is ready for the incoming ACK message. 
// The minimum ACK response time about 5us, and the IEEE standard, specifies that the ACK has to be able to be sent within 12 us of reception of an ACK request frame.

#define SNIFF_MODE	(0) //set to 1 to enable sniff mode (low-power listening mode) Sniff (PPM) mode means that the receiver
// pulses though ON and OFF state, in the ON state all the RF blocks are on/powered  and they are off in the OFF state
// The ON time is set to 2 (= 2+1 PACs) and the off time is 255us - MAX because of a bug in MPW3 (the off timer counts in clock cycles 8ns!).

//Note:	Double buffer mode can only be used with auto rx re-enable, auto rx re-enable is on by default in Listener and Anchor instances
#define DOUBLE_RX_BUFFER (0) //To enable double RX buffer set this to 1 - this only works for the Listener instance
//NOTE: this feature is really meant for a RX only instance, as TX will not be possible while double-buffer and auto rx-enable is on.

#define DR_DISCOVERY	(1) //to use discovery ranging mode (tag will blink until it receives ranging request from an anchor)

#define CORRECT_RANGE_BIAS  (1)     // Compensate for small bias due to uneven accumulator growth at close up high power

#define DEEP_SLEEP_AUTOWAKEUP (0) //to test SLEEP mode

#define PUT_TEMP_VOLTAGE_INTO_POLL   (0)     // to insert wakeup sampled TEMP/VOLTAGE into POLL message

#define MAIN_AND_SUB_RANGING         (1)    //will tof to main_station and sub_station

#define BLINK_AGAIN_IN_END           (1)    //every time after the tof or the blink process ,if there are enaught time ,send blink again

#define POLL_HAVE_BLINK_MSG          (0)    //POLL process have blink's next 1s and 5s idle slot massege ,destaddr= 0xffff,


// on last sector tail 6byte
#define DEV_ADDRESS (0x0801FF00)
/******************************************************************************************************************
*******************************************************************************************************************
*******************************************************************************************************************/

#define USING_64BIT_ADDR (0) //when set to 0 - the DecaRanging application will use 16-bit addresses


#define SIG_RX_ACK				5		// Frame Received is an ACK (length 5 bytes)
#define SIG_RX_BLINK			7		// Received ISO EUI 64 blink message
#define SIG_RX_BLINKDW			8		// Received ISO EUI 64 DW blink message
#define SIG_RX_STD_LENGTH_UP	127	    //STD frame length up
#define mytestsize         88
//lengths including the Decaranging Message Function Code byte
#define TAG_POLL_MSG_LEN                   9//;3				// FunctionCode(1), Temp (1), Volt (1), tick(4), seqnum(2)
#define ANCH_RESPONSE_MSG_LEN               4               // FunctionCode(1), RespOption (1), OptionParam(2)
#define TAG_FINAL_MSG_LEN                   16              // FunctionCode(1), Poll_TxTime(5), Resp_RxTime(5), Final_TxTime(5)
#define TOF_REPORT_MSG_LEN                  15//6               // FunctionCode(1), Measured_TOF_Time(5)   tick(4)  polltick(4) anchorRangeWith(1)
#define RANGINGINIT_MSG_LEN					 10//5				// FunctionCode(1), Tag Address (2), Response Time (2)  slot(1) tick(4)

#define STANDARD_FRAME_SIZE         88
#define MAX_MAC_MSG_DATA_LEN                (STANDARD_FRAME_SIZE)//(TAG_FINAL_MSG_LEN) //max message len of the above
#define MAX_CAR_CARD_CNT            20



#if (USING_64BIT_ADDR==1)
#define ADDR_BYTE_SIZE              (2)
#else
#define ADDR_BYTE_SIZE              (2)
#endif
#define ADDR_BYTE_SIZE_SHORT        (2)

#define FRAME_CONTROL_BYTES         2
#define FRAME_SEQ_NUM_BYTES         1
#define FRAME_PANID                 2
#define FRAME_CRC					2
#define FRAME_SOURCE_ADDRESS        (ADDR_BYTE_SIZE)
#define FRAME_DEST_ADDRESS          (ADDR_BYTE_SIZE)
#define FRAME_CTRLP					(FRAME_CONTROL_BYTES + FRAME_SEQ_NUM_BYTES + FRAME_PANID) //5
#define FRAME_CRTL_AND_ADDRESS      (FRAME_DEST_ADDRESS + FRAME_SOURCE_ADDRESS + FRAME_CTRLP) //21 bytes (or 9 for 16-bit addresses)
#define MAX_USER_PAYLOAD_STRING    88//(STANDARD_FRAME_SIZE-FRAME_CRTL_AND_ADDRESS-TAG_FINAL_MSG_LEN-FRAME_CRC) //127 - 21 - 16 - 2 = 88 (or 100 for 16-bit)

#define BLINK_FRAME_CONTROL_BYTES       1
#define BLINK_FRAME_SEQ_NUM_BYTES       1
#define BLINK_FRAME_CRC					2
#define BLINK_FRAME_SOURCE_ADDRESS      8
#define BLINK_FRAME_CTRLP				(BLINK_FRAME_CONTROL_BYTES + BLINK_FRAME_SEQ_NUM_BYTES) //2
#define BLINK_FRAME_CRTL_AND_ADDRESS    (BLINK_FRAME_SOURCE_ADDRESS + BLINK_FRAME_CTRLP) //10 bytes

#define ANCHOR_LIST_SIZE			(4)
#define TAG_LIST_SIZE				(50)
#define TAG_RANGE_SLOT  			(20*CLOCKS_PER_MILLI)
#define TAG_LIST_TIME 				(TAG_RANGE_SLOT*TAG_LIST_SIZE)	

#define FIXED_REPLY_DELAY            5 //ms  //response delay time (Tag or Anchor when sending Final/Response messages respectively)
#define FIXED_LONG_REPLY_DELAY       150
#define FIXED_LONG_BLINK_RESPONSE_DELAY       (5*FIXED_LONG_REPLY_DELAY) //NOTE: this should be a multiple of FIXED_LONG_REPLY_DELAY see DELAY_MULTIPLE below
#define DELAY_MULTIPLE				(FIXED_LONG_BLINK_RESPONSE_DELAY/FIXED_LONG_REPLY_DELAY - 1)

#define MAX_CARD_CYCLE_SEC      (5)      //the max time period; 5s card =5  ;15s card =15
#define TOF_SLOT_LOC_PERIOD      40    // slot分配的最小周期，1s = 40 slot :40
#define EVERY_SLOT_TIME          30  //MS
#define CARD_1S_SEC_TIME         1200
#define CARD_5S_SEC_TIME         6000
#define WAKE_UP_REDAY_TIME       15   //MS, when wakeup card into the event need some time to ready
#define CARD_WAKE_UP_MAX         50
#define LOST_TOF_MAX_NUM          2    //the max consecutive  lost number can tolerate
#define TOF_TO_SUB_MIN_DIST     (0.0)   //when the main station ditance less then this value ,should do tof to sub station
#define ANCHOR_TOF_PRO            1     //the card in tof process
#define ANCHOR_BLINK_PRO          0     //the card in blink process
#define ANCHOR_TOF_CHANNEL        1     //the card in tof process
#define ANCHOR_BLINK_CHANNEL      3     //the card in blink process
#define ALL_TOF_TIME              2     //Do 2 consecutive tof with sub and main station

#define MAX_CAR_CARD_SEND_COUNT      8      // the car send the card's msg ,the max count is 8,more than 8 will be lose

#define SUM_SLOT_COUNT     (MAX_CARD_CYCLE_SEC*TOF_SLOT_LOC_PERIOD)

#define MAX_SET_CARD_CNT    30
#define MAX_ALARM_NUM       50
typedef enum instanceModes{LISTENER,SUB_STA, TAG, ANCHOR, TAG_TDOA, NUM_MODES} INST_MODE;

typedef enum cardtypeModes{L_IDLE=0,CARD_1S=1,CARD_5S=5,OTHER=6} SLOTLIST_MODE;

typedef enum slotstatus{IDLE=0,USED_TOF=1,USED_UART=2,USED_BLINK=3 } SLOT_STATUS;


//Listener = in this mode, the instance only receives frames, does not respond
//Tag = Exchanges DecaRanging messages (Poll-Response-Final) with Anchor and enabling Anchor to calculate the range between the two instances
//Anchor = see above

// This file defines data and functions for access to Parameters in the Device

// -------------------------------------------------------------------------------------------------------------------
// structure to hold device's logging data


typedef struct
{
	uint32 icid;

	dwt_rxdiag_t diag;

#if DECA_LOG_ENABLE==1
    int         accumLogging ;                                // log data to a file, used to indicate that we are currenty logging (file is open)
	FILE        *accumLogFile ;                               // file
#endif

} devicelogdata_t ;

typedef struct
{
    uint8 frameCtrl[2];                         	//  frame control bytes 00-01
    uint8 seqNum;                               	//  cur slot
    uint8 panID[2];                             	//  PAN ID 03-04
    uint8 destAddr[ADDR_BYTE_SIZE];             	//  05-06 or using 64 bit addresses (05-12)
    uint8 sourceAddr[ADDR_BYTE_SIZE];           	//  07-08 or using 64 bit addresses (13-20)
    uint8 messageData[MAX_USER_PAYLOAD_STRING] ;    //  22-124 (application data and any user payload)
    uint8 fcs[2] ;                              	//  125-126  we allow space for the CRC as it is logically part of the message. However ScenSor TX calculates and adds these bytes.
} srd_msg ;

#if 0
//12 octets for Minimum IEEE ID blink
typedef struct
{
    uint8 frameCtrl;                         		//  frame control bytes 00
    uint8 seqNum;                               	//  sequence_number 01
    uint8 tagID[ADDR_BYTE_SIZE];           			//  02-09 64 bit addresses
    uint8 fcs[2] ;                              	//  10-11  we allow space for the CRC as it is logically part of the message. However ScenSor TX calculates and adds these bytes.
} iso_IEEE_EUI64_blink_msg ;

//18 octets for IEEE ID blink with Temp and Vbat values
typedef struct
{
    uint8 frameCtrl;                         		//  frame control bytes 00
    uint8 seqNum;                               	//  sequence_number 01
    uint8 tagID[ADDR_BYTE_SIZE];           			//  02-09 64 bit addresses
	uint8 enchead[2];								//  10-11 2 bytes (encoded header and header extension)
	uint8 messageID;								//  12 message ID (0xD1) - DecaWave message
	uint8 temp;										//  13 temperature value
	uint8 vbat;										//  14 voltage value
	uint8 gpio;										//  15 gpio status
    uint8 fcs[2] ;                              	//  16-17  we allow space for the CRC as it is logically part of the message. However ScenSor TX calculates and adds these bytes.
} iso_IEEE_EUI64_blinkdw_msg ;


typedef struct
{
    uint8 frameCtrl[2];                         	//  frame control bytes 00-01
    uint8 seqNum;                               	//  sequence_number 02
    uint8 fcs[2] ;                              	//  03-04  CRC
} ack_msg ;


typedef struct
{
    uint8 frameCtrl[2];                         	//  frame control bytes 00-01
    uint8 seqNum;                               	//  sequence_number 02
    uint8 panID[2];                             	//  PAN ID 03-04
    uint8 destAddr[ADDR_BYTE_SIZE_SHORT];           //  05-06
    //uint8 destAddr[ADDR_BYTE_SIZE];           	//  07-08 or using 64 bit addresses (07 - 14)
    uint8 sourceAddr[ADDR_BYTE_SIZE];           	//  07-08 or using 64 bit addresses (07 - 14)
    uint8 messageData[MAX_USER_PAYLOAD_STRING] ;    //  15-124 (application data and any user payload)
    uint8 fcs[2] ;                              	//  125-126  we allow space for the CRC as it is logically part of the message. However ScenSor TX calculates and adds these bytes.
} srd_msg_bcast ;
#endif

typedef struct
{
    unsigned char channelNumber ;       // valid range is 1 to 11
    unsigned char preambleCode ;        // 00 = use NS code, 1 to 24 selects code
    unsigned char pulseRepFreq ;        // NOMINAL_4M, NOMINAL_16M, or NOMINAL_64M
    unsigned char dataRate ;            // DATA_RATE_1 (110K), DATA_RATE_2 (850K), DATA_RATE_3 (6M81)
    unsigned char preambleLen ;         // values expected are 64, (128), (256), (512), 1024, (2048), and 4096
    unsigned char pacSize ;
    unsigned char nsSFD ;
} instanceConfig_t ;


#if (DR_DISCOVERY == 0)
typedef struct
{
	uint64 forwardToFRAddress;
    uint64 anchorAddress;
	uint64 *anchorAddressList;
	int anchorListSize ;
	int anchorPollMask ;
	int sendReport ;
} instanceAddressConfig_t ;
#endif

#define RTD_MED_SZ          8      // buffer size for mean of 8

typedef struct
{
	INST_MODE mode;				//instance mode (tag, anchor or listener)
    int testAppState ;
    int nextState ;
    int previousState ;
    int done ;

	dwt_config_t    configData ;
	dwt_txconfig_t  configTX ;
    uint32 rxTimeouts ;
//#if (DEEP_SLEEP == 1)	
	int txmsgcount;
	int	rxmsgcount;
//#endif
    uint8 shouldDoDelayedRx ;
#if (DR_DISCOVERY == 0)
    instanceAddressConfig_t payload ;
#endif

    uint16 seqnum; //add seqnum in poll segment, current handle seqnum
    srd_msg msg ; // simple 802.15.4a frame structure (used for tx message)
//	iso_IEEE_EUI64_blink_msg blinkmsg ; // simple 802.15.4a frame structure (used for tx blink message)
    srd_msg rxmsg ; //holds received frame (after a good RX frame event)
//	ack_msg rxackmsg ; //holds received ACK frame 
//	union {
//	iso_IEEE_EUI64_blink_msg rxblinkmsg;
//	iso_IEEE_EUI64_blinkdw_msg rxblinkmsgdw;
//	}blinku;

	uint16 tagShortAdd ;
	uint16 psduLength ;

	union {

		uint64 txTimeStamp ;		   // last tx timestamp
		uint64 tagPollTxTime ;		   // tag's poll timestamp
	  uint64 anchorRespTxTime ;
	}txu;

	union {

		uint64 rxTimeStamp ;		   // last rx timestamp
		uint64 anchorRespRxTime ;	   // receive time of response
	}rxu;
	uint16 rxLength ;
	uint8 ackreq;

	uint16 txantennaDelay ;

    uint64 tagPollRxTime ;         // time we received the Poll

	uint64 delayedReplyTime;

    uint64 tof ;
	uint64 m_tof ;
    uint16 relpyAddress ;

	double clockOffset ;

    double adist[RTD_MED_SZ] ;
 //   double adist4[4] ;
    double longTermRangeSum ;
    int longTermRangeCount ;
    int tofindex ;
    int tofcount ;
 //   uint8 lastReportSN ;


//    int last_update ;           // detect changes to status report

	devicelogdata_t devicelogdata;

//	uint8    dispClkOffset ;								// Display Clock Offset

    uint8    macdata_msdu[MAX_MAC_MSG_DATA_LEN];        //

    uint8    frame_sn;
	uint16   panid ;
//    double idistmax;
//    double idistmin;

    double idistance ; // instantaneous distance
    int newrange;
	int oldrange;     //if need send the pre diastance ,it =1

	uint32 lastReportTime;

	uint32 tagSleepTime_us; //in milliseconds
	uint32 tagBlinkSleepTime_us;
	uint32 anchReportTimeout_us;
    uint32 endSleepTime_ms ;
	uint32 endDelayedTime_ms ;

    uint8 instToSleep ;
    uint8 anchorListIndex ;
    uint8 sendTOFR2Tag ;	//sends report to TAG else forwards to Forwarding Address
    uint8 tag2rxReport ;    // tag should get ready to rx report after final message is sent

    uint64 rxOnDelay ;
	uint64 fixedReplyDelay ;
	uint64 fixedReportDelay ;		//TX delay when sending the (ToF) report
	double fixedReplyDelay_ms ;

	uint8 newReportSent;
//    uint8 sentSN;
//    uint8 ackdSN;
//    uint8 recvSN;
    uint8 dataxlen ;
    uint8 wait4ack ;
    uint8 ackexpected ;
    uint8 stoptimer;
    uint8 dwevent[2]; //this holds any TX/RX events - at the moment this is an array of 2 but should be changed to a queue
    uint8 dweventCnt;
    uint8 instancetimer_en;
    uint32 instancetimer; //(TagTimeoutTimer) --- this timer is used to timeout Tag when in deep sleep so it can send the next poll message

	uint32 slottimer;
//	uint32 toftickcount;
//	uint32 toftickcount1;
//	uint64 toftickcount_64;
	uint8 deviceissleeping; //this disabled reading/writing to DW1000 while it is in sleep mode 
							//(DW1000 will wake on chip select so need to disable and chip select line activity)

	uint8 rxautoreenable;
	uint16 lp_osc_cal ;
	uint16 blinktime ;
	uint8 eui64[8];
	uint8 tagToRangeWith;	//it is the index of the tagList array which contains the address of the Tag we are raning with
	uint8 tagCnt;
	uint8 TagSlot;
//	uint8 stawakeuptick;
//	uint64 tagList[TAG_LIST_SIZE];
	uint64 get_anchoraddr;
	uint16 shortaddr16;         //own addr
	uint16 rev_shortaddr16;     //rev source addr
//	int count_waitdata;
	int is_newstatus;
	uint8 is_intoflist;
	uint8 cardstatus;
	uint8 cur_slot;           //current slot
	SLOT_STATUS cur_slot_msg;
	uint8 next_idle_1sslot ;    //station use,if used in  USED_TOF ,this equal the next wakeup slot ; if USED_BLINK ,this slot used to allot the rev card
	uint8 next_idle_5sslot ;
	uint8 rev_card_type;         //rev card is 1s or 5s
//	uint8 only_blink_slot;       //is in the only blink slot or not ,1 yes , 0 no
	uint8 slotlist_full;         //all the idle slot have allot to the card ,is full; 1:full    0:not full
	uint8 have_rev_pollOblink;   //this slot have rev poll or blink
	uint16 cur_slot_seq;         //current slot's seqnum
	uint16 up_revrpoll_time;  //from slot start to rev first poll ,used to compensation the sleep time
	uint32 slot_starttick;
	uint8 station_status;         //station's status 
	uint8 shouldrepvbat;          //1:shoud send Battery and version
	uint8 tdoarepvbat;
	uint16 cardbattery;               //card's battery
	uint8 curslot_column;          //the station's current slot's column
	uint8 curslot_row;             //the station's current slot's cow
	uint8 change_devtype;          //change card's devtype or not
	uint8 revpolltype;             //whether the card rev the poll or not
	int8  i8rssi;
	uint8  help_excit_send;    //the alarm msg have been send to  the station,1:yes  ,0: no ; so is_send=0x11, help &0x01  ;excit &0x10
	uint8 new_inblink;
	uint8 car_revcard;
} instance_data_t ;


/*
typedef struct
{
    double idist ;
    double mean4 ; 
    double mean ; 
    int numAveraged ;
	uint32 lastRangeReportTime ;
} currentRangeingInfo_t ;
*/
typedef struct
{
    SLOT_STATUS b1Used;          // 0: idle, 1: used
    uint8 u8DeviceType ;         /// 1:1s card ;   5:5s card   
    uint8 u8cur_slot;              //current slot number
    uint8 dest_addr[ADDR_BYTE_SIZE];            //current slot's tag addr
    uint8 status;                  //card's current tatus
    uint8 u8LostNum;               // if u8LostNum >= 2, release this slot
    uint16 u16SeqNum;              // sequence number
    uint16 sleeptick;              //this card's need sleep time
    uint32 m_distance;             //main sation distance
	uint32 s_distance;             //sub station distance
} slot_msg_t;

typedef struct
{
    SLOTLIST_MODE slotmode;    // 0x00: idle, ;0x01: 1s card ;0x02:5s card ;
    uint8 num_of5s;          //the number of 5s card have used
    slot_msg_t allocslot[MAX_CARD_CYCLE_SEC];  //every row have MAX_CARD_CYCLE_SEC slot 
} alloc_Slot_t;

typedef struct
{
	uint8 u8cardaddr[ADDR_BYTE_SIZE];   //the slot's card addr
	uint16 u16seqnum;                   //current seq
	uint32 m_distance;                  //current main station distance
	uint32 s_distance;                  //current sub station distance
} uart_distmsg_t;

typedef struct
{
	uint8 u8cardaddr[ADDR_BYTE_SIZE];   //the slot's card addr
	uint8 status;                   //card's status
	uint8 devtype;                  //devtype
} Car_cardsmsg_t;

typedef struct
{
	uint8 u8CardCnt;
	uint32 m_distance;             //main sation distance
	uint32 s_distance;             //sub station distance
    Car_cardsmsg_t cardmsg[MAX_CAR_CARD_CNT];
    
} ts_Car_cardlist;

typedef struct
{
	uint8 count;
	uint16 addr[MAX_ALARM_NUM];
}alarm_addrlist_t;

typedef struct
{
	uint16 alarmaddr;
	uint8  alarmstatus;
	uint8  excitid;
}Sub_alarm_msg_t;

typedef struct
{
    uint16 u16CardNum[MAX_SET_CARD_CNT];
    uint8 u8CardCnt;
} tsCardTypeSet;

typedef struct
{
    uint8 channel ;
    uint8 prf ;
    uint8 datarate ;
    uint8 preambleCode ;
    uint8 preambleLength ;
    uint8 pacSize ;
    uint8 nsSFD ;
} chConfig_t ;

static chConfig_t chConfig[9] ={
//mode 1 - S1: 7 off, 6 off, 5 off
	{
		1,              // channel    //tof channel
		DWT_PRF_16M,    // prf
		DWT_BR_110K,    // datarate
		3,             // preambleCode
		DWT_PLEN_128,	// preambleLength
		DWT_PAC32,		// pacSize
		1		// non-standard SFD
	},

	 //mode 2
	{
		2,              // channel  //blink channel
		DWT_PRF_16M,    // prf
		DWT_BR_6M8,    // datarate
		3,             // preambleCode
		DWT_PLEN_128,	// preambleLength
		DWT_PAC8,		// pacSize
		0		// non-standard SFD
	},
	//mode 3
	{
		2,              // channel
		DWT_PRF_64M,    // prf
		DWT_BR_110K,    // datarate
		9,             // preambleCode
		DWT_PLEN_1024,	// preambleLength
		DWT_PAC32,		// pacSize
		1		// non-standard SFD
	},

	//mode 4
	{
		2,              // channel
		DWT_PRF_64M,    // prf
		DWT_BR_6M8,    // datarate
		9,             // preambleCode
		DWT_PLEN_128,	// preambleLength
		DWT_PAC8,		// pacSize
		0		// non-standard SFD
	},

	//mode 5
	{
		5,              // channel
		DWT_PRF_16M,    // prf
		DWT_BR_110K,    // datarate
		3,             // preambleCode
		DWT_PLEN_1024,	// preambleLength
		DWT_PAC32,		// pacSize
		1		// non-standard SFD
	},

	{
		5,              // channel
		DWT_PRF_16M,    // prf
		DWT_BR_110K,    // datarate
		3,             // preambleCode
		DWT_PLEN_64,	// preambleLength
		DWT_PAC32,		// pacSize
		1		// non-standard SFD
	},

	{
		5,              // channel
		DWT_PRF_16M,    // prf
		DWT_BR_6M8,    // datarate
		3,             // preambleCode
		DWT_PLEN_128,	// preambleLength
		DWT_PAC8,		// pacSize
		0		// non-standard SFD
	},

	//mode 7
	{
		5,              // channel
		DWT_PRF_64M,    // prf
		DWT_BR_110K,    // datarate
		9,             // preambleCode
		DWT_PLEN_1024,	// preambleLength
		DWT_PAC32,		// pacSize
		1		// non-standard SFD
	},

	//mode 8
	{
		5,              // channel
		DWT_PRF_64M,    // prf
		DWT_BR_6M8,    // datarate
		9,             // preambleCode
		DWT_PLEN_128,	// preambleLength
		DWT_PAC8,		// pacSize
		0		// non-standard SFD
	}
};
//-------------------------------------------------------------------------------------------------------------
//
//	Functions used in logging/dispalying range and status data
//
//-------------------------------------------------------------------------------------------------------------

// function to calculate and report the Time of Flight to the GUI/display
uint16 instance_get_cardid(void);

void reportTOF(instance_data_t *inst);

// clear the status/ranging data 
void instanceclearcounts(void) ;
uint64* instgettaglist(void);
void instcleartaglist(void);
void instsettagtorangewith(int tagID);

//void instancegetcurrentrangeinginfo(currentRangeingInfo_t *info) ;

// enable reading of the accumulator - used for displaying channel impulse response
void instancesetaccumulatorreadenable(int enable) ;      // enable reading of accumulator every 'period' frames
void instance_set_alarmlist(uint16 addr,uint8 type,uint8 status);
void instance_set_helpstatus(uint16 addr);
uint8 instance_get_retreatstatus(void);


void dec_sleep_wakeup(void);

void instancerxon(int delayed, uint64 delayedReceiveTime);

void clear_inblinkmsg(void);

void init_car_forrssi();

void init_car_forcard();

void txCar_list_wait_send(uint8 count);

//-------------------------------------------------------------------------------------------------------------
//
//	Functions used in driving/controlling the ranging application
//
//-------------------------------------------------------------------------------------------------------------

// opent the SPI Cheetah interface - called from inittestapplication()
int instancespiopen(void) ;  // Open SPI and return handle
// close the SPI Cheetah interface  
void instance_close(void);
// Call init, then call config, then call run. call close when finished
// initialise the instance (application) structures and DW1000 device
int instance_init(void);

uint8 instance_setpower_rssi(uint8 type);

void instance_init_slotlist(void);

void instance_init_cardslot(uint8 cardtype);

void instance_clear_substa(void);

uint8 instance_get_listslotmsg(uint8 cur_slot);

uint16 get_curslot_destaddr(uint8 curslot) ;

uint8 get_curslot_cardmsg(uint16 *cardid); 

uint8 instance_get_distancelist(uwb_tof_distance_ts *distance, uwb_tof_distance_ts *predistance ,uint8 type);

void instance_set_AnchorPanid(uint8 type);

void txhelp_call_send(void);

void tdoa_send(void);
// configure the instance and DW1000 device
void instance_config(instanceConfig_t *config) ;  

void instance_change_channel(uint8 channel);

#if (DR_DISCOVERY == 0)
// configure the payload and MAC address
void instancesetaddresses(instanceAddressConfig_t *plconfig) ;
#endif
// configure the antenna delays
void instancesetantennadelays(double fdelay) ;                      // delay in nanoseconds
// configure whether the Anchor sends the ToF reports to Tag
void instancesetreporting(int anchorSendsTofReports) ; // configure anchor to send TOF reports to Tag

// called (periodically or from and interrupt) to process any outstanding TX/RX events and to drive the ranging application
int instance_run(uint8 type) ;       // returns indication of status report change
// calls the DW1000 interrupt handler
#define instance_process_irq(x) 	dwt_isr()  //call device interrupt handler
// configure TX/RX callback functions that are called from DW1000 ISR
void instance_rxcallback(const dwt_callback_data_t *rxd);
void instance_txcallback(const dwt_callback_data_t *txd);

// sets the Tag sleep delay time (the time Tag "sleeps" between each ranging attempt)
void instancesettagsleepdelay(uint32);
// sets the Tag/Anchor reply delay time (the time Tag/Anchor wait before sending the reply message (Final/Response))
void instancesetreplydelay(double delayms);
void instancesetblinkreplydelay(double delayms); //delay in ms

// set/get the instance roles e.g. Tag/Anchor/Listener
void instancesetrole(int mode) ;                // 
uint8 instancegetrole(void) ;
// get the DW1000 device ID (e.g. 0xDECA0130 for MP)
uint32 instancereaddeviceid(void) ;                                 // Return Device ID reg, enables validation of physical device presence

int instance_getevent(void);

double instance_get_adist(void);

double instance_get_idist(void);

double instance_get_m_idist(void);

double instance_get_ldist(void);

uint16 instance_get_seqnum(void);

void instance_set_seqnum(void);

uint8 instance_get_helpstatus(void);

void instance_reset_helpstatus(void);

uint8 instance_rev_poll(void);

void instance_set_idle(void);

void instance_cardnewslot_init(void);
uint16 instance_get_sleeptick(void);

uint16 instance_get_ifrevsig(void);

uint16 instance_get_uptick(void);

int instancenewrange(void);
int instanceoldrange(void);

int instance_new_Car(void);

void instance_set_car_rev(void);

int instance_get_anchoraddr(void);
	
//void insertbuff(instance_data_t *inst, int start);

int instance_get_lcount(void) ;

int instance_get_rxf(void);

int instance_get_txf(void); //get number of Txed frames
int instance_get_status(void);
void instance_set_status(int eventstatus);
void instance_set_event(int type);
void card_rev_poll_tontinue_count(void);

void instance_set_card_status(uint8 cardstatus);    

uint8 instance_get_card_status(void) ;

void instance_set_slot_starttick(uint32 tick);

uint32 instance_get_slot_starttick(void);

void instance_set_sta_status(uint8 status);

void instance_reset_sta_status(uint8 status);

uint8 instance_get_sta_status(uint8 status);

void instance_set_vbat(uint16 vdd);

void instance_change_devtype(void);

uint8 instance_getchange_devtype(void);

void instance_set_revpolltype(uint8 type);

uint8 instance_get_revpolltype(void);

void instance_set_helpexcit(uint8 type);

int8 instance_get_rssi(void);

uint8 instance_get_inblinkmsg(void);

int8 instance_get_powerlever(void);

void dec_sleep_wakeup(void);

#define DWT_PRF_64M_RFDLY   (515.6f)
#define DWT_PRF_16M_RFDLY   (515.0f)
extern const uint16 rfDelays[2];
#ifdef __cplusplus
}
#endif

#endif





