/**************************************************************************************************
  Filename:       OnBoard.h
  Revised:        $Date: 2010/11/01 18:01:12 $
  Revision:       $Revision: 1.1 $

  Description:    Defines stuff for EVALuation boards
                  This file targets the Chipcon CC2430DB/CC2430EB


  Copyright 2006-2007 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

#ifndef ONBOARD_H
#define ONBOARD_H

#include "hal_mcu.h"
#include "hal_sleep.h"
#include "hal_types.h"
#include "osal.h"
/*********************************************************************
 */
// Internal (MCU) RAM addresses
#define MCU_RAM_BEG 0x0100
#define MCU_RAM_END RAMEND
#define MCU_RAM_LEN (MCU_RAM_END - MCU_RAM_BEG + 1)

// Internal (MCU) heap size
#if !defined( INT_HEAP_LEN )
  #define INT_HEAP_LEN  4096  // 4.00K
#endif

// Memory Allocation Heap
#if defined( EXTERNAL_RAM )
  #define MAXMEMHEAP EXT_RAM_LEN   // Typically, 32K
#else
  #define MAXMEMHEAP INT_HEAP_LEN  // Typically, 0.70-1.50K
#endif

// Timer clock and power-saving definitions
#define TIMER_DECR_TIME    1  // 1ms - has to be matched with TC_OCC
#define RETUNE_THRESHOLD   1  // Threshold for power saving algorithm

/* OSAL timer defines */
#define TICK_TIME   1000   /* Timer per tick - in micro-sec */
#define TICK_COUNT  1
#define OSAL_TIMER  HAL_TIMER_3

/* WATCHDOG TIMER DEFINITIONS */
// WDCTL bit definitions

#define WDINT0     0x01  // Interval, bit0
#define WDINT1     0x02  // Interval, bit1
#define WDINT      0x03  // Interval, mask
#define WDMODE     0x04  // Mode: watchdog=0, timer=1
#define WDEN       0x08  // Timer: disabled=0, enabled=1
#define WDCLR0     0x10  // Clear timer, bit0
#define WDCLR1     0x20  // Clear timer, bit1
#define WDCLR2     0x40  // Clear timer, bit2
#define WDCLR3     0x80  // Clear timer, bit3
#define WDCLR      0xF0  // Clear timer, mask

// WD timer intervals
#define WDTISH     0x03  // Short: clk * 64
#define WDTIMD     0x02  // Medium: clk * 512
#define WDTILG     0x01  // Long: clk * 8192
#define WDTIMX     0x00  // Maximum: clk * 32768

// WD clear timer patterns
#define WDCLP1     0xA0  // Clear pattern 1
#define WDCLP2     0x50  // Clear pattern 2


#ifndef _WIN32
extern void _itoa(uint16 num, uint8 *buf, uint8 radix);
#endif

#ifndef RAMEND
#define RAMEND 0x1000
#endif

/* Tx and Rx buffer size defines used by SPIMgr.c */
#define MT_UART_THRESHOLD    5
#define MT_UART_TX_BUFF_MAX  170
#define MT_UART_RX_BUFF_MAX  170
#define MT_UART_IDLE_TIMEOUT 5

#define WatchDogEnable(wdti) \
{ \
  WDCTL = WDCLP1 | WDEN | (wdti & WDINT); \
  WDCTL = WDCLP2 | WDEN | (wdti & WDINT); \
}

/* system restart and boot loader used from MTEL.c */
#define SystemReset()       \
{                           \
  HAL_DISABLE_INTERRUPTS(); \
  WatchDogEnable( WDTISH ); \
  while (1)  asm("NOP");    \
}

#define BootLoader()

/* Reset reason for reset indication */
#define ResetReason() ((SLEEP >> 3) & 0x03)

/* port definition stuff used by MT */
#if defined (ZAPP_P1)
  #define ZAPP_PORT HAL_UART_PORT_0 //SERIAL_PORT1
#elif defined (ZAPP_P2)
  #define ZAPP_PORT HAL_UART_PORT_1 //SERIAL_PORT2
#else
  #undef ZAPP_PORT
#endif
#if defined (ZTOOL_P1)
  #define ZTOOL_PORT HAL_UART_PORT_0 //SERIAL_PORT1
#elif defined (ZTOOL_P2)
  #define ZTOOL_PORT HAL_UART_PORT_1 //SERIAL_PORT2
#else
  #undef ZTOOL_PORT
#endif

/* sleep macros required by OSAL_PwrMgr.c */
#define SLEEP_DEEP                  0             /* value not used */
#define SLEEP_LITE                  0             /* value not used */
#define MIN_SLEEP_TIME              14            /* minimum time to sleep */
#define OSAL_SET_CPU_INTO_SLEEP(m)  halSleep(m)   /* interface to HAL sleep */


typedef struct
{
  osal_event_hdr_t hdr;
  byte             state; // shift
  //byte             keys;  // keys
  uint16             keys;  // keys
} keyChange_t;


/* used by MTEL.c */
  /*
   * Register for all key events
   */
  extern byte RegisterForKeys( byte task_id );

/* Keypad Control Functions */

  /*
   * Send "Key Pressed" message to application
   */
  extern byte OnBoard_SendKeys(  uint16 keys, byte shift);

  /*
   * Read the keyboard on eval board.
   */
extern void OnBoard_KeyCallback ( uint16 keys, uint8 state );

/*
 * Board specific random number generator
 */
extern uint16 Onboard_rand( void );

/*
 * Get elapsed timer clock counts
 *   reset: reset count register if TRUE
 */
extern uint32 TimerElapsed( void );


/*********************************************************************
 */

#endif
