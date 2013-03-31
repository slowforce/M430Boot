/**************************************************************************
 * File: 			protocol.h
 * Description:		header file for communication protocol
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

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define	ACK					(0)
#define	NAK					(1)

#define	ADDR_BROADCAST		(0)		/*! Broadcast address */
#define ADDRESS_MIN      	(1)   	/*! Smallest possible slave address. */
#define ADDRESS_MAX      	(247) 	/*! Biggest possible slave address. */

#define	ADDR_OFF			(0)
#define	FUNC_OFF			(1)
#define	SEQ_OFF				(2)
#define	PLEN_OFF			(3)
#define	PLD_OFF				(4)
#define	CSM_OFF				(14)
#define	PDU_SIZE			(16)
#define	MAX_PLD_SIZE		(10)

#define	FUNC_SETID			(0x64)
#define	FUNC_GETID			(0x65)
#define	FUNC_SETADDR		(0x66)
#define	FUNC_GETADDR		(0x67)
#define	FUNC_SETTIME		(0x68)
#define	FUNC_GETTIME		(0x69)
#define	FUNC_GETFWINFO		(0x6A)

#define	FUNC_BOOT2UPGRADE	(0x70)
#define	FUNC_SETFWINFO		(0x71)
#define	FUNC_SENDFWDATA		(0x72)
#define	FUNC_SENDVECT		(0x73)
#define	FUNC_REBOOT			(0x74)

#define	PLEN_SETID			(4)
#define	PLEN_GETID			(0)
#define	PLEN_SETADDR		(1)
#define	PLEN_GETADDR		(0)
#define	PLEN_SETTIME		(4)
#define	PLEN_GETTIME		(0)
#define	PLEN_BOOT2UPGRADE	(0)
#define	PLEN_SENDFWINFO		(6)
#define PLEN_REBOOT			(0)

typedef	enum
{
	PROTO_STAT_MISC=0,		/* miscellaneous command without state */
	PROTO_STAT_UPGRADE,		/* firmware upgrade write firmware */
	PROTO_STAT_VECT			/* firmware upgrade write vectors */
}eProtocolState;

typedef enum
{
	PROTO_EOK=0,
	PROTO_ESIZE,		/* packet size error */
	PROTO_EADDR,		/* unmatched modbus address */
	PROTO_EFUNC,		/* unsupported command */
	PROTO_ESEQ,			/* sequence number error */
	PROTO_EPLEN,		/* payload length error */
	PROTO_ECSM,			/* checksum error */
	PROTO_EPROTO		/* unsupported protocol */
}eProtocolErrno;

typedef	struct intrVect
{
	uint16 addr;
	uint16 value;
} INTRVECT_T;

extern uint8 g_seqNum;
extern eProtocolErrno g_errorNum;
extern eProtocolState g_protoState;

extern eProtocolErrno protocolCheckPacket(void);
extern void protocolProcessPacket(void);
extern void protocolSendPacket(uint8 *, uint8);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
