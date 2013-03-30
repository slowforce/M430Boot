/*******************************************************
 * File: 			datatype.h
 * Description:		redefine data type
 * Author:			Tim (tim.xu@pyxis-lab.com)
 * Date:			Dec 26, 2012
 *******************************************************/
#ifndef __DATATYPE_H__
#define __DATATYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <msp430x14x.h>

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

typedef	unsigned char 	bool;

typedef unsigned char	uint8; 	
typedef unsigned char	uchar;

typedef unsigned int	uint;
typedef unsigned int	uint16;

typedef	long			int32;
typedef unsigned long	uint32; 	

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* __DATATYPE_H__ */ 
