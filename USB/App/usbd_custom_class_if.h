
#ifndef __USBD_CUSTOM_CLASS_IF_H
#define __USBD_CUSTOM_CLASS_IF_H

#ifdef __cplusplus
extern "C" {
#endif


#include "usbd_custom_class.h"

/* send data */
uint8_t custom_class_transmit_data(USBD_HandleTypeDef *pdev, uint8_t *pbuf, uint16_t len);

/* store Init function and callback */
extern USBD_CUSTOM_CLASS_ItfTypeDef  USBD_CUSTOM_CLASS_interface;


#ifdef __cplusplus
}
#endif

#endif /* __USBD_CUSTOM_CLASS_IF_TEMPLATE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
