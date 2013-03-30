/******************************************************************************
 * Copyright (C) 2013-2014 Pyxis-lab LLC
 *
 *-----------------------------------------------------------------------------
 * File: 			param.c
 * Description:		system parameter operation
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Jan 15, 2013
 *****************************************************************************/

/* ----------------------- Platform includes --------------------------------*/
#include "datatype.h"
#include "flash.h"
#include "param.h"
#include "serial.h"
#include "timer.h"

SYSPARAM_T  	gSysParam;
BOOTPARAM_T	gBootParam;

void resetSysParams(SYSPARAM_T *sys)
{
	sys->serial = 0x2013;
	sys->modbusAddr = 10;
	sys->modbusDatabits = 8;
	sys->modbusParity = PARITY_EVEN;
	sys->modbusBaud = 9600;
	sys->config = CONFIG_DEFAULT;
	sys->reset = 0;

	return ;
}

void updateSysParams(SYSPARAM_T *sys)
{
	sys->config = CONFIG_UPDATED;
	flashWriteInfoMem((uint8 *)FLASH_INFO_B, (uint8 *)sys, sizeof(SYSPARAM_T));

	return ;
}

void initSysParams(SYSPARAM_T *sys)
{
	/**************************************************
	 * 1. read parameters from INFO_B (0x1000)
	 *************************************************/
	flashReadInfoMem((uint8 *)sys, (uint8 *)FLASH_INFO_B, sizeof(SYSPARAM_T));

	/**************************************************
	 * 2. check if parameters have been updated
	 * or have been set to default value, if not, set
	 * all parameters to default value and write them
	 * back to information memory
	 *************************************************/
	if ( (sys->config != CONFIG_DEFAULT) && (sys->config != CONFIG_UPDATED) )
	{
		resetSysParams(sys);
		flashWriteInfoMem((uint8 *)FLASH_INFO_B, (uint8 *)sys, sizeof(SYSPARAM_T));
	}

	return ;
}


void resetBootParam(BOOTPARAM_T *boot)
{
	boot->resetAddr = 0xFFFF;
	boot->rsvd1 = 0x0001;
	boot->rsvd2 = 0x0002;
	boot->rsvd3 = 0x0003;

	return ;
}

void updateBootParam(BOOTPARAM_T *boot)
{
	flashWriteInfoMem((uint8 *)FLASH_INFO_A, (uint8 *)boot, sizeof(BOOTPARAM_T));

	return ;
}

void initBootParam(BOOTPARAM_T *boot)
{
	flashReadInfoMem((uint8 *)boot, (uint8 *)FLASH_INFO_A, sizeof(BOOTPARAM_T));

	if (gBootParam.rsvd1 != 0x0001)
	{
		resetBootParam();
		updateBootParam();
	}

	return ;
}



