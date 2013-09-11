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
$Id: serial_driver.h,v 1.1.2.1 2006/12/05 08:33:25 danielru Exp $
*/

#ifndef _CDC_SERIAL_DRIVER_H
#define _CDC_SERIAL_DRIVER_H

//------------------------------------------------------------------------------
//      Definitions
//------------------------------------------------------------------------------

//! \brief  Number of endpoint used by the CDC serial driver
#define SER_NUM_ENDPOINTS           4

//! \brief  Address of the bulk-OUT endpoint used by the data class interface
#define SER_EPT_DATA_OUT            0x01

//! \brief  Address of the bulk-IN endpoint used by the data class interface
#define SER_EPT_DATA_IN             0x02

//! \brief  Address of the interrupt-IN notification endpoint
#define SER_EPT_NOTIFICATION        0x03

//! \brief  Product ID expected by the host serial driver
#define SER_PRODUCT_ID              0x6119

// Status codes
#define SER_STATUS_SUCCESS          0x00
#define SER_STATUS_LOCKED           0x01
#define SER_STATUS_ERROR            0x02

//------------------------------------------------------------------------------
//      Structures
//------------------------------------------------------------------------------

//! \brief  Configuration descriptor for an Abstract Control Model device.
//! \see    usbcdc11.pdf - Section 3.6.2
__packed typedef struct {

    //! Standard USB configuration descriptor
    S_usb_configuration_descriptor               sCfg;
    //! Communication class interface
    S_usb_interface_descriptor                   sCommunication;
    //! Header functional descriptor
    S_cdc_header_descriptor                      sHeader;
    //! Call management functional descriptor
    S_cdc_call_management_descriptor             sCallManagement;
    //! Abstract control management functional descriptor
    S_cdc_abstract_control_management_descriptor sAbstract;
    //! Union functional descriptor
    S_cdc_union_1slave_descriptor                sUnion;
    //! Notification endpoint descriptor
    S_usb_endpoint_descriptor                    sNotification;
    //! Data class interface
    S_usb_interface_descriptor                   sData;
    //! Data out endpoint descriptor
    S_usb_endpoint_descriptor                    sDataOut;
    //! Data in endpoint descriptor
    S_usb_endpoint_descriptor                    sDataIn;

} S_ser_configuration_descriptor;

//! \brief  CDC class driver structure
typedef struct {

    S_std_class       sClass;
    S_cdc_line_coding sLineCoding;
    bool              isCarrierActivated;

} S_ser;

//------------------------------------------------------------------------------
//      Prototypes
//------------------------------------------------------------------------------

extern void SER_Init(S_ser *pSer, const S_usb *pUsb);
extern void SER_RequestHandler(S_ser *pSer);
extern unsigned char SER_Read(S_ser *pSer,
                              void *pBuffer,
                              unsigned int dLength,
                              Callback_f fCallback,
                              void *pArgument);
extern unsigned char SER_Write(S_ser *pSer,
                               void *pBuffer,
                               unsigned int dLength,
                               Callback_f fCallback,
                               void *pArgument);

#endif // !defined(_CDC_SERIAL_DRIVER_H)
