/**************************************************************************
 * File: 			datatype.h
 * Description:		redefine local data type
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

#ifndef __DATATYPE_H__
#define __DATATYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <msp430x14x.h>

#ifndef TRUE
#define TRUE            (1)
#endif

#ifndef FALSE
#define FALSE           (0)
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
