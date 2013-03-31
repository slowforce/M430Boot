/**************************************************************************
 * File: 			protocol.c
 * Description:		parse communication protocols
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
#include "checksum.h"
#include "flash.h"
#include "param.h"
#include "protocol.h"
#include "serial.h"
#include "system.h"
#include "upgrade.h"
#include "util.h"

uint8 			g_seqNum;
eProtocolErrno 	g_errorNum;
eProtocolState 	g_protoState;
INTRVECT_T 		g_intrVect[16];
uint8 			g_intrVectIdx;

void protoSetDeviceId(uint8 *data, uint8 len)
{
	uint32 serial;

	serial =  ( ((uint32)data[3] << 24) | \
			    ((uint32)data[2] << 16) | \
			    ((uint32)data[1] << 8)  | \
			    ((uint32)data[0] << 0) );

	gSysParam.serial = serial;
	g_sysOperMode |= MODE_PARAM_UPDATE;

	/*******************************
	 * set reply message
	 ******************************/
	data[0] = ACK;

	return ;
}

void protoGetDeviceId(uint8 *data, uint8 len)
{
	uint32 serial = gSysParam.serial;

	data[3] = (serial >> 24) & 0xFF;
	data[2] = (serial >> 16) & 0xFF;
	data[1] = (serial >> 8)  & 0xFF;
	data[0] = (serial >> 0)  & 0xFF;

	return ;
}

void protoSetDeviceAddr(uint8 *data, uint8 len)
{
	if ( (data[0] >= ADDRESS_MIN) && (data[0] <= ADDRESS_MAX) )
	{
		gSysParam.modbusAddr = data[0];
		g_sysOperMode |= MODE_PARAM_UPDATE;
		data[0] = ACK;
	}
	else
	{
		data[0] = NAK;
	}

	return ;
}

void protoGetDeviceAddr(uint8 *data, uint8 len)
{
	data[0] = gSysParam.modbusAddr;

	return ;
}

void protoSetFwInfo(uint8 *data, uint8 len)
{
	uint32 length;

	length  = ( ((uint32)data[5] << 24) | \
		    	((uint32)data[4] << 16) | \
		    	((uint32)data[3] << 8)  | \
		    	((uint32)data[2] << 0) );

	if (length > FIRMWARE_LEN_MAX)
	{
		data[0] = NAK;
	}
	else
	{
		g_firmwareLength = length;
		g_firmwareRecvLength = 0;

		/*********************************
		 * 1. upgrade firmware version
		 ********************************/
		updateSysParams(&gSysParam);

		/*********************************
		 * 2. erase application segments
		 ********************************/
		upgradeFwEraseSegments(length);

		g_protoState = PROTO_STAT_UPGRADE;
		data[0] = ACK;
	}

	return ;
}

void protoSendFwData(uint8 *data, uint8 len)
{
	if (len == 0)
	{
		if (g_firmwareRecvLength != g_firmwareLength)
		{
			data[0] = NAK;
		}
		else
		{
			data[0] = ACK;
		}
		/***************************************
		 * last packet, reset protocol state
		 * to miscellaneous state
		 **************************************/
		g_protoState = PROTO_STAT_MISC;
	}
	else
	{
		upgradeFwWriteSegments(data, len);
		g_firmwareRecvLength += len;
		data[0] = ACK;
	}

	return ;
}

void protoSendVector(uint8 *data, uint8 len)
{
	uint16  addr;
	uint16	value;

	addr = ( ((uint16)data[1] << 8) | ((uint16)data[0]<<0) );
	value = ( ((uint16)data[3] << 8) | ((uint16)data[2]<<0) );

	/****************************************
	 * save interrupt vector address
	 * in ram, and after receiving reboot
	 * command, erase flash and write
	 * interrupt vector address
	 ***************************************/
	if (g_protoState == PROTO_STAT_MISC)
	{
		g_intrVectIdx = 0;
		g_protoState = PROTO_STAT_VECT;
	}

	if ( (addr<FLASH_INT00_ADDR) || (addr>FLASH_INT15_ADDR) )
	{
		data[0] = NAK;
	}
	else
	{
		g_intrVect[g_intrVectIdx].addr = addr;
		g_intrVect[g_intrVectIdx].value = value;
		g_intrVectIdx++;

		data[0] = ACK;
	}

	return ;
}

void protoBoot2Upgrade(uint8 *data, uint8 len)
{
	g_protoState = PROTO_STAT_MISC;
	data[0] = ACK;

	return ;
}

void protoReboot(uint8 *data, uint8 len)
{
	g_protoState = PROTO_STAT_MISC;

	/****************************
	 * if payload data is 1, then
	 * program interrupt vector
	 * segment, otherwise just
	 * reboot
	 ***************************/
	if (data[0] == 1)
	{
		g_sysOperMode |= MODE_REBOOT_APP;
	}
	else
	{
		g_sysOperMode |= MODE_REBOOT;
	}

	data[0] = ACK;

	return ;
}

