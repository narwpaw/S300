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
$Id: serial_driver.c,v 1.1.2.1 2006/12/05 08:33:25 danielru Exp $
*/

//------------------------------------------------------------------------------
//      Includes
//------------------------------------------------------------------------------

#include "core/common.h"
#include "core/device.h"
#include "core/board.h"
#include "core/trace.h"
#include "core/usb.h"
#include "core/standard.h"
#include "cdc.h"
#include "serial_driver.h"

//------------------------------------------------------------------------------
//      Global variables
//------------------------------------------------------------------------------

// Descriptors
//------------------------------------------------------------------------------
//! \brief  Standard USB device descriptor
//! \see    S_usb_device_descriptor
const S_usb_device_descriptor sDevice = {

    sizeof(S_usb_device_descriptor), // Size of this descriptor
    USB_DEVICE_DESCRIPTOR,           // DEVICE Descriptor Type
    USB1_10,                         // USB 1.10 specification
    USB_CLASS_COMMUNICATION,         // USB Communication class code
    0x00,                            // No device subclass code
    0x00,                            // No device protocol code
    USB_ENDPOINT0_MAXPACKETSIZE,     // Maximum packet size for endpoint zero
    USB_VENDOR_ATMEL,                // ATMEL Vendor ID
    SER_PRODUCT_ID,                  // Product ID (6119)
    0x0001,                          // Device release number 0.01
    0x01,                            // Index of manufacturer description
    0x02,                            // Index of product description
    0x03,                            // Index of serial number description
    0x01                             // One possible configuration
};

