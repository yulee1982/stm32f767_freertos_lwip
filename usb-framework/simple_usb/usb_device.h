/*
 * usb_device.h
 *
 *  Created on: May 30, 2023
 *      Author: IsaiahOdunze
 */

#ifndef USB_DEVICE_H_
#define USB_DEVICE_H_


typedef struct
{
  uint8_t device_mode;

  //current usb device state
  UsbDeviceState device_state;

  //current control transfer stage (for endpoint0)
  UsbControlTransferStage control_transfer_stage;

  //Selected usb configuration
  uint8_t configuration_value;

  //UsbDevice out/in buffer pointers
  void const *ptr_out_buffer;
  uint32_t out_data_size;
  void const *ptr_in_buffer;
  uint32_t in_data_size;

}UsbDevice;

#endif /* USB_DEVICE_H_ */
