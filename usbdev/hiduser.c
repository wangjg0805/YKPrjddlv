/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    HIDUSER.C
 *      Purpose: HID Custom User Module
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2006 Keil Software.
 *---------------------------------------------------------------------------*/
#include "config.h"

#include "type.h"

#include "usb.h"
#include "hid.h"
#include "usbcfg.h"
#include "usbcore.h"
#include "hiduser.h"

#include "global.h"
/* HID Demo Variables */
BYTE  InReport[8];
// BYTE OutReport;

 BYTE HID_Protocol;
 BYTE HID_IdleTime[HID_REPORT_NUM];

///////////////////////////////////////
//       send data
///////////////////////////////////////
void USB_SendData (void) {
//filled with 
 sprintf(InReport,"%6.1f", ph_data);
/*
 InReport[0] = 0x30;
 InReport[1] = 0x31;
 InReport[2] = 0x32;
 InReport[3] = 0x33;
 InReport[4] = 0x34;
 InReport[5] = 0x35;
 InReport[6] = 0x36;
 InReport[7] = 0x37;

if(FIO2PIN & 0x01)
 InReport[0] += 0x01;
 else
 InReport[0] |= 0x00;*/
}

///////////////////////////////////////
//       receive command
///////////////////////////////////////

 USB_RevCommand(uint32 command)
  {
 
  }
 
/*
 *  Set HID Output Report <- OutReport
 */

//void SetOutReport (void) {

//  IO1SET  = LEDMASK;
//  IO1CLR  = OutReport<<14;	
//}

/*
 *  HID Get Report Request Callback
 *   Called automatically on HID Get Report Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_GetReport (void) {
  uint32 i;
  /* ReportID = SetupPacket.wValue.WB.L; */
  switch (SetupPacket.wValue.WB.H) {
    case HID_REPORT_INPUT:
      USB_SendData();
      //GetInReport();
      for(i=0;i<8;i++)
      EP0Buf[i] = InReport[i];
      //EP0Buf[0] = InReport;
      break;
    case HID_REPORT_OUTPUT:
      return (FALSE);          /* Not Supported */
    case HID_REPORT_FEATURE:
      /* EP0Buf[] = ...; */
      /* break; */
      return (FALSE);          /* Not Supported */
  }
  return (TRUE);
}


/*
 *  HID Set Report Request Callback
 *   Called automatically on HID Set Report Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_SetReport (void) {

  /* ReportID = SetupPacket.wValue.WB.L; */
  switch (SetupPacket.wValue.WB.H) {
    case HID_REPORT_INPUT:
      return (FALSE);          /* Not Supported */
    case HID_REPORT_OUTPUT:
      //
      USB_RevCommand(EP0Buf[0]);
      //OutReport = EP0Buf[0];
      //SetOutReport();
      break;
    case HID_REPORT_FEATURE:
      return (FALSE);          /* Not Supported */
  }
  return (TRUE);
}


/*
 *  HID Get Idle Request Callback
 *   Called automatically on HID Get Idle Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_GetIdle (void) {

  EP0Buf[0] = HID_IdleTime[SetupPacket.wValue.WB.L];
  return (TRUE);
}


/*
 *  HID Set Idle Request Callback
 *   Called automatically on HID Set Idle Request
 *    Parameters:      None (global SetupPacket)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_SetIdle (void) {

  HID_IdleTime[SetupPacket.wValue.WB.L] = SetupPacket.wValue.WB.H;

  /* Idle Handling if needed */
  /* ... */

  return (TRUE);
}


/*
 *  HID Get Protocol Request Callback
 *   Called automatically on HID Get Protocol Request
 *    Parameters:      None (global SetupPacket)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_GetProtocol (void) {

  EP0Buf[0] = HID_Protocol;
  return (TRUE);
}


/*
 *  HID Set Protocol Request Callback
 *   Called automatically on HID Set Protocol Request
 *    Parameters:      None (global SetupPacket)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_SetProtocol (void) {

  HID_Protocol = SetupPacket.wValue.WB.L;

  /* Protocol Handling if needed */
  /* ... */

  return (TRUE);
}
