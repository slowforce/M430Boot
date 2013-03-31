/**************************************************************************
 * File: 			serial.c
 * Description:		MSP430 serial port driver
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
#include "protocol.h"
#include "serial.h"
#include "system.h"
#include "timer.h"
#include "util.h"

volatile uint8 *g_serialPortSendPtr;
volatile uint8  g_serialPortBufRecvPos;
volatile uint8  g_serialPortBufSendCnt;
volatile uint8  g_serialPortBuf[MAX_PDU_SIZE];

volatile eSerialRecvState g_serialPortRecvState;
volatile eSerialSendState g_serialPortSendState;

volatile bool g_serialPortEventReady;
volatile eSerialPortEventType g_serialPortEvent;

/* ----------------------- Start implementation -----------------------------*/
void serialPortPoll(void)
{
	eProtocolErrno rc;
	eSerialPortEventType event;

	if (serialPortEventGet(&event) == TRUE)
	{
		switch (event)
		{
			case EV_READY:
				break;

			case EV_FRAME_RECEIVED:
				rc = protocolCheckPacket();
				if (rc != PROTO_EADDR)
				{
					/*******************************
					 * don't process packet if
					 * it is not for me
					 ******************************/
					serialPortEventPost(EV_EXECUTE);
				}
				break;

			case EV_EXECUTE:
				protocolProcessPacket();
				if ( (g_serialPortBuf[ADDR_OFF] != ADDR_BROADCAST) || (g_serialPortBuf[FUNC_OFF] == FUNC_GETADDR) )
				{
					protocolSendPacket((uint8 *)g_serialPortBuf, g_serialPortBufRecvPos);
				}
				break;

			case EV_FRAME_SENT:
				g_sysOperMode &= ~MODE_SERIAL;
				break;

			default:
				break;
		}
	}

	return ;
}

void serialPortEventPost(eSerialPortEventType event)
{
	g_serialPortEventReady = TRUE;
	g_serialPortEvent = event;

	return ;
}

bool serialPortEventGet(eSerialPortEventType *event)
{
	if (g_serialPortEventReady == TRUE)
	{
		*event = g_serialPortEvent;
		g_serialPortEventReady = FALSE;

		return TRUE;
	}

	return FALSE;
}

bool serialPortInit(uint32 ulBaudRate, uint8 ucDataBits, uint8 eParity)
{
	P1DIR |= BIT1;							  // set port to output mode
	P1OUT &= ~BIT1;							  // set 485 to receive mode

	P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
	if (eParity == PARITY_EVEN)				  // 8-bit, EVEN parity check, reset
	{
		UCTL0 |= (PENA + PEV + CHAR + SWRST);
	}
	else if (eParity == PARITY_ODD)           // 8-bit, ODD parity check, reset
	{
		UCTL0 |= (PENA + CHAR + SWRST);
	}
	else									  // no parity check
	{
		UCTL0 |= (CHAR + SWRST);
	}
	ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD

#if 1	// 9600bps
	UTCTL0 |= SSEL0;                          // BRCLK = ACLK (32,768Hz)
	UBR00 = ACLK/ulBaudRate;                  // BRCLK/baudrate
	UBR10 = 0x00;
	UMCTL0 = 0x4A;                            // Modulation
#else	// 19200bps
    P5SEL |= BIT4 + BIT5;          			//MCLK,SMCLK Êä³ö
    BCSCTL1 = 0x00;
    BCSCTL1 |= XT2OFF;
    BCSCTL1 &= ~RSEL0;
    DCOCTL |= DCO1;
    BCSCTL2 |= SELM0;
    BCSCTL2 &= ~SELS;
    BCSCTL2 &= ~DCOR;
    UTCTL0 = SSEL1;
    UBR00 = 0x05;
    UBR10 = 0x00;
    UMCTL0 = 0x77;
#endif

	UCTL0 &= ~SWRST;                          // Initialize USART state machine
	IFG1 &= ~UTXIFG0;                         // Clear Tx interrupt flag
	IFG1 &= ~URXIFG0;                         // Clear Rx interrupt flag

	return TRUE;
}

