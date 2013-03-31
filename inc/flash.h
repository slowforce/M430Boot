/**************************************************************************
 * File: 			flash.h
 * Description:		header file for flash read/write API
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

#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#define	FLASH_INFO_A				(0x1080)
#define	FLASH_INFO_B				(0x1000)

#define	FLASH_BOOT_ADDR				(0x1100)
#define	FLASH_BOOT_SIZE				(0x2000)

#define	FLASH_APP_ADDR				(0x3200)
#define	FLASH_APP_LEN				(0xCDE0)
#define	FLASH_SEGMENT_SIZE			(0x100)

#define	FLASH_INTVEC_SEGMENT_ADDR	(0xFF00)
#define	FLASH_INTVEC_ADDR			(0xFFE0)

#define	FLASH_INT00_ADDR			(0xFFE0)	/* int00		*/
#define	FLASH_INT01_ADDR			(0xFFE2)	/* PORT2		*/
#define	FLASH_INT02_ADDR			(0xFFE4)	/* USART1TX		*/
#define	FLASH_INT03_ADDR			(0xFFE6)	/* USART1RX		*/
#define	FLASH_INT04_ADDR			(0xFFE8)	/* PORT1		*/
#define	FLASH_INT05_ADDR			(0xFFEA)	/* TIMERA1		*/
#define	FLASH_INT06_ADDR			(0xFFEC)	/* TIMERA0  	*/
#define	FLASH_INT07_ADDR			(0xFFEE)	/* ADC12		*/
#define	FLASH_INT08_ADDR			(0xFFF0)	/* USART0TX    	*/
#define	FLASH_INT09_ADDR			(0xFFF2)	/* USART0RX    	*/
#define	FLASH_INT10_ADDR			(0xFFF4)	/* WDT			*/
#define	FLASH_INT11_ADDR			(0xFFF6)	/* COMPARATORA	*/
#define	FLASH_INT12_ADDR			(0xFFF8)	/* TIMERB1		*/
#define	FLASH_INT13_ADDR			(0xFFFA)	/* TIMERB0		*/
#define	FLASH_INT14_ADDR			(0xFFFC)	/* NMI			*/
#define	FLASH_INT15_ADDR			(0xFFFE)	/* Reset vector */

#define	flashUnlock()				(FCTL3=FWKEY)
#define	flashEnableErase()			(FCTL1=FWKEY+ERASE)
#define	flashEnableWrite()			(FCTL1=FWKEY+WRT)
#define	flashDisableWrite()			(FCTL1=FWKEY)
#define	flashLock()					(FCTL3=FWKEY+LOCK)
#define	flashBusy()					(FCTL3&BUSY)
#define	flashWriteBusy()			(!(FCTL3&WAIT))

extern void flashReadInfoMem(uint8 *, uint8 *, uint8);
extern int flashWriteInfoMem(uint8 *, uint8 *, uint8);
extern uint16 flashReadIntrVecAddr(uint8 *);
extern void flashWriteIntrVecAddr(uint8 *, uint16);
extern void flashEraseAppSegments(uint8*, uint32);
extern void flashEraseIntrVecSegment(void);
extern void flashPopulateIntrVecSegment(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
