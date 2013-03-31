/**************************************************************************
 * File: 			upgrade.h
 * Description:		header file for utility functions
 * Copyright (C) <2013>  <Tim.Xu> slowforce@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

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
