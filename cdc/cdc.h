/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/*
$Id: cdc.h,v 1.1.2.1 2006/12/05 08:33:25 danielru Exp $
*/

#ifndef _CDC_DRIVER_H
#define _CDC_DRIVER_H

//------------------------------------------------------------------------------
//      Definitions
//------------------------------------------------------------------------------
//! \brief  Communication device class specification version 1.10
#define CDC1_10                                 0x0110

//! \brief  Interface class codes
#define CDC_INTERFACE_COMMUNICATION             0x02
#define CDC_INTERFACE_DATA                      0x0A

//! \brief  Communication interface class subclass codes
//! \see    usbcdc11.pdf - Section 4.3 - Table 16
#define CDC_DIRECT_LINE_CONTROL_MODEL           0x01
#define CDC_ABSTRACT_CONTROL_MODEL              0x02
#define CDC_TELEPHONE_CONTROL_MODEL             0x03
#define CDC_MULTI_CHANNEL_CONTROL_MODEL         0x04
#define CDC_CAPI_CONTROL_MODEL                  0x05
#define CDC_ETHERNET_NETWORKING_CONTROL_MODEL   0x06
#define CDC_ATM_NETWORKING_CONTROL_MODEL        0x07

//! \brief  Communication interface class control protocol codes
//! \see    usbcdc11.pdf - Section 4.4 - Table 17
#define CDC_PROTOCOL_COMMON_AT_COMMANDS         0x01

//! \brief  Data interface class protocol codes
//! \see    usbcdc11.pdf - Section 4.7 - Table 19
#define CDC_PROTOCOL_ISDN_BRI                   0x30
#define CDC_PROTOCOL_HDLC                       0x31
#define CDC_PROTOCOL_TRANSPARENT                0x32
#define CDC_PROTOCOL_Q921_MANAGEMENT            0x50
#define CDC_PROTOCOL_Q921_DATA_LINK             0x51
#define CDC_PROTOCOL_Q921_MULTIPLEXOR           0x52
#define CDC_PROTOCOL_V42                        0x90
#define CDC_PROTOCOL_EURO_ISDN                  0x91
#define CDC_PROTOCOL_V24_RATE_ADAPTATION        0x92
#define CDC_PROTOCOL_CAPI                       0x93
#define CDC_PROTOCOL_HOST_BASED_DRIVER          0xFD
#define CDC_PROTOCOL_DESCRIBED_IN_PUFD          0xFE

//! \brief  CDC class-specific request codes
//!
//!         Values of the bRequest field for the various class-specific requests
//!         defined in the CDC specification.
//! \see    usbcdc11.pdf - Section 6.2 - Table 45
#define CDC_SEND_ENCAPSULATED_COMMAND       0x00
#define CDC_GET_ENCAPSULATED_COMMAND        0x01
#define CDC_SET_COMM_FEATURE                0x02
#define CDC_GET_COMM_FEATURE                0x03
#define CDC_CLEAR_COMM_FEATURE              0x04
#define CDC_SET_AUX_LINE_STATE              0x10
#define CDC_SET_HOOK_STATE                  0x11
#define CDC_PULSE_SETUP                     0x12
#define CDC_SEND_PULSE                      0x13
#define CDC_SET_PULSE_TIME                  0x14
#define CDC_RING_AUX_JACK                   0x15
#define CDC_SET_LINE_CODING                 0x20
#define CDC_GET_LINE_CODING                 0x21
#define CDC_SET_CONTROL_LINE_STATE          0x22
#define CDC_SEND_BREAK                      0x23
#define CDC_SET_RINGER_PARMS                0x30
#define CDC_GET_RINGER_PARMS                0x31
#define CDC_SET_OPERATION_PARMS             0x32
#define CDC_GET_OPERATION_PARMS             0x33
#define CDC_SET_LINE_PARMS                  0x34
#define CDC_GET_LINE_PARMS                  0x35
#define CDC_DIAL_DIGITS                     0x36
#define CDC_SET_UNIT_PARAMETER              0x37
#define CDC_GET_UNIT_PARAMETER              0x38
#define CDC_CLEAR_UNIT_PARAMETER            0x39
#define CDC_GET_PROFILE                     0x3A
#define CDC_SET_ETHERNET_MULTICAST_FILTERS  0x40
#define CDC_SET_ETHERNET_PMP_FILTER         0x41
#define CDC_GET_ETHERNET_PMP_FILTER         0x42
#define CDC_SET_ETHERNET_PACKET_FILTER      0x43
#define CDC_GET_ETHERNET_STATISTIC          0x44
#define CDC_SET_ATM_DATA_FORMAT             0x50
#define CDC_GET_ATM_DEVICE_STATISTICS       0x51
#define CDC_SET_ATM_DEFAULT_VC              0x52
#define CDC_GET_ATM_VC_STATISTICS           0x53

//! \brief  Type values for the bDescriptorType field of functional descriptors
//! \see    usbcdc11.pdf - Section 5.2.3 - Table 24
#define CDC_CS_INTERFACE                    0x24
#define CDC_CS_ENDPOINT                     0x25

