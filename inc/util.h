/******************************************************************************
 * File: 			util.h
 * Description:		header file for utility functions
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Dec 26, 2012
 *****************************************************************************/

#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/
#define ENTER_CRITICAL_SECTION()   EnterCriticalSection()
#define EXIT_CRITICAL_SECTION()    ExitCriticalSection()

#define	MCLK			(800000UL)
#define SMCLK        	(800000UL)
#define ACLK            (32768UL)

extern __interrupt void port2Isr(void);
extern __interrupt void serial1PortTxIsr(void);
extern __interrupt void serial1PortRxIsr(void);
extern __interrupt void port1Isr(void);
extern __interrupt void timerA1Isr(void);
extern __interrupt void adc12Isr(void);
extern __interrupt void wdtIsr(void);
extern __interrupt void comparatorAIsr(void);
extern __interrupt void timerB1Isr(void);
extern __interrupt void timerB0Isr(void);
extern __interrupt void nmiIsr(void);

extern void	EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern void rebootSys(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
