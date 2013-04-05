/**************************************************************************
 * File: 			param.c
 * Description:		manage bootloader and system parameters
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
#include "serial.h"
#include "timer.h"

SYSPARAM_T  gSysParam;
BOOTPARAM_T	gBootParam;

/*********************************************
 * func: resetSysParams
 * desc: reset system parameters
 * input:
 * 	sys - system parameter pointer
 * output: parameters are set default value
 * return: none
 ********************************************/
void resetSysParams(SYSPARAM_T *sys)
{
	sys->serial = 0x2013;
	sys->address = 10;
	sys->baudrate = 9600;
	sys->databits = 8;
	sys->parity = PARITY_EVEN;
	sys->config = CONFIG_DEFAULT;
	sys->rsvd = 0;

	return ;
}

/*********************************************
 * func: updateSysParams
 * desc: write parameter to info FLASH
 * input:
 * 	sys - system parameter pointer
 * output: none
 * return: none
 ********************************************/
void updateSysParams(SYSPARAM_T *sys)
{
	sys->config = CONFIG_UPDATED;
	flashWriteInfoMem((uint8 *)FLASH_INFO_B, (uint8 *)sys, sizeof(SYSPARAM_T));

	return ;
}

/*********************************************
 * func: initSysParams
 * desc: read parameters from info FLASH B
 * input:
 * 	sys - parameter pointer
 * output: parameters are initialized with
 * 		   values from info FLASH B
 * return: none
 ********************************************/
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

/*********************************************
 * func: resetBootParam
 * desc: reset boot parameters
 * input:
 * 	boot - boot parameter pointer
 * output: parameters are set default value
 * return: none
 ********************************************/
void resetBootParam(BOOTPARAM_T *boot)
{
	boot->resetAddr = 0xFFFF;
	boot->rsvd1 = 0x2009;
	boot->rsvd2 = 0x0010;
	boot->rsvd3 = 0x0013;

	return ;
}

/*********************************************
 * func: updateBootParam
 * desc: write parameter to info FLASH A
 * input:
 * 	boot - boot parameter pointer
 * output: none
 * return: none
 ********************************************/
void updateBootParam(BOOTPARAM_T *boot)
{
	flashWriteInfoMem((uint8 *)FLASH_INFO_A, (uint8 *)boot, sizeof(BOOTPARAM_T));

	return ;
}

/***********************************************
 * func: initBootParam
 * desc: read boot parameter from info FLASH B
 * input:
 * 	boot - boot parameter pointer
 * output: parameters are initialized with
 * 		   values from info FLASH A
 * return: none
 **********************************************/
void initBootParam(BOOTPARAM_T *boot)
{
	flashReadInfoMem((uint8 *)boot, (uint8 *)FLASH_INFO_A, sizeof(BOOTPARAM_T));

	if ( (gBootParam.rsvd1 != 0x2009) ||
		 (gBootParam.rsvd2 != 0x0010) ||
		 (gBootParam.rsvd3 != 0x0013) )
	{
		resetBootParam(boot);
		updateBootParam(boot);
	}

	return ;
}

/*************************************************
 * func: restoreBootParam
 * desc: check reset address, restore it to
 * 		 0xFFFF and write back to info FLASH A
 * input:
 * 	boot - boot parameter pointer
 * output: boot parameters are restored
 * return: none
 ************************************************/
void restoreBootParam(BOOTPARAM_T *boot)
{
	flashReadInfoMem((uint8 *)&gBootParam, (uint8 *)FLASH_INFO_A, sizeof(BOOTPARAM_T));

	if (gBootParam.resetAddr != 0xFFFF)
	{
		resetBootParam(&gBootParam);
		updateBootParam(&gBootParam);
	}

	return ;
}
