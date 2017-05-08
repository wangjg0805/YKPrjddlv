/*
**************************************************************************************************************
*                                                 NXP USB Host Stack
*
*                                     (c) Copyright 2008, NXP SemiConductors
*                                     (c) Copyright 2008, OnChip  Technologies LLC
*                                                 All Rights Reserved
*
*                                                  www.nxp.com
*                                               www.onchiptech.com
*
* File           : usbhost_uart.h
* Programmer(s)  : Prasad.K.R.S.V
* Version        :
*
**************************************************************************************************************
*/

#ifndef _LPC2478_UART_H
#define _LPC2489_UART_H

/*
**************************************************************************************************************
*                                           INCLUDE HEADER FILES
**************************************************************************************************************
*/

#include <stdarg.h>
#include <stdio.h>
#include "LPC2400.h"
#include "config.h"

#define  DATA_TYPE_NULL         0
#define  DATA_TYPE_RAW          1
#define  DATA_TYPE_KG           2
#define  DATA_TYPE_LB           3
#define  DATA_TYPE_OZ           4
#define  DATA_TYPE_PCS          5
#define  DATA_TYPE_OTHER        6
/*
**************************************************************************************************************
*                                             FUNCTION PROTOTYPES
**************************************************************************************************************
*/

void  UART_Init      (INT32U   baudrate);
void  UART_SendData  (void);
void  UART_PrintChar (uint8   ch);
void  UART_PrintStr  (uint8  *str);
void  UART_CommStr   (uint8  *str,uint8 length);
#endif
