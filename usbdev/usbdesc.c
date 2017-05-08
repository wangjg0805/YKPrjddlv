/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    USBDESC.C
 *      Purpose: USB Descriptors
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------*/

#include "type.h"

#include "usb.h"
#include "hid.h"
#include "usbcfg.h"
#include "usbdesc.h"


/* HID Report Descriptor */
const BYTE HID_ReportDescriptor[] = {
  HID_UsagePageVendor(0x00),
  HID_Usage(0x01),
  HID_Collection(HID_Application),
    HID_UsagePage(HID_USAGE_PAGE_BUTTON),
    HID_UsageMin(1),
    HID_UsageMax(3),
    HID_LogicalMin(0),
    HID_LogicalMax(1),
    HID_ReportCount(8),
    HID_ReportSize(8),    //TO PC 8个字节
    HID_Input(HID_Data | HID_Variable | HID_Absolute),
    //HID_ReportCount(1),
    //HID_ReportSize(5),
    //HID_Input(HID_Constant),
    HID_UsagePage(HID_USAGE_PAGE_LED),
    HID_Usage(HID_USAGE_LED_GENERIC_INDICATOR),
    HID_LogicalMin(0),
    HID_LogicalMax(1),
    HID_ReportCount(1),
    HID_ReportSize(8),    //COMMAND 1个字节
    HID_Output(HID_Data | HID_Variable | HID_Absolute),
  HID_EndCollection,
};

const WORD HID_ReportDescSize = sizeof(HID_ReportDescriptor);


/* USB Standard Device Descriptor */
const BYTE USB_DeviceDescriptor[] = {
  USB_DEVICE_DESC_SIZE,              /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0110), /* 1.10 */          /* bcdUSB */
  0x00,                              /* bDeviceClass */
  0x00,                              /* bDeviceSubClass */
  0x00,                              /* bDeviceProtocol */
  USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
  WBVAL(0xC251),                     /* idVendor */
  WBVAL(0x1701),                     /* idProduct */
  WBVAL(0x0100), /* 1.00 */          /* bcdDevice */
  0x04,                              /* iManufacturer */
  0x20,                              /* iProduct */
  0x42,                              /* iSerialNumber */
  0x01                               /* bNumConfigurations */
};

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const BYTE USB_ConfigDescriptor[] = {
/* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,       /* bDescriptorType */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
  WBVAL(                             /* wTotalLength */
    USB_CONFIGUARTION_DESC_SIZE +
    USB_INTERFACE_DESC_SIZE     +
    HID_DESC_SIZE               +
    USB_ENDPOINT_DESC_SIZE
  ),
  0x01,                              /* bNumInterfaces */
  0x01,                              /* bConfigurationValue */
  0x00,                              /* iConfiguration */
  USB_CONFIG_BUS_POWERED /*|*/       /* bmAttributes */
/*USB_CONFIG_REMOTE_WAKEUP*/,
  USB_CONFIG_POWER_MA(100),          /* bMaxPower */
/* Interface 0, Alternate Setting 0, HID Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x00,                              /* bInterfaceNumber */
  0x00,                              /* bAlternateSetting */
  0x01,                              /* bNumEndpoints */
  USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
  HID_SUBCLASS_NONE,                 /* bInterfaceSubClass */
  HID_PROTOCOL_NONE,                 /* bInterfaceProtocol */
  0x5C,                              /* iInterface */
/* HID Class Descriptor */
/* HID_DESC_OFFSET = 0x0012 */
  HID_DESC_SIZE,                     /* bLength */
  HID_HID_DESCRIPTOR_TYPE,           /* bDescriptorType */
  WBVAL(0x0100), /* 1.00 */          /* bcdHID */
  0x00,                              /* bCountryCode */
  0x01,                              /* bNumDescriptors */
  HID_REPORT_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(HID_REPORT_DESC_SIZE),       /* wDescriptorLength */
/* Endpoint, HID Interrupt In */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_IN(1),                /* bEndpointAddress */
  USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
  WBVAL(0x0008),                     /* wMaxPacketSize */
  0x64,          /* 32ms */          /* bInterval */
//optional endpoint
/* Endpoint, Interrupt out */    
//  USB_ENDPOINT_DESC_SIZE,            /* bLength */
//  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
//  USB_ENDPOINT_IN(1),                /* bEndpointAddress */
//  USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
//  WBVAL(0x0004),                     /* wMaxPacketSize */
//  0x20,          /* 32ms */          /* bInterval */

/* Terminator */
  0                                  /* bLength */
};

/* USB String Descriptor (optional) */
const BYTE USB_StringDescriptor[] = {
/* Index 0x00: LANGID Codes */
  0x04,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0409), /* US English */    /* wLANGID */
/* Index 0x04: Manufacturer */
  0x1C,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'S',0,
  'h',0,
  'a',0,
  'n',0,
  'g',0,
  'H',0,
  'a',0,
  'i',0,
  ' ',0,
  'Y',0,
  'O',0,
  'K',0,
  'E',0,
/* Index 0x20: Product */
  0x22,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'Y',0,
  'O',0,
  'K',0,
  'E',0,
  ' ',0,
  ' ',0,
  'P',0,
  '8',0,
  '1',0,
  '1',0,
  ' ',0,
  ' ',0,
  ' ',0,
  'H',0,
  'I',0,
  'D',0,
/* Index 0x42: Serial Number */
  0x1A,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  '0',0,
  '1',0,
  '2',0,
  ' ',0,
  '5',0,
  '5',0,
  '5',0,
  '5',0,
  '8',0,
  '8',0,
  '8',0,
  '8',0,
/* Index 0x5C: Interface 0, Alternate Setting 0 */
  0x08,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'H',0,
  'I',0,
  'D',0,
};
