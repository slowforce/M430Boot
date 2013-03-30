/*******************************************************
 * File: 			checksum.h
 * Description:		header file for checksum API
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Jan 24, 2013
 *******************************************************/
#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

#ifdef __cplusplus
extern "C" {
#endif

uint16	computeChecksum(uint8 *frame, uint16 len);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
