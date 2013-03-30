/*******************************************************
 * File: 			serial.h
 * Description:		header file for serial driver
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Jan 24, 2013
 *******************************************************/
#ifndef __SERIAL_H__
#define __SERIAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define	PARITY_NONE		(0)
#define	PARITY_ODD		(1)
#define	PARITY_EVEN		(2)

#define	MAX_PDU_SIZE	(32)

typedef enum
{
    EV_READY=0,                 /*!< Startup finished. */
    EV_FRAME_RECEIVED,          /*!< Frame received. */
    EV_EXECUTE,                 /*!< Execute function. */
    EV_FRAME_SENT               /*!< Frame sent. */
} eSerialPortEventType;

typedef enum
{
    STATE_RX_INIT=0,            /*!< Receiver is in initial state. */
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is being received. */
    STATE_RX_ERROR              /*!< If the frame is invalid. */
} eSerialRecvState;

typedef enum
{
    STATE_TX_IDLE=0,            /*!< Transmitter is in idle state. */
    STATE_TX_XMIT               /*!< Transmitter is in transfer state. */
} eSerialSendState;

extern volatile uint8 *g_serialPortSendPtr;
extern volatile uint8 g_serialPortBufRecvPos;
extern volatile uint8 g_serialPortBufSendCnt;
extern volatile uint8 g_serialPortBuf[];

extern volatile eSerialRecvState g_serialPortRecvState;
extern volatile eSerialSendState g_serialPortSendState;

extern volatile bool g_serialPortEventReady;
extern volatile eSerialPortEventType g_serialPortEvent;

extern void serialPortEnable(bool, bool);
extern bool serialPortInit(uint32, uint8, uint8);
extern void serialPortPutByte(uint8);
extern void serialPortEventPost(eSerialPortEventType);
extern bool serialPortEventGet(eSerialPortEventType *);
extern void serialPortPoll(void);
extern void serialPortStart(void);

extern __interrupt void serialPortTxIsr(void);
extern __interrupt void serialPortRxIsr(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
