/******************************************************************************
 * File: 			upgrade.h
 * Description:		header file for firmware upgrading
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Feb 4, 2013
 *****************************************************************************/

#ifndef __UPGRADE_H__
#define __UPGRADE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/
#define	FIRMWARE_LEN_MAX	0xE8E0

extern uint32 g_firmwareLength;
extern uint32 g_firmwareRecvLength;

extern void upgradeFwEraseSegments(uint32);
extern void upgradeFwWriteSegments(uint8 *, uint8);
extern void upgradeFwUpdateIntrVect(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