void serialPortEnable(bool xRxEnable, bool xTxEnable)
{
    ENTER_CRITICAL_SECTION();

    if( xRxEnable )
    {
        P1OUT &= ~BIT1;		// set to receive mode
        IE1 |= URXIE0;
    }
    else
    {
    	IFG1 &= ~URXIFG0;   // Clear rx interrupt flag
        IE1  &= ~URXIE0;
    }

    if(xTxEnable)
    {
        P1OUT |= BIT1;		// set to transmit mode
    	IE1   |= UTXIE0;
        IFG1  |= UTXIFG0;	// trigger 1st interrupt manually
    }
    else
    {
    	IFG1 &= ~UTXIFG0;   // Clear tx interrupt flag
        IE1  &= ~UTXIE0;
    }

    EXIT_CRITICAL_SECTION();

    return ;
}

void serialPortStart(void)
{
    ENTER_CRITICAL_SECTION();

    /*******************************************************************
     * Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE
     ******************************************************************/
    g_errorNum = PROTO_EOK;
    g_protoState = PROTO_STAT_MISC;
    g_serialPortEventReady = FALSE;
    g_serialPortRecvState = STATE_RX_INIT;
    serialPortEnable(TRUE, FALSE);
    serialPortTimerEnable();

    EXIT_CRITICAL_SECTION();

    return ;
}

void serialPortPutByte(uint8 ucByte)
{
    TXBUF0 = ucByte;

    return ;
}

void serialPortGetByte(uint8 *pucByte)
{
    *pucByte = RXBUF0;

    return ;
}

#pragma vector=USART0RX_VECTOR
__interrupt void serialPortRxIsr(void)
{
    uint8  ucByte;

    if (g_serialPortSendState != STATE_TX_IDLE)
    {
    	return;
    }

    serialPortGetByte((uint8 *)&ucByte);

    switch (g_serialPortRecvState)
    {
        /****************************************************************
         * If we have received a character in the init state we have to
         * wait until the frame is finished.
         ***************************************************************/
    case STATE_RX_INIT:
    	serialPortTimerEnable();
        break;

        /**********************************************************
         * In the error state we wait until all characters in the
         * damaged frame are transmitted.
         *********************************************************/
    case STATE_RX_ERROR:
    	serialPortTimerEnable();
        break;

        /****************************************************************
         * In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RCV
         ***************************************************************/
    case STATE_RX_IDLE:
    	g_serialPortBufRecvPos = 0;
    	g_serialPortBuf[g_serialPortBufRecvPos++] = ucByte;
    	g_serialPortRecvState = STATE_RX_RCV;

        serialPortTimerEnable();
        break;

        /****************************************************************
         * We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         ***************************************************************/
    case STATE_RX_RCV:
        if(g_serialPortBufRecvPos < MAX_PDU_SIZE)
        {
        	g_serialPortBuf[g_serialPortBufRecvPos++] = ucByte;
        }
        else
        {
        	g_serialPortRecvState = STATE_RX_ERROR;
        }

        serialPortTimerEnable();
        break;
    }

    return ;
}

#pragma vector=USART0TX_VECTOR
__interrupt void serialPortTxIsr(void)
{
    if (g_serialPortRecvState != STATE_RX_IDLE)
    {
    	return ;
    }

    switch (g_serialPortSendState)
    {
        /**************************************************************
         * We should not get a transmitter event if the transmitter
         * is in idle state, then disable transmitter and enable
         * receiver
         *************************************************************/
    case STATE_TX_IDLE:
    	serialPortEnable(TRUE, FALSE);
        break;

    case STATE_TX_XMIT:
        /* check if we are finished. */
        if(g_serialPortBufSendCnt != 0)
        {
        	serialPortPutByte(*g_serialPortSendPtr);
        	while((UTCTL0&0x01)==0);
        	g_serialPortSendPtr++;
        	g_serialPortBufSendCnt--;
        }
        else
        {
        	/*********************************************
        	 * wait for the last byte moved out
        	 * from hardware
        	 ********************************************/
        	while((UTCTL0&0x01)==0);

        	/*********************************************
        	 * post frame sent event
        	 ********************************************/
        	serialPortEventPost(EV_FRAME_SENT);

        	/**********************************************************
             * disable transmitter. This prevents another transmit
             * buffer empty interrupt.
             *********************************************************/
            serialPortEnable(TRUE, FALSE);
            g_serialPortSendState = STATE_TX_IDLE;

        }
        break;
    }

    return ;
}

