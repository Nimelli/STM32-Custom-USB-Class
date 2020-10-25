
#include "usbd_custom_class_if.h"

#include "term.h"


static uint8_t Custom_Class_Init(void);
static uint8_t Custom_Class_DeInit(void);
//static uint8_t Custom_Class_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static uint8_t Custom_Class_TransmitCplt_callback(USBD_HandleTypeDef* pDev, uint8_t *pbuf, uint32_t len, uint8_t epnum);
static uint8_t Custom_Class_Receive_callback(USBD_HandleTypeDef* pDev, uint8_t *pbuf, uint32_t len);

/* function for sending data */
uint8_t custom_class_transmit_data(USBD_HandleTypeDef* pDev, uint8_t *pbuf, uint16_t len)
{
  return USBD_CUSTOM_CLASS_TransmitPacket(pDev, pbuf, len);
}

USBD_CUSTOM_CLASS_ItfTypeDef USBD_CUSTOM_CLASS_interface =
{
  Custom_Class_Init,
  Custom_Class_DeInit,
  // Custom_Class_Control, /* not used for this custom class implementation */
  Custom_Class_TransmitCplt_callback,
  Custom_Class_Receive_callback
};


static uint8_t Custom_Class_Init(void)
{
  term_printf("Custom Class Interface Init\r\n");
  /*  add application-level initialization if needed */
  return (uint8_t)USBD_OK;
}


static uint8_t Custom_Class_DeInit(void)
{
  term_printf("Custom Class Interface DeInit\r\n");
  /*  add application-level deinitialization if needed */
  return (uint8_t)USBD_OK;
}


/*static uint8_t Custom_Class_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
  return (uint8_t)USBD_OK;
}*/


/* called by custom_class implementation when a transmit is complete */
static uint8_t Custom_Class_TransmitCplt_callback(USBD_HandleTypeDef* pDev, uint8_t *Buf, uint32_t len, uint8_t epnum)
{
  term_printf("Custom Class Transmit Complete\r\n");
  return (uint8_t)USBD_OK;
}

/* called by custom_class implementation when data are received */
static uint8_t Custom_Class_Receive_callback(USBD_HandleTypeDef* pDev, uint8_t *Buf, uint32_t len)
{
  term_printf("Custom Class Receive new data: %ld, bytes\r\n", len);


  // echoing received data (for testing)
  custom_class_transmit_data(pDev, Buf, len);

  return (uint8_t)USBD_OK;
}