//! \brief  Device configuration descriptor
//! \see    S_ser_configuration_descriptor
const S_ser_configuration_descriptor sConfiguration = {

    // Standard configuration descriptor
    {
        // Size of this descriptor
        sizeof(S_usb_configuration_descriptor),
        // CONFIGURATION descriptor type
        USB_CONFIGURATION_DESCRIPTOR,
        // Total size of this configuration (including other descriptors)
        sizeof(S_ser_configuration_descriptor),
        // Two interfaces are used by this configuration
        0x02,
        // Value 0x01 is used to select this configuration
        0x01,
        // No string is used to describe this configuration
        0x00,
        // Device is self-powered and does not support remote wakeup
        USB_CONFIG_SELF_WAKEUP,
        // Maximum power consumption of the device is 100mA
        USB_POWER_MA(100)
    },
    // Communication class interface descriptor
    {
        sizeof(S_usb_interface_descriptor), // Size of this descriptor in bytes
        USB_INTERFACE_DESCRIPTOR,           // INTERFACE Descriptor Type
        0x00,                               // Interface 0
        0x00,                               // No alternate settings
        0x01,                               // One endpoint used
        CDC_INTERFACE_COMMUNICATION,        // Communication interface class
        CDC_ABSTRACT_CONTROL_MODEL,         // Abstract control model subclass
        0x01,                               // No protocol code
        0x00                                // No associated string descriptor
    },
    // Header functional descriptor
    {
        sizeof(S_cdc_header_descriptor), // Size of this descriptor in bytes
        CDC_CS_INTERFACE,                // CS_INTERFACE descriptor type
        CDC_HEADER,                      // Header functional descriptor
        CDC1_10,                         // CDC version 1.10
    },
    // Call management functional descriptor
    {
        sizeof(S_cdc_call_management_descriptor), // Size of this descriptor
        CDC_CS_INTERFACE,                         // CS_INTERFACE type
        CDC_CALL_MANAGEMENT,                      // Call management descriptor
        0x01,                                     // Call management is
                                                  // handled by the device
        0x01                                      // Data interface is 0x01
    },
    // Abstract control management functional descriptor
    {
        // Size of this descriptor in bytes
        sizeof(S_cdc_abstract_control_management_descriptor),
        // CS_INTERFACE descriptor type
        CDC_CS_INTERFACE,
        // Abstract control management functional descriptor
        CDC_ABSTRACT_CONTROL_MANAGEMENT,
        // Every notification/request except NetworkConnection supported
        0x07
    },
    // Union functional descriptor with one slave interface
    {
        // Union functional descriptor
        {
            sizeof(S_cdc_union_descriptor)+1, // Size of this descriptor
            CDC_CS_INTERFACE,                 // CS_INTERFACE descriptor type
            CDC_UNION,                        // Union functional descriptor
            0x00,                             // Master interface is 0x00
        },                                    // (Communication class interface)
        0x01                                  // First slave interface is 0x01
    },                                        // (Data class interface)
    // Notification endpoint descriptor
    {
        sizeof(S_usb_endpoint_descriptor),  // Size of this descriptor in bytes
        USB_ENDPOINT_DESCRIPTOR,            // ENDPOINT descriptor type
        USB_ENDPOINT_IN | SER_EPT_NOTIFICATION, // IN endpoint, address = 0x03
        ENDPOINT_TYPE_INTERRUPT,            // INTERRUPT endpoint type
        64,                                 // Maximum packet size is 64 bytes
        0x10                                // Endpoint polled every 10ms
    },
    // Data class interface descriptor
    {
        sizeof(S_usb_interface_descriptor), // Size of this descriptor in bytes
        USB_INTERFACE_DESCRIPTOR,           // INTERFACE descriptor type
        0x01,                               // Interface 0x01
        0x00,                               // No alternate settings
        0x02,                               // Two endpoints used
        CDC_INTERFACE_DATA,                 // Data class code
        0x00,                               // No subclass code
        0x00,                               // No protocol code
        0x00                                // No description string
    },
    // Bulk-OUT endpoint descriptor
    {
        sizeof(S_usb_endpoint_descriptor), // Size of this descriptor in bytes
        USB_ENDPOINT_DESCRIPTOR,           // ENDPOINT descriptor type
        USB_ENDPOINT_OUT | SER_EPT_DATA_OUT,   // OUT endpoint, address = 0x01
        ENDPOINT_TYPE_BULK,                // Bulk endpoint
        64,                                // Endpoint size is 64 bytes
        0x00                               // Must be 0x00 for full-speed bulk
    },                                     // endpoints
    // Bulk-IN endpoint descriptor
    {
        sizeof(S_usb_endpoint_descriptor), // Size of this descriptor in bytes
        USB_ENDPOINT_DESCRIPTOR,           // ENDPOINT descriptor type
        USB_ENDPOINT_IN | SER_EPT_DATA_IN,     // IN endpoint, address = 0x02
        ENDPOINT_TYPE_BULK,                // Bulk endpoint
        64,                                // Endpoint size is 64 bytes
        0x00                               // Must be 0x00 for full-speed bulk
    },                                     // endpoints
};

//! \brief  Language ID string descriptor
const S_usb_language_id sLanguageID = {

    USB_STRING_DESCRIPTOR_SIZE(1),
    USB_STRING_DESCRIPTOR,
    USB_LANGUAGE_ENGLISH_US
};

//! \brief  Manufacturer string descriptor
const char pManufacturer[] = {

    USB_STRING_DESCRIPTOR_SIZE(5),
    USB_STRING_DESCRIPTOR,
    USB_UNICODE('A'),
    USB_UNICODE('T'),
    USB_UNICODE('M'),
    USB_UNICODE('E'),
    USB_UNICODE('L')
};

//! \brief  Product string descriptor
const char pProduct[] = {

    USB_STRING_DESCRIPTOR_SIZE(13),
    USB_STRING_DESCRIPTOR,
    USB_UNICODE('A'),
    USB_UNICODE('T'),
    USB_UNICODE('9'),
    USB_UNICODE('1'),
    USB_UNICODE('U'),
    USB_UNICODE('S'),
    USB_UNICODE('B'),
    USB_UNICODE('S'),
    USB_UNICODE('e'),
    USB_UNICODE('r'),
    USB_UNICODE('i'),
    USB_UNICODE('a'),
    USB_UNICODE('l')
};

