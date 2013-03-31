/**************************************************************************
 * File: 			timer.h
 * Description:		header file for timer A0 functions
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

#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

extern bool serialPortTimerInit(uint16);
extern void serialPortTimerEnable(void);
extern void serialPortTimerDisable(void);
extern void disableGeneralTimer(void);
extern __interrupt void serialPortTimerIsr(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
