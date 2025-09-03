/*
 * usbd_framework.h
 *
 *  Created on: Mar 6, 2023
 *      Author: IsaiahOdunze
 */

#ifndef USBD_FRAMEWORK_H_
#define USBD_FRAMEWORK_H_
#include "usbd_driver.h"
#include "usb_device.h"

void usbd_initialize(UsbDevice *);
void usbd_poll_loop(UsbDevice *usb_device);


#endif /* USBD_FRAMEWORK_H_ */
