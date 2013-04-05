/**************************************************************************
 * File: 			flash.c
 * Description:		MSP430 internal FLASH read/write API
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

#include "datatype.h"
#include "flash.h"
#include "serial.h"
#include "timer.h"
#include "util.h"

extern void _c_int00(void);

/******************************************
 * func: flashReadInfoMem
 * desc: read data from info memory
 * input:
 *   dst - destination address
 *   src - source address
 *   len - data length
 * output:
 * 	 destination address is filled
 * return: none
 *****************************************/
void flashReadInfoMem(uint8 *dst, uint8 *src, uint8 len)
{
	uint8 i;

	for (i=len; i>0; i--)
	{
		*dst++ = *src++;
	}

	return ;
}

/******************************************
 * func: flashWriteInfoMem
 * desc: write data to info memory
 * input:
 *   dst - destination address
 *   src - source address
 *   len - data length
 * output:
 * 	 destination address is filled
 * return:
 * 	 0 - OK
 *****************************************/
int flashWriteInfoMem(uint8 *dst, uint8 *src, uint8 len)
{
	uint8 i;
	uint8 *flashPtr;

	flashEnableErase();
	flashUnlock();

	if (dst < (uint8 *)FLASH_INFO_A)
	{
		flashPtr = (uint8 *)FLASH_INFO_B;
	}
	else
	{
		flashPtr = (uint8 *)FLASH_INFO_A;
	}

	*flashPtr = 0;					// Dummy write to erase Flash segment
	flashEnableWrite();

	for (i=len; i>0; i--)
	{
		*dst++ = *src++;			// Write value to flash
	}

	flashDisableWrite();
	flashLock();

	return 0;
}

/******************************************
 * func: flashReadIntrVecAddr
 * desc: read ISR address
 * input:
 * 	addr - ISR vector address
 * output: none
 * return: ISR address
 *****************************************/
uint16 flashReadIntrVecAddr(uint8 *addr)
{
	uint8  valHigh, valLow;
	uint16 intVectAddr;

	valLow = *addr;
	addr++;
	valHigh = *addr;
	intVectAddr = valHigh<<8 | valLow;

	return intVectAddr;
}

/******************************************
 * func: flashWriteIntrVecAddr
 * desc: write ISR address
 * input:
 * 	addr - ISR vector address
 * 	value - ISR address
 * output: none
 * return: none
 *****************************************/
void flashWriteIntrVecAddr(uint8 *addr, uint16 value)
{
	uint8 *flashPtr;

	_DINT();

	flashUnlock();
	flashEnableWrite();

	flashPtr = (uint8 *)addr;
	*flashPtr = value&0xFF;
	while(flashWriteBusy());
	flashPtr++;
	*flashPtr = value>>8;
	while(flashWriteBusy());

	flashDisableWrite();
	while(flashBusy());
	flashLock();

	_EINT();

	return ;
}

/******************************************
 * func: flashEraseIntrVecSegment
 * desc: earse ISR vectors segment
 * input: none
 * output: none
 * return: none
 *****************************************/
void flashEraseIntrVecSegment(void)
{
	uint8 *flashPtr;

	_DINT();

	flashEnableErase();
	flashUnlock();

	flashPtr = (uint8 *)FLASH_INTVEC_SEGMENT_ADDR;
	*flashPtr = 0;								// Dummy write to erase Flash segment B
	while(flashWriteBusy());

	flashLock();

	_EINT();

	return ;
}

/**********************************************
 * func: flashEraseAppSegments
 * desc: erase application firmware segments
 * input:
 *  addr - application firmware start address
 *  length - application firmware length
 * output: none
 * return: none
 *********************************************/
void flashEraseAppSegments(uint8* addr, uint32 length)
{
	uint16 i;
	uint8 *flashPtr;
	uint16 eraseSegments;

	eraseSegments = length/FLASH_SEGMENT_SIZE;

	_DINT();

	for (i=0; i<eraseSegments+1; i++)
	{
		flashEnableErase();
		flashUnlock();

		flashPtr = addr+i*FLASH_SEGMENT_SIZE;
		*flashPtr = 0;
		while(flashWriteBusy());

		flashLock();
	}

	_EINT();

	return ;
}

/**********************************************
 * func: flashPopulateIntrVecSegment
 * desc: write ISR addresses to ISR vectors
 * input: none
 * output: none
 * return: none
 *********************************************/
void flashPopulateIntrVecSegment(void)
{
	flashWriteIntrVecAddr((uint8 *)FLASH_INT15_ADDR, (uint16)_c_int00);

	flashWriteIntrVecAddr((uint8 *)FLASH_INT00_ADDR, (uint16)0xFFFF);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT01_ADDR, (uint16)port2Isr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT02_ADDR, (uint16)serial1PortTxIsr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT03_ADDR, (uint16)serial1PortRxIsr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT04_ADDR, (uint16)port1Isr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT05_ADDR, (uint16)timerA1Isr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT06_ADDR, (uint16)serialPortTimerIsr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT07_ADDR, (uint16)adc12Isr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT08_ADDR, (uint16)serialPortTxIsr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT09_ADDR, (uint16)serialPortRxIsr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT10_ADDR, (uint16)wdtIsr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT11_ADDR, (uint16)comparatorAIsr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT12_ADDR, (uint16)timerB1Isr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT13_ADDR, (uint16)timerB0Isr);
	flashWriteIntrVecAddr((uint8 *)FLASH_INT14_ADDR, (uint16)nmiIsr);

	return ;
}

