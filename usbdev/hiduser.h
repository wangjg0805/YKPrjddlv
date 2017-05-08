/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    HIDUSER.H
 *      Purpose: HID Custom User Definitions
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2006 Keil Software.
 *---------------------------------------------------------------------------*/

#ifndef __HIDUSER_H__
#define __HIDUSER_H__

#include "type.h"
/* Push Button Definitions */
#define   KEY0  (0x01 << 23)
#define   KEY1  (0x01 << 24)
#define   KEY2  (0x01 << 25)
#define   KEY3  (0x01 << 26)

/* LED Definitions */
#define   LEDMASK  0x0003c000

#define   LED0  (0x01 << 14)
#define   LED1  (0x01 << 15)
#define   LED2  (0x01 << 16)
#define   LED3  (0x01 << 17)

/* HID Number of Reports */
#define HID_REPORT_NUM      1

/* HID Global Variables */
extern BYTE InReport[8];
//extern BYTE OutReport;

extern BYTE HID_Protocol;
extern BYTE HID_IdleTime[HID_REPORT_NUM];

/* HID Requests Callback Functions */
extern BOOL HID_GetReport   (void);
extern BOOL HID_SetReport   (void);
extern BOOL HID_GetIdle     (void);
extern BOOL HID_SetIdle     (void);
extern BOOL HID_GetProtocol (void);
extern BOOL HID_SetProtocol (void);


#endif  /* __HIDUSER_H__ */
