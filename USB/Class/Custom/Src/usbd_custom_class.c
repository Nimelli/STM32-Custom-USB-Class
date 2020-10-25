/**
  ******************************************************************************
  * @file    usbd_template.c
  * @author  MCD Application Team
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                CUSTOM_CLASS Class  Description
  *          ===================================================================
  *
  *
  *
  *
  *
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
  *
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_class.h"
#include "usbd_ctlreq.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_CUSTOM_CLASS
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_CUSTOM_CLASS_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_CUSTOM_CLASS_Private_Defines
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_CUSTOM_CLASS_Private_Macros
  * @{
  */

/**
  * @}
  */




/** @defgroup USBD_CUSTOM_CLASS_Private_FunctionPrototypes
  * @{
  */


static uint8_t USBD_CUSTOM_CLASS_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_CUSTOM_CLASS_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_CUSTOM_CLASS_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t USBD_CUSTOM_CLASS_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_CUSTOM_CLASS_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_CUSTOM_CLASS_EP0_RxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_CUSTOM_CLASS_EP0_TxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_CUSTOM_CLASS_SOF(USBD_HandleTypeDef *pdev);
static uint8_t USBD_CUSTOM_CLASS_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_CUSTOM_CLASS_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t *USBD_CUSTOM_CLASS_GetCfgDesc(uint16_t *length);
static uint8_t *USBD_CUSTOM_CLASS_GetDeviceQualifierDesc(uint16_t *length);
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_CLASS_Private_Variables
  * @{
  */

static uint8_t data_out_buffer[CUSTOM_CLASS_MAX_PACKET_SIZE] = {};

/* this struct will store some internal class data */
USBD_CUSTOM_CLASS_DataTypeDef custom_class_data =
{
  data_out_buffer,
  NULL,
  CUSTOM_CLASS_MAX_PACKET_SIZE,
  CUSTOM_CLASS_MAX_PACKET_SIZE,
  0,
  0,
};


USBD_ClassTypeDef USBD_CUSTOM_CLASS_ClassDriver =
{
  USBD_CUSTOM_CLASS_Init,
  USBD_CUSTOM_CLASS_DeInit,
  USBD_CUSTOM_CLASS_Setup,            /* not used in the current class implementation */
  USBD_CUSTOM_CLASS_EP0_TxReady,      /* not used in the current class implementation */
  USBD_CUSTOM_CLASS_EP0_RxReady,      /* not used in the current class implementation */
  USBD_CUSTOM_CLASS_DataIn,
  USBD_CUSTOM_CLASS_DataOut,
  USBD_CUSTOM_CLASS_SOF,              /* not used in the current class implementation */
  USBD_CUSTOM_CLASS_IsoINIncomplete,  /* not used in the current class implementation */
  USBD_CUSTOM_CLASS_IsoOutIncomplete, /* not used in the current class implementation */
  USBD_CUSTOM_CLASS_GetCfgDesc, // get config desc for HS
  USBD_CUSTOM_CLASS_GetCfgDesc, // get config desc for FS
  USBD_CUSTOM_CLASS_GetCfgDesc, // get config desc for otherspeed
  USBD_CUSTOM_CLASS_GetDeviceQualifierDesc,
};



#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
/* USB CUSTOM_CLASS device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_CUSTOM_CLASS_CfgDesc[USB_CUSTOM_CLASS_CONFIG_DESC_SIZ] __ALIGN_END =
{
  /* Configuration Descriptor */
  USB_LEN_CFG_DESC,              /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,   /* bDescriptorType: Configuration */
  USB_CUSTOM_CLASS_CONFIG_DESC_SIZ,  /* wTotalLength: Bytes returned */
  0x00,
  0x01,                          /*bNumInterfaces: 1 interface*/
  0x01,                          /*bConfigurationValue: Configuration value*/
  0x00,                          /*iConfiguration */
  0xC0,                          /*bmAttributes: bus powered and Supports Remote Wakeup */
  0x32,                          /*MaxPower 100 mA: this current is used for detecting Vbus*/

  /*---------------------------------------------------------------------------*/

  /* Interface Descriptor */
  USB_LEN_IF_DESC,          /* bLength */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: */
  0x01,                     /* bInterfaceNumber */
  0x00,                     /* bAlternateSetting */
  0x02,                     /* bNumEndpoints */
  0xFF,                     /* bInterfaceClass: 0xFF=vendor specific, can be set to 0x0A (CDC data) in this implementation*/
  0x00,                     /* bInterfaceSubClass */
  0x00,                     /* bInterfaceProtocol */
  0x00,                     /* iInterface */

  /*---------------------------------------------------------------------------*/

  /* Endpoint Descriptor */
  USB_LEN_EP_DESC,                      /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  CUSTOM_CLASS_EP1_OUT_ADDR,             /* bEndpointAddress */
  USBD_EP_TYPE_BULK,                    /* bmAttributes */
  LOBYTE(CUSTOM_CLASS_MAX_PACKET_SIZE), /* wMaxPacketSize */
  HIBYTE(CUSTOM_CLASS_MAX_PACKET_SIZE),
  0x00,                                 /* bInterval, ignore for bulk transfert */

  /*---------------------------------------------------------------------------*/

  /* Endpoint Descriptor */
  USB_LEN_EP_DESC,                      /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  CUSTOM_CLASS_EP1_IN_ADDR,              /* bEndpointAddress */
  USBD_EP_TYPE_BULK,                                 /* bmAttributes */
  LOBYTE(CUSTOM_CLASS_MAX_PACKET_SIZE), /* wMaxPacketSize */
  HIBYTE(CUSTOM_CLASS_MAX_PACKET_SIZE),
  0x00                                  /* bInterval, ignore for bulk transfert */

};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_CUSTOM_CLASS_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_CLASS_Private_Functions
  * @{
  */

/**
  * @brief  USBD_CUSTOM_CLASS_Init
  *         Initialize the CUSTOM_CLASS interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  /* Open EP1 IN */
  USBD_LL_OpenEP(pdev, CUSTOM_CLASS_EP1_IN_ADDR, USBD_EP_TYPE_BULK, CUSTOM_CLASS_MAX_PACKET_SIZE);
  pdev->ep_in[CUSTOM_CLASS_EP1_IN_ADDR & 0xFU].is_used = 1U;

  /* Open EP1 OUT */
  USBD_LL_OpenEP(pdev, CUSTOM_CLASS_EP1_OUT_ADDR, USBD_EP_TYPE_BULK, CUSTOM_CLASS_MAX_PACKET_SIZE);
  pdev->ep_in[CUSTOM_CLASS_EP1_OUT_ADDR & 0xFU].is_used = 1U;

  /* Call Interface Init  */
  ((USBD_CUSTOM_CLASS_ItfTypeDef *)pdev->pUserData)->Init();

  // register custom class data
  pdev->pClassData = (void *)&custom_class_data;

  /* Prepare Out endpoint to receive next packet */
  USBD_LL_PrepareReceive(pdev, CUSTOM_CLASS_EP1_OUT_ADDR, custom_class_data.RxBuffer, custom_class_data.RxLength);
  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CUSTOM_CLASS_Init
  *         DeInitialize the CUSTOM_CLASS layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  /* Close EP1 IN */
  USBD_LL_CloseEP(pdev, CUSTOM_CLASS_EP1_IN_ADDR);
  pdev->ep_in[CUSTOM_CLASS_EP1_IN_ADDR & 0xFU].is_used = 0U;

  /* Close EP1 OUT */
  USBD_LL_CloseEP(pdev, CUSTOM_CLASS_EP1_OUT_ADDR);
  pdev->ep_in[CUSTOM_CLASS_EP1_OUT_ADDR & 0xFU].is_used = 0U;

  /* Call Interface DeInit  */
  ((USBD_CUSTOM_CLASS_ItfTypeDef *)pdev->pUserData)->DeInit();

  pdev->pClassData = NULL;

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CUSTOM_CLASS_Setup
  *         Handle the CUSTOM_CLASS specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_Setup(USBD_HandleTypeDef *pdev,
                                   USBD_SetupReqTypedef *req)
{
  USBD_StatusTypeDef ret = USBD_OK;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :
    switch (req->bRequest)
    {
    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  default:
    USBD_CtlError(pdev, req);
    ret = USBD_FAIL;
    break;
  }

  return (uint8_t)ret;
}


/**
  * @brief  USBD_CUSTOM_CLASS_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_CUSTOM_CLASS_GetCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_CUSTOM_CLASS_CfgDesc);
  return USBD_CUSTOM_CLASS_CfgDesc;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t *USBD_CUSTOM_CLASS_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_CUSTOM_CLASS_DeviceQualifierDesc);

  return USBD_CUSTOM_CLASS_DeviceQualifierDesc;
}


/**
  * @brief  USBD_CUSTOM_CLASS_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_CUSTOM_CLASS_DataTypeDef *data;
  // retreive custom class data
  data = (USBD_CUSTOM_CLASS_DataTypeDef *)pdev->pClassData;

  // A bulk transfer is complete when the endpoint does on of the following:
  // - Has transferred exactly the amount of data expected
  // - Transfers a packet with a payload size less than wMaxPacketSize or transfers a zero-length packet
  if ( (pdev->ep_in[epnum].total_length > 0) &&
    ((pdev->ep_in[epnum].total_length % CUSTOM_CLASS_MAX_PACKET_SIZE) == 0U) )
  {
    /* Update the packet total length */
    pdev->ep_in[epnum].total_length = 0;

    /* Send ZLP */
    USBD_LL_Transmit(pdev, epnum, NULL, 0);
  }
  else
  {
    /* transmit complete */
    data->TxState = 0U; // flag data transmit done

    /* call interface transmit cplt callback */
    ((USBD_CUSTOM_CLASS_ItfTypeDef *)pdev->pUserData)->TransmitCpltCallback(pdev, data->TxBuffer, data->TxLength, epnum);
  }

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_CUSTOM_CLASS_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  // USBD_CUSTOM_CLASS_DataTypeDef *data;
  // retreive custom class data
  // data = (USBD_CUSTOM_CLASS_DataTypeDef *)pdev->pClassData;

  size_t const bytes_received = USBD_LL_GetRxDataSize(pdev, epnum);

  USBD_CUSTOM_CLASS_ReceivedPacket(pdev, bytes_received);

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CUSTOM_CLASS_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_EP0_RxReady(USBD_HandleTypeDef *pdev)
{

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_CUSTOM_CLASS_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_EP0_TxReady(USBD_HandleTypeDef *pdev)
{

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_CUSTOM_CLASS_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_SOF(USBD_HandleTypeDef *pdev)
{

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_CUSTOM_CLASS_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_CUSTOM_CLASS_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_CUSTOM_CLASS_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CUSTOM_CLASS_TransmitPacket
  *         Transmit packet on IN endpoint
  * @param  pdev: device instance
  * @retval status
  */
uint8_t USBD_CUSTOM_CLASS_TransmitPacket(USBD_HandleTypeDef *pdev, uint8_t* buf, uint16_t length)
{
  USBD_StatusTypeDef ret = USBD_BUSY;

  USBD_CUSTOM_CLASS_DataTypeDef *data;
  // retreive custom class data
  data = (USBD_CUSTOM_CLASS_DataTypeDef *)pdev->pClassData;

  if(data->TxState) // transfert in progress (busy)
  {
    return USBD_BUSY;
  }

  data->TxBuffer = buf;
  data->TxLength = length;

  /* Update the packet total length */
  pdev->ep_in[CUSTOM_CLASS_EP1_IN_ADDR & 0xFU].total_length = length;

  /* Transmit next packet */
  ret = USBD_LL_Transmit(pdev, CUSTOM_CLASS_EP1_IN_ADDR, data->TxBuffer, data->TxLength);
  data->TxState = 1; // transfert in progress (busy)

  return (uint8_t)ret;
}

/**
  * @brief  USBD_CUSTOM_CLASS_ReceivedPacket
  *         callback when a packet was received
  *         prepare OUT Endpoint for the next packet reception
  * @param  pdev: device instance
  * @retval status
  */
uint8_t USBD_CUSTOM_CLASS_ReceivedPacket(USBD_HandleTypeDef *pdev, size_t const bytes_received)
{
  USBD_CUSTOM_CLASS_DataTypeDef *data;
  // retreive custom class data
  data = (USBD_CUSTOM_CLASS_DataTypeDef *)pdev->pClassData;

  /* Call interface callback here to copy rx buffer to application */
  ((USBD_CUSTOM_CLASS_ItfTypeDef *)pdev->pUserData)->ReceiveCallback(pdev, data->RxBuffer, bytes_received);

  /* Prepare Out endpoint to receive next packet */
  USBD_LL_PrepareReceive(pdev, CUSTOM_CLASS_EP1_OUT_ADDR, data->RxBuffer, data->RxLength);

  return (uint8_t)USBD_OK;
}

uint8_t USBD_CUSTOM_CLASS_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_CUSTOM_CLASS_ItfTypeDef *intf)
{
  if (intf == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  /* refister the interface */
  pdev->pUserData = intf;

  return (uint8_t)USBD_OK;
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
