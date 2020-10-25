/**
  ******************************************************************************
  * @file    usbd_template_core.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_template_core.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CUSTOM_CLASS_CORE_H
#define __USB_CUSTOM_CLASS_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_CUSTOM_CLASS
  * @brief This file is the header file for usbd_template_core.c
  * @{
  */


/** @defgroup USBD_CUSTOM_CLASS_Exported_Defines
  * @{
  */
#define CUSTOM_CLASS_EP1_OUT_ADDR                0x01U  /* EP1 for data OUT */
#define CUSTOM_CLASS_EP1_IN_ADDR                 0x81U  /* EP1 for data IN */

#define CUSTOM_CLASS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */

#define USB_CUSTOM_CLASS_CONFIG_DESC_SIZ       32U

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

typedef struct
{
  uint8_t (* Init)(void);
  uint8_t (* DeInit)(void);
  // uint8_t (* Control)(uint8_t cmd, uint8_t *pbuf, uint16_t length); /* not used in the current class implementation */
  uint8_t (* TransmitCpltCallback)(USBD_HandleTypeDef *pdev, uint8_t *Buf, uint32_t len, uint8_t epnum);
  uint8_t (* ReceiveCallback)(USBD_HandleTypeDef *pdev, uint8_t *Buf, uint32_t len);
} USBD_CUSTOM_CLASS_ItfTypeDef;

typedef struct
{
  uint8_t  *RxBuffer;     /* buffer used to receive (OUT) data */
  uint8_t  *TxBuffer;     /* buffer that is sending to (IN) */
  uint32_t RxLength;      /* bufer length */
  uint32_t TxLength;      /* bufer length */
  __IO uint32_t TxState;  /* flag to notify if a transmition is already ongoing (busy) */
  __IO uint32_t RxState;  /* not used in the current class implementation */
} USBD_CUSTOM_CLASS_DataTypeDef;

/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_CUSTOM_CLASS_ClassDriver;
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_CUSTOM_CLASS_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_CUSTOM_CLASS_ItfTypeDef *intf);
uint8_t USBD_CUSTOM_CLASS_TransmitPacket(USBD_HandleTypeDef *pdev, uint8_t* buf, uint16_t length);
uint8_t USBD_CUSTOM_CLASS_ReceivedPacket(USBD_HandleTypeDef *pdev, size_t const bytes_received);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_CUSTOM_CLASS_CORE_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
