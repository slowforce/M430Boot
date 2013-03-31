/**************************************************************************
 * File: 			upgrade.h
 * Description:		header file for firmware upgrading function
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

#ifndef __UPGRADE_H__
#define __UPGRADE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define	FIRMWARE_LEN_MAX	(0xE8E0)

extern uint32 g_firmwareLength;
extern uint32 g_firmwareRecvLength;

extern void upgradeFwEraseSegments(uint32);
extern void upgradeFwWriteSegments(uint8 *, uint8);
extern void upgradeFwUpdateIntrVect(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
