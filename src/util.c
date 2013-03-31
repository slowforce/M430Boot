/**************************************************************************
 * File: 			util.c
 * Description:		system level utility functions
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

static uint8 g_criticalNesting = 0x00;

void EnterCriticalSection(void)
{
    if( g_criticalNesting == 0 )
    {
    	__disable_interrupt();
    }
    g_criticalNesting++;

    return ;
}

void ExitCriticalSection(void)
{
	g_criticalNesting--;
    if( g_criticalNesting == 0 )
    {
    	__enable_interrupt();
    }

    return ;
}

void rebootSys(void)
{
	WDTCTL = 0;
}

#pragma vector=PORT2_VECTOR
__interrupt void port2Isr(void)
{
	return ;
}

#pragma vector=USART1TX_VECTOR
__interrupt void serial1PortTxIsr(void)
{
	return ;
}

#pragma vector=USART1RX_VECTOR
__interrupt void serial1PortRxIsr(void)
{
	return ;
}

#pragma vector=PORT1_VECTOR
__interrupt void port1Isr(void)
{
	return ;
}

#pragma vector=TIMERA1_VECTOR
__interrupt void timerA1Isr(void)
{
	return ;
}

#pragma vector=ADC12_VECTOR
__interrupt void adc12Isr(void)
{
	return ;
}

#pragma vector=WDT_VECTOR
__interrupt void wdtIsr(void)
{
	return ;
}

#pragma vector=COMPARATORA_VECTOR
__interrupt void comparatorAIsr(void)
{
	return ;
}

#pragma vector=TIMERB1_VECTOR
__interrupt void timerB1Isr(void)
{
	return ;
}

#pragma vector=TIMERB0_VECTOR
__interrupt void timerB0Isr(void)
{
	return ;
}

#pragma vector=NMI_VECTOR
__interrupt void nmiIsr(void)
{
	return ;
}


