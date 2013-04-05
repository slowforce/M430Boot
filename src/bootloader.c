/**************************************************************************
 * File: 			bootloader.c
 * Description:		entry point of bootloader module
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
#include "flash.h"
#include "param.h"
#include "protocol.h"
#include "serial.h"
#include "system.h"
#include "timer.h"
#include "upgrade.h"
#include "util.h"

/* ----------------------- Definitions --------------------------------------*/

/* ----------------------- Static variables ---------------------------------*/
uint8  g_sysOperMode=MODE_IDLE;

/* ----------------------- Start implementation -----------------------------*/

void initSerialPort(uint32 baudrate, uint8 databits, uint8 parity)
{
	uint32 t35Cnt;

    /************************************************************
     * The timer reload value for a character is given by:
     *
     * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
     *             = 11 * Ticks_per_1s / Baudrate
     *             = 220000 / Baudrate
     * The reload for t3.5 is 1.5 times this value
     * and similar for t3.5.
     ***********************************************************/
	t35Cnt = (7UL*220000UL) / (2UL*baudrate);

	serialPortInit(baudrate, databits, parity);
	serialPortTimerInit(t35Cnt);
	serialPortStart();

	return ;
}

void sendBootupPacket(void)
{
	g_serialPortRecvState = STATE_RX_IDLE;

	g_serialPortBuf[ADDR_OFF] = gSysParam.address;
	g_serialPortBuf[FUNC_OFF] = FUNC_BOOT2UPGRADE;
	g_serialPortBuf[SEQ_OFF] = 1;
	g_serialPortBuf[PLEN_OFF] = 1;
	g_serialPortBuf[PLD_OFF] = ACK;

	protocolSendPacket((uint8 *)g_serialPortBuf, PDU_SIZE);
}

/******************************************
 * func: main
 * desc: entry point of bootloader
 * input: none
 * output: none
 * return: none
 *****************************************/
void main(void)
{
	uint16 resetVectorAddr;
	uint16 int06VecAddr;
	uint16 int08VecAddr;
	uint16 int09VecAddr;

	WDTCTL = WDTPW + WDTHOLD;

	/***************************************************
	 * disable timerB0 as it is configured in app code
	 * we need to disable it here
	 **************************************************/
	disableGeneralTimer();

	/***************************************************
	 * initialize boot parameters
	 **************************************************/
	initBootParam(&gBootParam);

	/***************************************************
	 * initialize system parameters
	 **************************************************/
	initSysParams(&gSysParam);

	/***************************************************
	 * read Timer A, USART0 Tx/Rx interrupt reset
	 * vector address
	 **************************************************/
	int06VecAddr = flashReadIntrVecAddr((uint8 *)FLASH_INT06_ADDR);
	int08VecAddr = flashReadIntrVecAddr((uint8 *)FLASH_INT08_ADDR);
	int09VecAddr = flashReadIntrVecAddr((uint8 *)FLASH_INT09_ADDR);

	/***************************************************
	 * 1. read resetVector address saved in InfoA
	 * if reset vector address is not 0xFFFF, and
	 * if all interrupt vector address is not
	 * equal to bootloader's interrupt vector address,
	 * jump to probe application. This is a normal
	 * probe application boot case
	 **************************************************/
	resetVectorAddr = gBootParam.resetAddr;
	if ( (resetVectorAddr != 0xFFFF) && \
		 (int06VecAddr != (uint16)serialPortTimerIsr) 	&& \
		 (int08VecAddr != (uint16)serialPortTxIsr) 	&& \
		 (int09VecAddr != (uint16)serialPortRxIsr) )
	{
		((void (*)(void))resetVectorAddr)();
	}

	/***************************************************
	 * 2. otherwise, if reset vector is 0xFFFF and
	 * any of the interrupt vector address is not
	 * equal to bootloader's interrupt vector address,
	 * write bootloader's interrupt vector address
	 * back to FLASH and wait for PC command.
	 * This is a normal firmware upgrading case
	 **************************************************/
	if (resetVectorAddr == 0xFFFF)
	{
		if ( (int06VecAddr != (uint16)serialPortTimerIsr) || \
			 (int08VecAddr != (uint16)serialPortTxIsr) || \
			 (int09VecAddr != (uint16)serialPortRxIsr) )
		{
			flashEraseIntrVecSegment();
			flashPopulateIntrVecSegment();
		}
	}
	/***************************************************
	 * 3. if reset vector is not 0xFFFF and if
	 * any of the interrupt vector address is
	 * equal to bootloader's interrupt vector address,
	 * write bootloader's interrupt vector address
	 * back to FLASH and wait for PC command.
	 * This case happened when firmware upgrading
	 * complete, but write probe application interrupt
	 * vector address to FLASH not complete
	 **************************************************/
	else
	{
		if ( (int06VecAddr == (uint16)serialPortTimerIsr) || \
			 (int08VecAddr == (uint16)serialPortTxIsr) || \
			 (int09VecAddr == (uint16)serialPortRxIsr) )
		{
			flashEraseIntrVecSegment();
			flashPopulateIntrVecSegment();
		}
	}

	/***************************************************
	 * initialize serial port
	 **************************************************/
	initSerialPort(gSysParam.baudrate, (uint8)gSysParam.databits, (uint8)gSysParam.parity);

	/**********************************************
	 * send out bootup packet to acknowledge
	 * firmware upgrade command received in
	 * application code
	 *********************************************/
	sendBootupPacket();

	for (; ;)
	{
		if (g_sysOperMode == MODE_IDLE)
		{
			_BIS_SR(LPM0_bits + GIE);
		}
		else if (g_sysOperMode & MODE_PARAM_UPDATE)
		{
			g_sysOperMode &= ~MODE_PARAM_UPDATE;
			updateSysParams(&gSysParam);
		}
		else if (g_sysOperMode & MODE_REBOOT_APP)
		{
			upgradeFwUpdateIntrVect();
			rebootSys();
		}
		else if (g_sysOperMode & MODE_REBOOT_UPGRADE)
		{
			restoreBootParam(&gBootParam);
			rebootSys();
		}
		else
		{
			serialPortPoll();
		}
	}
}

