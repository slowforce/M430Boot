/**************************************************************************
 * File: 			timer.c
 * Description:		MPS430 timer A0 driver
 * 					borrow code from FreeModbus (www.freemodbus.org)
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
#include "serial.h"
#include "system.h"
#include "util.h"

/* ----------------------- Defines ------------------------------------------*/
/* Timer ticks are counted in multiples of 50us. Therefore 20000 ticks are
 * one second.
 */
#define TIMER_TICKS          (20000L)

/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- External variables -------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
bool serialPortTimerInit(uint16 timeout50us)
{
    bool 	bInitialized = FALSE;
    uint32  ulReloadValue = (ACLK*(uint32)timeout50us) / TIMER_TICKS;

    if(ulReloadValue <= 1)
    {
        ulReloadValue = 1;
    }
    else
    {
        ulReloadValue -= 1;
    }

    if(ulReloadValue < 0xFFFE)
    {
        /* Timer A clock source is ACLK, Start disabled. */
        TACTL = TASSEL0;
        TACCR0 = (uint16)ulReloadValue;

        /* Enable Timer A capture compare interrupt. */
        TACCTL0 = CCIE;

        bInitialized = TRUE;
    }

    return bInitialized;
}

void serialPortTimerEnable(void)
{
    /**********************************
     *  Reset timer counter and set
     *  compare interrupt.
     *********************************/
    TAR = 0;
    TACCTL0 |= CCIE;
    TACTL |= MC0;

    return ;
}

void serialPortTimerDisable(void)
{
    TACCTL0 &= ~CCIE;
    TACTL &= ~(MC0|MC1);

    return ;
}

#pragma vector=TIMERA0_VECTOR
__interrupt void serialPortTimerIsr(void)
{
    switch (g_serialPortRecvState)
    {
        /* Timer t35 expired. Startup phase is finished. */
    case STATE_RX_INIT:
        serialPortEventPost(EV_READY);
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_RX_RCV:
        serialPortEventPost(EV_FRAME_RECEIVED);
		g_sysOperMode |= MODE_SERIAL;
		_BIC_SR_IRQ(LPM0_bits);
        break;

        /* An error occurred while receiving the frame. */
    case STATE_RX_ERROR:
    default:
        break;
    }

    serialPortTimerDisable();

    g_serialPortRecvState = STATE_RX_IDLE;
    g_serialPortSendState = STATE_TX_IDLE;

    return ;
}

void disableGeneralTimer(void)
{
	TBCTL &= ~TBIE;
}

