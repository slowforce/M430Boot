/**************************************************************************
 * File: 			upgrade.c
 * Description:		erase and update firmware in MSP430 internal FLASH
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
#include "param.h"
#include "protocol.h"
#include "serial.h"
#include "system.h"
#include "upgrade.h"
#include "util.h"

extern void _c_int00(void);

uint32 g_firmwareLength;
uint32 g_firmwareRecvLength;
uint8 *g_firmwareWritePtr;

extern INTRVECT_T 	g_intrVect[];
extern uint8 		g_intrVectIdx;

void upgradeFwEraseSegments(uint32 length)
{
	g_firmwareWritePtr = (uint8 *)FLASH_APP_ADDR;
	flashEraseAppSegments(g_firmwareWritePtr, length);
	flashEnableWrite();

	return ;
}

void upgradeFwWriteSegments(uint8 *buf, uint8 len)
{
	uint8 i;

	flashUnlock();
	flashEnableWrite();

	for (i=0; i<len; i++)
	{
		*g_firmwareWritePtr = buf[i];
		while(flashWriteBusy());
		g_firmwareWritePtr++;
	}

	flashDisableWrite();
	while(flashBusy());
	flashLock();

	return ;
}

void upgradeFwUpdateIntrVect(void)
{
	uint8 i;

	if (g_intrVectIdx > 16)
	{
		g_intrVectIdx = 16;
	}

	/***********************************
	 * 1. find reset application vector
	 * address and save it to info A
	 **********************************/
	for (i=0; i<g_intrVectIdx; i++)
	{
		if (g_intrVect[i].addr == 0xFFFE)
		{
			gBootParam.resetAddr = g_intrVect[i].value;;
			updateBootParam(&gBootParam);
		}
	}

	/*******************************************
	 * 2. erase interrupt vector segment and
	 * write back bootloader reset vector
	 * address to 0xFFFE
	 ******************************************/
	flashEraseIntrVecSegment();
	flashWriteIntrVecAddr((uint8 *)FLASH_INT15_ADDR, (uint16)_c_int00);

	/*******************************************
	 * 3. write application interrupt vector
	 * address into flash
	 ******************************************/
	for (i=0; i<g_intrVectIdx; i++)
	{
		if (g_intrVect[i].addr != 0xFFFE)
		{
			if (g_intrVect[i].value != 0xFFFF)
			{
				flashWriteIntrVecAddr((uint8 *)g_intrVect[i].addr, g_intrVect[i].value);
			}
		}
	}

	return ;
}
