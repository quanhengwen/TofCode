/*******************************************************************************
  Filename:     bsp.c
  Revised:        $Date: 18:21 2012年5月7日
  Revision:       $Revision: 1.0 $
  Description:  BSP library

*******************************************************************************/

/*******************************************************************************
* INCLUDES
*/
#include <hal_mcu.h>
#include <hal_flash.h>
#include <hal_sleep.h>
#include <hal_timer.h>
#include <hal_adc.h>
#include <bsp.h>
#include <hal_adc.h>
#include <string.h>
#include <hal_dma.h>
#include "app_card_wireless.h"
#include <timer_event.h>

/*******************************************************************************
* LOCAL VARIABLES
*/
UINT8 s_au8ExAddr[HAL_FLASH_IEEE_SIZE];

static UINT32 s_u32SysTick = 0;
static VOID bsp_SysTick_Update(VOID);
static VOID bsp_timer_Adjust(VOID);

extern LF_CARD_STATE_E cardState;

extern LF_TRANSLATION_INFO_T transInfo;

/*******************************************************************************
* GLOBAL FUNCTIONS
*/

UINT16 BSP_GetRandom(VOID)
{
    return HAL_MCU_Random();
}

/*******************************************************************************
* @fn          BSP_BoardInit
*
* @brief       硬件初始化接口
*
* @param       none
*
*
* @return      none
*/
VOID BSP_BoardInit(VOID)
{
    // Initialise board peripherals
    HAL_MCU_XOSC_Init();

    HalDmaInit();

    HalFlashInit();

    HAL_TIMER2_Start(bsp_SysTick_Update);

    HalFlashRead(HAL_FLASH_IEEE_PAGE, HAL_FLASH_IEEE_OSET,
                 s_au8ExAddr, HAL_FLASH_IEEE_SIZE);

#ifdef OPEN_WTD
    HAL_WATCHDOG_Start(DOGTIMER_INTERVAL_1S);
#endif
}

#ifdef OPEN_WTD
/*******************************************************************************
* @fn          BSP_WATCHDOG_Feed
*
* @brief       喂狗操作
*
* @param       none
*
*
* @return      none
*/
VOID BSP_WATCHDOG_Feed(VOID)
{
    HAL_WATCHDOG_Feed();
}
#endif

/*******************************************************************************
* @fn          BSP_GetExIEEEInfo
*
* @brief       获取IEEE地址参数
*
* @param      output- pu8Buffer 存放IEEE地址参数的buffer
*
*             input- u8Len 存放参数的长度
* @return      none
*/
UINT8 BSP_GetExIEEEInfo(UINT8 *pu8Buffer, UINT8 u8Len)
{
    if (pu8Buffer && u8Len)
    {
        UINT8 x;

        HAL_ENTER_CRITICAL_SECTION(x);
        memcpy(pu8Buffer, s_au8ExAddr, MIN(HAL_FLASH_IEEE_SIZE, u8Len));
        HAL_EXIT_CRITICAL_SECTION(x);
    }

    return MIN(HAL_FLASH_IEEE_SIZE, u8Len);
}

/*******************************************************************************
* @fn          BSP_ADC_GetVdd
*
* @brief       获取当前系统的电量，单位0.05v
*
* @param      none
*
*
* @return      电量值
*/
UINT8 BSP_ADC_GetVdd(VOID)
{
    UINT16 u16Adc = 0;

    u16Adc = HalAdcRead();


#ifdef OPEN_WTD
    BSP_WATCHDOG_Feed();
#endif
    return ((UINT8)(((u16Adc * 10) / 220)));   //delta :0.4V    
}

/*******************************************************************************
* @fn          BSP_SLEEP_Enter
*
* @brief       进入休眠
*
* @param     input - u32Ms 休眠的时间长度
*
*
* @return     none
*/
VOID BSP_SLEEP_Enter(UINT32 u32Ms)
{
    if (u32Ms)
    {
        HAL_SLEEP_Enter(u32Ms);
        bsp_timer_Adjust();
    #ifdef OPEN_WTD
        BSP_WATCHDOG_Feed();
    #endif
    }
}

/*******************************************************************************
* @fn          BSP_GetSysTick
*
* @brief       获取当前系统TICK
*
* @param     none
*
*
* @return     单前系统的Tick数
*/
UINT32 BSP_GetSysTick(VOID)
{
   return s_u32SysTick;
}

/*******************************************************************************
* LOCAL FUNCTIONS
*/

/*******************************************************************************
* @fn          bsp_SysTick_Update
*
* @brief       timer2的回调函数，用于1MS统计一次系统TICK
*
* @param     none
*
*
* @return     none
*/
static VOID bsp_SysTick_Update(VOID)
{
    s_u32SysTick++;
}

/*******************************************************************************
* @fn          bsp_timer_Adjust
*
* @brief       系统TICK校准函数，每次休眠后，都会调用此函数进行TICK校准
*
*
* @param    none
*
*
* @return      none
*/
static VOID bsp_timer_Adjust(VOID)
{
    UINT32 u32AdjustTick;

#ifdef OPEN_WTD
    HAL_WATCHDOG_Feed();
#endif

    u32AdjustTick = HAL_SLEEP_Adjust();

    HAL_CRITICAL_STATEMENT(s_u32SysTick += u32AdjustTick);
}

