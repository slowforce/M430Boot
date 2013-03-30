/******************************************************************************
 * File: 			timer.h
 * Description:		header file for timer B functions
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Jan 08, 2013
 *****************************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/
extern bool serialPortTimerInit(uint16);
extern void serialPortTimerEnable(void);
extern void serialPortTimerDisable(void);
extern void disableGeneralTimer(void);
extern __interrupt void serialPortTimerIsr(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
