/*
 * usbd_driver.h
 *
 *  Created on: Mar 6, 2023
 *      Author: IsaiahOdunze
 */

#ifndef USBD_DRIVER_H_
#define USBD_DRIVER_H_

#include "stm32f767xx.h"
#include "stm32f7xx.h"
#include "usb_standards.h"
#include "main.h"
/** @defgroup USB_Core_Mode_ USB Core Mode
  * @{
  */
#define USBD_OTG_MODE_DEVICE                    0U
#define USBD_OTG_MODE_HOST                      1U
#define USBD_OTG_MODE_DRD                       2U

/** @defgroup USB_LL_Core_Speed USB Low Layer Core Speed
  * @{
  */
#define USBD_OTG_SPEED_HIGH                     0U
#define USBD_OTG_SPEED_HIGH_IN_FULL             1U
#define USBD_OTG_SPEED_FULL                     3U

/**\addtogroup USBD_CORE USB device core
 * \brief Contains core API
 * @{ */
#define USB_EPTYPE_DBLBUF   0x04    /**<\brief Doublebuffered endpoint (bulk endpoint only).*/
/** \anchor USB_ENDPOINT_DEF
 *  \name USB endpoint attributes definitions
 * @{ */
#define USB_EPDIR_IN                0x00    /**<\brief Host-to-device endpoint direction.*/
#define USB_EPDIR_OUT               0x80    /**<\brief Device-to-host endpoint direction.*/
#define USB_EPTYPE_CONTROL          0x00    /**<\brief Control endpoint.*/
#define USB_EPTYPE_ISOCHRONUS       0x01    /**<\brief Isochronous endpoint.*/
#define USB_EPTYPE_BULK             0x02    /**<\brief Bbulk endpoint.*/
#define USB_EPTYPE_INTERRUPT        0x03    /**<\brief Interrupt endpoint.*/
#define USB_EPATTR_NO_SYNC          0x00    /**<\brief No synchronization.*/
#define USB_EPATTR_ASYNC            0x04    /**<\brief Asynchronous endpoint.*/
#define USB_EPATTR_ADAPTIVE         0x08    /**<\brief Adaptive endpoint.*/
#define USB_EPATTR_SYNC             0x0C    /**<\brief Synchronous endpoint.*/
#define USB_EPUSAGE_DATA            0x00    /**<\brief Data endpoint.*/
#define USB_EPUSAGE_FEEDBACK        0x10    /**<\brief Feedback endpoint.*/
#define USB_EPUSAGE_IMP_FEEDBACK    0x20    /**<\brief Implicit feedback Data endpoint.*/

#define USB_OTG_HS_GLOBAL ((USB_OTG_GlobalTypeDef *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_GLOBAL_BASE))
#define USB_OTG_HS_DEVICE ((USB_OTG_DeviceTypeDef *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#define USB_OTG_HS_PCGCCTL ((uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_PCGCCTL_BASE))

#define USB_OTG_FS_GLOBAL ((USB_OTG_GlobalTypeDef *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_GLOBAL_BASE))
#define USB_OTG_FS_DEVICE ((USB_OTG_DeviceTypeDef *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#define USB_OTG_FS_PCGCCTL ((uint32_t *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_PCGCCTL_BASE))

//total IN or OUT endpoints
#define ENDPOINT_COUNT 6


// \Description returns the structure containing the registers of a specific IN endpoint
// \param endpoint_number The number of the In endpoint that we want to access the registers of.
inline static USB_OTG_INEndpointTypeDef * IN_ENDPOINT(uint8_t endpoint_number)
{
  return (USB_OTG_INEndpointTypeDef *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_IN_ENDPOINT_BASE + (endpoint_number << 5)); //(endpoint_number * 0x20)
}

// \Description returns the structure containing the registers of a specific OUT endpoint
// \param endpoint_number The number of the OUT endpoint that we want to access the registers of.
inline static USB_OTG_OUTEndpointTypeDef * OUT_ENDPOINT(uint8_t endpoint_number)
{
  return (USB_OTG_OUTEndpointTypeDef *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_OUT_ENDPOINT_BASE + (endpoint_number << 5)); //(endpoint_number * 0x20)
}

inline static __IO uint32_t *FIFO(uint8_t endpoint_number)
{
  return (__IO uint32_t *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_FIFO_BASE + (endpoint_number << 12));//(endpoint_number * 0x1000)
}

typedef struct
{
  void (*initialize_core)();
  void (*initialize_gpio_pins)();
  void (*set_device_address)(uint8_t address);
  void (*connect)(uint8_t connect);
  void (*flush_rxfifo)();
  void (*flush_txfifo)(uint8_t enpoint_number);
  void (*configure_in_endpoint)(uint8_t endpoint_number, enum UsbEndpointType endpoint_type, uint16_t endpoint_size);
  void (*read_packet)(void const *buffer, uint16_t size);
  void (*write_packet)(uint8_t endpoint_number, void const *buffer, uint16_t size);
  void (*poll)();
  //TODO Add pointer to the other driver functions

} UsbDriver;

extern const UsbDriver usb_driver;
extern UsbEvents usb_events;

#endif /* USBD_DRIVER_H_ */
