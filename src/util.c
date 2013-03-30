/******************************************************************************
 * Copyright (C) 2013-2014 Pyxis-lab LLC
 *
 *-----------------------------------------------------------------------------
 * File: 			util.c
 * Description:		implement utility functions
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Jan 25, 2013
 *****************************************************************************/

/* ----------------------- Platform includes --------------------------------*/
#include "datatype.h"

uint8 g_criticalNesting = 0x00;

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


