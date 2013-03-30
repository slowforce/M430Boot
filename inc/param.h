/******************************************************************************
 * File: 			param.h
 * Description:		header file for system parameter operation
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Jan 15, 2012
 *****************************************************************************/

#ifndef __PARAM_H__
#define __PARAM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/
#define	CONFIG_DEFAULT			0x11
#define 	CONFIG_UPDATED			0x55

#define	PARAM_RESET				1
#define   PARAM_UPDATE				2

typedef	struct bootParam
{
	uint16		resetAddr;		/* probe application reset vector address */
	uint16		rsvd1;
	uint16		rsvd2;
	uint16		rsvd3;
} BOOTPARAM_T;

typedef	struct sysParam
{
	uint32   		serial;					/* 1032-1033, read only */
	uint16 		config;					/* 1036, read only, indicate if device is configured */
	uint16		modbusAddr;			/* 1037, r/w */
	uint16		modbusDatabits;			/* 1038, r/w */
	uint16		modbusParity;			/* 1039, r/w */
	uint32		modbusBaud;			/* 1040-1041, r/w */
	uint16		reset;					/* 1044, reserved 1 */
} SYSPARAM_T;

extern SYSPARAM_T  	gSysParam;
extern BOOTPARAM_T	gBootParam;

extern void initSysParams(SYSPARAM_T *);
extern void resetSysParams(SYSPARAM_T *);
extern void updateParams(SYSPARAM_T *);

extern void initBootParam(BOOTPARAM_T *);
extern void resetBootParam(BOOTPARAM_T *);
extern void updateBootParam(BOOTPARAM_T *);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