eProtocolErrno protocolCheckPacket(void)
{
	uint8 *pkt=(uint8 *)g_serialPortBuf;
	eProtocolErrno rc=PROTO_EOK;

	if (g_serialPortBufRecvPos != PDU_SIZE)
	{
		rc = PROTO_ESIZE;
	}
	else if ( (pkt[ADDR_OFF] != ADDR_BROADCAST) && (pkt[ADDR_OFF] != gSysParam.modbusAddr) )
	{
		rc = PROTO_EADDR;
	}
	else if ( (pkt[FUNC_OFF]>FUNC_REBOOT) || (pkt[FUNC_OFF]<FUNC_SETID) )
	{
		rc = PROTO_EFUNC;
	}
	else if (pkt[PLEN_OFF] > MAX_PLD_SIZE)
	{
		rc = PROTO_EPLEN;
	}
	else if (computeChecksum((uint8 *)g_serialPortBuf, g_serialPortBufRecvPos) != 0)
	{
		rc = PROTO_ECSM;
	}

	if (rc != PROTO_EOK)
	{
		g_errorNum = rc;
		return rc;
	}

	/********************************************
	 * check packet sequence only in firmware
	 * upgrade procedure
	 *******************************************/
	if (g_protoState == PROTO_STAT_UPGRADE)
	{
		if (pkt[FUNC_OFF] == FUNC_SENDFWDATA)
		{
			/*******************************
			 * start tracking packet
			 * sequence number for the
			 * first packet
			 ******************************/
			if (g_firmwareRecvLength == 0)
			{
				g_seqNum = pkt[SEQ_OFF];
			}
			else
			{
				if (g_seqNum == 255)
				{
					if (pkt[SEQ_OFF] != 0)
					{
						rc = PROTO_ESEQ;
					}
				}
				else if (g_seqNum+1 != pkt[SEQ_OFF])
				{
					rc = PROTO_ESEQ;
				}
				g_seqNum = pkt[SEQ_OFF];
			}
		}
		else
		{
			g_protoState = PROTO_STAT_MISC;
		}
	}

	g_errorNum = rc;

	return rc;
}

void protocolProcessPacket(void)
{
	uint8 func;
	uint8 *pldBuf;
	uint8 pldLen;

	if (g_errorNum != PROTO_EOK)
	{
		g_serialPortBuf[FUNC_OFF] += 0x80;
		g_serialPortBuf[PLEN_OFF] = 1;
		g_serialPortBuf[PLD_OFF] = g_errorNum;

		return;
	}

	pldBuf = (uint8 *)&(g_serialPortBuf[PLD_OFF]);
	pldLen = g_serialPortBuf[PLEN_OFF];

	func = g_serialPortBuf[FUNC_OFF];
	switch (func)
	{
	case FUNC_SETID:
		protoSetDeviceId(pldBuf, pldLen);
		g_serialPortBuf[PLEN_OFF] = 1;
		break;

	case FUNC_GETID:
		protoGetDeviceId(pldBuf, pldLen);
		g_serialPortBuf[PLEN_OFF] = 4;
		break;

	case FUNC_SETADDR:
		protoSetDeviceAddr(pldBuf, pldLen);
		g_serialPortBuf[PLEN_OFF] = 1;
		break;

	case FUNC_GETADDR:
		protoGetDeviceAddr(pldBuf, pldLen);
		g_serialPortBuf[PLEN_OFF] = 1;
		break;

	case FUNC_BOOT2UPGRADE:	// also implemented in probe application code
		protoBoot2Upgrade(pldBuf, pldLen);
		g_serialPortBuf[PLEN_OFF] = 1;
		break;

	case FUNC_SETFWINFO:
		protoSetFwInfo(pldBuf, pldLen);
		g_serialPortBuf[PLEN_OFF] = 1;
		break;

	case FUNC_SENDFWDATA:
		protoSendFwData(pldBuf, pldLen);
		g_serialPortBuf[PLEN_OFF] = 1;
		break;

	case FUNC_SENDVECT:
		protoSendVector(pldBuf, pldLen);
		g_serialPortBuf[PLEN_OFF] = 1;
		break;

	case FUNC_REBOOT:
		protoReboot(pldBuf, pldLen);
		g_serialPortBuf[PLEN_OFF] = 1;
		break;

	default:
		g_errorNum = PROTO_EPROTO;
		g_serialPortBuf[FUNC_OFF] += 0x80;
		g_serialPortBuf[PLEN_OFF] = 1;
		g_serialPortBuf[PLD_OFF] = g_errorNum;
		break;
	}

	return ;
}

void protocolSendPacket(uint8 *buf, uint8 len)
{
    uint16	checksum;

    if ( (len!=PDU_SIZE) || (g_serialPortRecvState!=STATE_RX_IDLE) )
    {
    	return ;
    }

    ENTER_CRITICAL_SECTION();

	g_serialPortBufSendCnt = len;
	g_serialPortSendPtr = g_serialPortBuf;

	/* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
	checksum = computeChecksum(buf, len-2);
	buf[CSM_OFF] = (uint8)(checksum & 0xFF);
    buf[CSM_OFF+1] = (uint8)(checksum >> 8);

    g_serialPortSendState = STATE_TX_XMIT;
    serialPortEnable(FALSE, TRUE);

    EXIT_CRITICAL_SECTION();

    return ;
}


