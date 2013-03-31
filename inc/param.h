/**************************************************************************
 * File: 			param.h
 * Description:		header file for parameters operation
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

#ifndef __PARAM_H__
#define __PARAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define	CONFIG_DEFAULT			(0x11)
#define CONFIG_UPDATED			(0x55)

#define	PARAM_RESET				(1)
#define PARAM_UPDATE			(2)

typedef	struct bootParam
{
	uint16		resetAddr;			/* probe application reset vector address */
	uint16		rsvd1;
	uint16		rsvd2;
	uint16		rsvd3;
} BOOTPARAM_T;

typedef	struct sysParam
{
	uint32   	serial;				/* 1032-1033, read only */
	uint16 		config;				/* 1034, read only, indicate if device is configured */
	uint16		modbusAddr;			/* 1035, r/w */
	uint16		modbusDatabits;		/* 1036, r/w */
	uint16		modbusParity;		/* 1037, r/w */
	uint32		modbusBaud;			/* 1038-1039, r/w */
	uint16		reset;				/* 1040, reserved 1 */
} SYSPARAM_T;

extern SYSPARAM_T  	gSysParam;
extern BOOTPARAM_T	gBootParam;

extern void initSysParams(SYSPARAM_T *);
extern void resetSysParams(SYSPARAM_T *);
extern void updateSysParams(SYSPARAM_T *);

extern void initBootParam(BOOTPARAM_T *);
extern void resetBootParam(BOOTPARAM_T *);
extern void updateBootParam(BOOTPARAM_T *);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