//! \brief  Serial number string descriptor
const char pSerialNumber[] = {

    USB_STRING_DESCRIPTOR_SIZE(12),
    USB_STRING_DESCRIPTOR,
    USB_UNICODE('0'),
    USB_UNICODE('1'),
    USB_UNICODE('2'),
    USB_UNICODE('3'),
    USB_UNICODE('4'),
    USB_UNICODE('5'),
    USB_UNICODE('6'),
    USB_UNICODE('7'),
    USB_UNICODE('8'),
    USB_UNICODE('9'),
    USB_UNICODE('A'),
    USB_UNICODE('F')
};

//! \brief  List of string descriptors
const char *pStrings[] = {

    (char *) &sLanguageID,
    pManufacturer,
    pProduct,
    pSerialNumber
};

//! \brief List of endpoint descriptors
const S_usb_endpoint_descriptor *pEndpoints[] = {

    &(sConfiguration.sDataOut),
    &(sConfiguration.sDataIn),
    &(sConfiguration.sNotification)
};

//! \brief  Standard descriptors list
const S_std_descriptors sDescriptors = {

    &sDevice,
    (S_usb_configuration_descriptor *) &sConfiguration,
    pStrings,
    pEndpoints
};

//------------------------------------------------------------------------------
//      Internal functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//! \brief  Sets asynchronous line-character formatting properties
//!
//!         This function is used as a callback when receiving the data part
//!         of a SET_LINE_CODING request.
//! \param  pSer         Pointer to a S_ser instance
//! \see    S_ser
//! \see    usbcdc11.pdf - Section 6.2.12
//------------------------------------------------------------------------------
static void ACM_SetLineCoding(const S_ser *pSer)
{
    USB_SendZLP0(pSer->sClass.pUsb, 0, 0);
}

//------------------------------------------------------------------------------
//! \brief  Sends the currently configured line coding to the host
//! \param  pSer               Pointer to a S_ser instance
//! \see    S_ser
//! \see    usbcdc11.pdf - Section 6.2.13
//------------------------------------------------------------------------------
static void ACM_GetLineCoding(const S_ser *pSer)
{
    USB_Write(pSer->sClass.pUsb,
              0,
              (void *) &(pSer->sLineCoding),
              sizeof(S_cdc_line_coding),
              0,
              0);
}

//------------------------------------------------------------------------------
//! \brief  Sets the state of control line parameters.
//! \param  pSer              Pointer to a S_ser instance
//! \param  isActivateCarrier Indicates if the device should activate
//!                           its carrier
//! \param  isDTEPresent      Indicates if the terminal is present
//! \see    S_ser
//! \see    usbcdc11.pdf - Section 6.2.14
//------------------------------------------------------------------------------
static void ACM_SetControlLineState(S_ser *pSer,
                                    bool  isActivateCarrier,
                                    bool  isDTEPresent)
{
    pSer->isCarrierActivated = isActivateCarrier;
    USB_SendZLP0(pSer->sClass.pUsb, 0, 0);
}