//! \brief  Type values for the bDescriptorSubtype field of functional
//!         descriptors
//! \see    usbcdc11.pdf - Section 5.2.3 - Table 25
#define CDC_HEADER                          0x00
#define CDC_CALL_MANAGEMENT                 0x01
#define CDC_ABSTRACT_CONTROL_MANAGEMENT     0x02
#define CDC_DIRECT_LINE_MANAGEMENT          0x03
#define CDC_TELEPHONE_RINGER                0x04
#define CDC_REPORTING_CAPABILITIES          0x05
#define CDC_UNION                           0x06
#define CDC_COUNTRY_SELECTION               0x07
#define CDC_TELEPHONE_OPERATIONAL_MODES     0x08
#define CDC_USB_TERMINAL                    0x09
#define CDC_NETWORK_CHANNEL                 0x0A
#define CDC_PROTOCOL_UNIT                   0x0B
#define CDC_EXTENSION_UNIT                  0x0C
#define CDC_MULTI_CHANNEL_MANAGEMENT        0x0D
#define CDC_CAPI_CONTROL_MANAGEMENT         0x0E
#define CDC_ETHERNET_NETWORKING             0x0F
#define CDC_ATM_NETWORKING                  0x10

//! \brief  Control signal bitmap values for the SetControlLineState request
//! \see    usbcdc11.pdf - Section 6.2.14 - Table 51
#define CDC_DTE_PRESENT                     (1 << 0)
#define CDC_ACTIVATE_CARRIER                (1 << 1)

//! \brief  Serial state notification bitmap values.
//! \see    usbcdc11.pdf - Section 6.3.5 - Table 69
#define CDC_SERIAL_STATE_OVERRUN            (1 << 6)
#define CDC_SERIAL_STATE_PARITY             (1 << 5)
#define CDC_SERIAL_STATE_FRAMING            (1 << 4)
#define CDC_SERIAL_STATE_RING               (1 << 3)
#define CDC_SERIAL_STATE_BREAK              (1 << 2)
#define CDC_SERIAL_STATE_TX_CARRIER         (1 << 1)
#define CDC_SERIAL_STATE_RX_CARRIER         (1 << 0)

//! \brief  Notification requests
//! \see    usbcdc11.pdf - Section 6.3 - Table 68
#define CDC_NOTIFICATION_NETWORK_CONNECTION 0x00
#define CDC_NOTIFICATION_SERIAL_STATE       0x20

//------------------------------------------------------------------------------
//      Structures
//------------------------------------------------------------------------------
//! \brief  Header functional descriptor
//!
//!         This header must precede any list of class-specific descriptors.
//! \see    usbcdc11.pdf - Section 5.2.3.1
#pragma pack(1)
__packed typedef struct {

    //! Size of this descriptor in bytes
    unsigned char bFunctionLength;
    //! CS_INTERFACE descriptor type
    unsigned char bDescriptorType;
    //! Header functional descriptor subtype
    unsigned char bDescriptorSubtype;
    //! USB CDC specification release version
    unsigned short bcdCDC;

} S_cdc_header_descriptor;

//! \brief  Call management functional descriptor
//!
//!         Describes the processing of calls for the communication class
//!         interface.
//! \see    usbcdc11.pdf - Section 5.2.3.2
__packed typedef struct {

    //! Size of this descriptor in bytes
    unsigned char bFunctionLength;
    //! CS_INTERFACE descriptor type
    unsigned char bDescriptorType;
    //! Call management functional descriptor subtype
    unsigned char bDescriptorSubtype;
    //! The capabilities that this configuration supports
    unsigned char bmCapabilities;
    //! Interface number of the data class interface used for call management
    //! (optional)
    unsigned char bDataInterface;

} S_cdc_call_management_descriptor;

//! \brief  Abstract control management functional descriptor
//!
//!         Describes the command supported by the communication interface class
//!         with the Abstract Control Model subclass code.
//! \see    usbcdc11.pdf - Section 5.2.3.3
__packed typedef struct {

    //! Size of this descriptor in bytes
    unsigned char bFunctionLength;
    //! CS_INTERFACE descriptor type
    unsigned char bDescriptorType;
    //! Abstract control management functional descriptor subtype
    unsigned char bDescriptorSubtype;
    //! Capabilities supported by this configuration
    unsigned char bmCapabilities;

} S_cdc_abstract_control_management_descriptor;

//! \brief  Union functional descriptors
//!
//!         Describes the relationship between a group of interfaces that can
//!         be considered to form a functional unit.
//! \see    usbcdc11.pdf - Section 5.2.3.8
__packed typedef struct {

    //! Size of this descriptor in bytes
    unsigned char bFunctionLength;
    //! CS_INTERFACE descriptor type
    unsigned char bDescriptorType;
    //! Union functional descriptor subtype
    unsigned char bDescriptorSubtype;
    //! The interface number designated as master
    unsigned char bMasterInterface;

} S_cdc_union_descriptor;

//! \brief  Union functional descriptors with one slave interface
//! \see    S_cdc_union_descriptor
__packed typedef struct {

    //! Union functional descriptor
    S_cdc_union_descriptor sUnion;
    //! Slave interface 0
    unsigned char          bSlaveInterfaces[1];

} S_cdc_union_1slave_descriptor;

//! \brief  Line coding structure
//!
//!         Format of the data returned when a GetLineCoding request is received
//! \see    usbcdc11.pdf - Section 6.2.13
__packed typedef struct {

    //! Data terminal rate in bits per second
    unsigned int dwDTERate;
    //! Number of stop bits
    char         bCharFormat;
    //! Parity bit type
    char         bParityType;
    //! Number of data bits
    char         bDataBits;

} S_cdc_line_coding;

#pragma pack()

//! \brief  Notification header
//!
//!         Data sent on the notification endpoint must follow this header.
//! \see    S_usb_request
typedef S_usb_request S_cdc_notification_header;

#endif // !defined(_CDC_DRIVER_H)
