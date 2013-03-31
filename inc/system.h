/**************************************************************************
 * File: 			serial.h
 * Description:		header file for system level definitions
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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define	MODE_IDLE			(0)
#define	MODE_SERIAL			(1)
#define	MODE_PARAM_UPDATE	(2)
#define	MODE_REBOOT_APP		(4)
#define	MODE_REBOOT			(8)

extern uint8  g_sysOperMode;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