//------------------------------------------------------------------------------
//      Exported functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//! \brief  SETUP request handler for an Abstract Control Model device
//! \param  pSer Pointer to a S_ser instance
//! \see    S_ser
//! \see    usbcdc11.pdf - Section 6.2
//------------------------------------------------------------------------------
void SER_RequestHandler(S_ser *pSer)
{
    S_usb_request *pSetup = USB_GetSetup(pSer->sClass.pUsb);

    //TRACE_DEBUG_M("NewReq ");

    // Handle the request
    switch (pSetup->bRequest) {

    //-----------------------
    case CDC_SET_LINE_CODING:
    //-----------------------
        //TRACE_DEBUG_M("sLineCoding ");

        // Start the read operation with ACM_SetLineCoding as the callback
        USB_Read(pSer->sClass.pUsb,
                 0,
                 (void *) &(pSer->sLineCoding),
                 sizeof(S_cdc_line_coding),
                 (Callback_f) ACM_SetLineCoding,
                 pSer);

        break;

    //-----------------------
    case CDC_GET_LINE_CODING:
    //-----------------------
        TRACE_DEBUG_M("gLineCoding ");
        ACM_GetLineCoding(pSer);

        break;

    //------------------------------
    case CDC_SET_CONTROL_LINE_STATE:
    //------------------------------
        {
            bool isActivateCarrier = false;
            bool isDTEPresent = false;

            //TRACE_DEBUG_M("sControlLineState(");

            if (ISSET(pSetup->wValue, CDC_DTE_PRESENT)) {

                isDTEPresent = true;
            }

            if (ISSET(pSetup->wValue, CDC_ACTIVATE_CARRIER)) {

                isActivateCarrier = true;
            }

            //TRACE_DEBUG_M("%d,%d) ", isDTEPresent, isActivateCarrier);

            ACM_SetControlLineState(pSer, isActivateCarrier, isDTEPresent);
        }
        break;

    //------
    default:
    //------
        // Forward request to standard request handler
        STD_RequestHandler((S_std_class *) pSer);

        break;
    }
}

//------------------------------------------------------------------------------
//! \brief  Initializes a CDC serial driver
//!
//!         This method sets the standard descriptors of the device and the
//!         default CDC configuration.
//! \param  pSer Pointer to a S_ser instance
//! \param  pUsb Pointer to the S_usb driver instance to use
//! \see    S_ser
//! \see    S_usb
//------------------------------------------------------------------------------
void SER_Init(S_ser *pSer, const S_usb *pUsb)
{
    // Initialize standard class attributes
    pSer->sClass.pUsb = pUsb;
    pSer->sClass.pDescriptors = &sDescriptors;

    // Initialize ACM attributes
    // Line coding
    pSer->sLineCoding.dwDTERate = 0;
    pSer->sLineCoding.bCharFormat = 0;
    pSer->sLineCoding.bParityType = 0;
    pSer->sLineCoding.bDataBits = 0;

    // Carrier
    pSer->isCarrierActivated = false;

    // Initialize the USB driver
    USB_Init(pUsb);
}

//------------------------------------------------------------------------------
//! \brief  Reads data from the Data OUT endpoint
//! \param  pSer      Pointer to a S_ser instance
//! \param  pBuffer   Buffer in which to store the received data
//! \param  dLength   Length of data buffer
//! \param  fCallback Optional callback function
//! \param  pArgument Optional parameter for the callback function
//! \return SER_STATUS_SUCCESS if transfer has started successfully;
//!         SER_STATUS_LOCKED if endpoint is currently in use;
//!         SER_STATUS_ERROR if transfer cannot be started.
//------------------------------------------------------------------------------
unsigned char SER_Read(S_ser *pSer,
                       void *pBuffer,
                       unsigned int dLength,
                       Callback_f fCallback,
                       void *pArgument)
{
    return USB_Read(pSer->sClass.pUsb, SER_EPT_DATA_OUT, pBuffer,
                    dLength, fCallback, pArgument);
}

//------------------------------------------------------------------------------
//! \brief  Sends data through the Data IN endpoint
//! \param  pSer      Pointer to a S_ser instance
//! \param  pBuffer   Buffer holding the data to transmit
//! \param  dLength   Length of data buffer
//! \param  fCallback Optional callback function
//! \param  pArgument Optional parameter for the callback function
//! \return SER_STATUS_SUCCESS if transfer has started successfully;
//!         SER_STATUS_LOCKED if endpoint is currently in use;
//!         SER_STATUS_ERROR if transfer cannot be started.
//------------------------------------------------------------------------------
unsigned char SER_Write(S_ser *pSer,
                        void *pBuffer,
                        unsigned int dLength,
                        Callback_f fCallback,
                        void *pArgument)
{
    return USB_Write(pSer->sClass.pUsb, SER_EPT_DATA_IN, pBuffer,
                     dLength, fCallback, pArgument);
}
