/******************************************************************************
 * File: 			system.h
 * Description:		header file for system level definitions
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Jan 25, 2013
 *****************************************************************************/

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/
#define	MODE_IDLE			0
#define	MODE_SERIAL			1
#define	MODE_PARAM_UPDATE	2
#define	MODE_REBOOT_APP		4
#define	MODE_REBOOT			8

extern uint8  g_sysOperMode;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
