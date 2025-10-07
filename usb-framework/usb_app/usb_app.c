
//USE_LIBUSB_STM32 or USE_SIMPLE_USB or USE_CHERRY_USB Choose one of the three

//#define USE_SIMPLE_USB
#define USE_LIBUSB_STM32


#include "FreeRTOS.h"
#include "task.h"

#if defined(USE_LIBUSB_STM32)
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32_compat.h"
#include "usb.h"
#include "usb_cdc.h"
#include "usb_hid.h"
#include "hid_usage_desktop.h"
#include "hid_usage_button.h"

#define CDC_EP0_SIZE    0x08
#define CDC_RXD_EP      0x01
#define CDC_TXD_EP      0x81
#define CDC_DATA_SZ     0x40
#define CDC_NTF_EP      0x82
#define CDC_NTF_SZ      0x08
#define HID_RIN_EP      0x83
#define HID_RIN_SZ      0x10

#define CDC_LOOPBACK
#define ENABLE_HID_COMBO

//#define SIGNAL_MODEM  /* uncomment to signal modem capabilities */
#define CDC_USE_IRQ   /* uncomment to build interrupt-based demo */


#if defined(SIGNAL_MODEM)
#define CDC_PROTOCOL USB_CDC_PROTO_V25TER
#else
#define CDC_PROTOCOL USB_PROTO_NONE
#endif

/* Declaration of the report descriptor */
struct cdc_config {
    struct usb_config_descriptor        config;
    struct usb_iad_descriptor           comm_iad;
    struct usb_interface_descriptor     comm;
    struct usb_cdc_header_desc          cdc_hdr;
    struct usb_cdc_call_mgmt_desc       cdc_mgmt;
    struct usb_cdc_acm_desc             cdc_acm;
    struct usb_cdc_union_desc           cdc_union;
    struct usb_endpoint_descriptor      comm_ep;
    struct usb_interface_descriptor     data;
    struct usb_endpoint_descriptor      data_eprx;
    struct usb_endpoint_descriptor      data_eptx;
#ifdef ENABLE_HID_COMBO
    struct usb_interface_descriptor     hid;
    struct usb_hid_descriptor           hid_desc;
    struct usb_endpoint_descriptor      hid_ep;
#endif //ENABLE_HID_COMBO
} __attribute__((packed));

/* HID mouse report desscriptor. 2 axis 5 buttons */
static const uint8_t hid_report_desc[] =
{
  HID_USAGE_PAGE(HID_PAGE_DESKTOP),
  HID_USAGE(HID_DESKTOP_MOUSE),
  HID_COLLECTION(HID_APPLICATION_COLLECTION),
    HID_USAGE(HID_DESKTOP_POINTER),
    HID_COLLECTION(HID_PHYSICAL_COLLECTION),
      HID_USAGE(HID_DESKTOP_X),
      HID_USAGE(HID_DESKTOP_Y),
      HID_LOGICAL_MINIMUM(-127),
      HID_LOGICAL_MAXIMUM(127),
      HID_REPORT_SIZE(8),
      HID_REPORT_COUNT(2),
      HID_INPUT(HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE ),
      HID_USAGE_PAGE(HID_PAGE_BUTTON),
      HID_USAGE_MINIMUM(1),
      HID_USAGE_MAXIMUM(5),
      HID_LOGICAL_MINIMUM(0),
      HID_LOGICAL_MAXIMUM(1),
      HID_REPORT_SIZE(1),
      HID_REPORT_COUNT(5),
      HID_INPUT(HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE ),
      HID_REPORT_SIZE(1),
      HID_REPORT_COUNT(3),
      HID_INPUT(HID_IOF_CONSTANT),
    HID_END_COLLECTION,
  HID_END_COLLECTION,
};

/* Device descriptor */
static const struct usb_device_descriptor device_desc = {
    .bLength            = sizeof(struct usb_device_descriptor),
    .bDescriptorType    = USB_DTYPE_DEVICE,
    .bcdUSB             = VERSION_BCD(2,0,0),
    .bDeviceClass       = USB_CLASS_IAD,
    .bDeviceSubClass    = USB_SUBCLASS_IAD,
    .bDeviceProtocol    = USB_PROTO_IAD,
    .bMaxPacketSize0    = CDC_EP0_SIZE,
    .idVendor           = 0x0483,
    .idProduct          = 0x5740,
    .bcdDevice          = VERSION_BCD(1,0,0),
    .iManufacturer      = 1,
    .iProduct           = 2,
    .iSerialNumber      = INTSERIALNO_DESCRIPTOR,
    .bNumConfigurations = 1,
};

/* Device configuration descriptor */
static const struct cdc_config config_desc = {
    .config = {
        .bLength                = sizeof(struct usb_config_descriptor),
        .bDescriptorType        = USB_DTYPE_CONFIGURATION,
        .wTotalLength           = sizeof(struct cdc_config),
#ifdef ENABLE_HID_COMBO
        .bNumInterfaces         = 3,
#else
        .bNumInterfaces         = 2,
#endif //ENABLE_HID_COMBO
        .bConfigurationValue    = 1,
        .iConfiguration         = NO_DESCRIPTOR,
        .bmAttributes           = USB_CFG_ATTR_RESERVED | USB_CFG_ATTR_SELFPOWERED,
        .bMaxPower              = USB_CFG_POWER_MA(100),
    },
    .comm_iad = {
        .bLength = sizeof(struct usb_iad_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFASEASSOC,
        .bFirstInterface        = 0,
        .bInterfaceCount        = 2,
        .bFunctionClass         = USB_CLASS_CDC,
        .bFunctionSubClass      = USB_CDC_SUBCLASS_ACM,
        .bFunctionProtocol      = CDC_PROTOCOL,
        .iFunction              = NO_DESCRIPTOR,
    },
    .comm = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = 0,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 1,
        .bInterfaceClass        = USB_CLASS_CDC,
        .bInterfaceSubClass     = USB_CDC_SUBCLASS_ACM,
        .bInterfaceProtocol     = CDC_PROTOCOL,
        .iInterface             = NO_DESCRIPTOR,
    },
    .cdc_hdr = {
        .bFunctionLength        = sizeof(struct usb_cdc_header_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_HEADER,
        .bcdCDC                 = VERSION_BCD(1,1,0),
    },
    .cdc_mgmt = {
        .bFunctionLength        = sizeof(struct usb_cdc_call_mgmt_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_CALL_MANAGEMENT,
        .bmCapabilities         = 0,
        .bDataInterface         = 1,

    },
    .cdc_acm = {
        .bFunctionLength        = sizeof(struct usb_cdc_acm_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_ACM,
        .bmCapabilities         = 0,
    },
    .cdc_union = {
        .bFunctionLength        = sizeof(struct usb_cdc_union_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_UNION,
        .bMasterInterface0      = 0,
        .bSlaveInterface0       = 1,
    },
    .comm_ep = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_NTF_EP,
        .bmAttributes           = USB_EPTYPE_INTERRUPT,
        .wMaxPacketSize         = CDC_NTF_SZ,
        .bInterval              = 0xFF,
    },
    .data = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = 1,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 2,
        .bInterfaceClass        = USB_CLASS_CDC_DATA,
        .bInterfaceSubClass     = USB_SUBCLASS_NONE,
        .bInterfaceProtocol     = USB_PROTO_NONE,
        .iInterface             = NO_DESCRIPTOR,
    },
    .data_eprx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_RXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CDC_DATA_SZ,
        .bInterval              = 0x01,
    },
    .data_eptx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_TXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CDC_DATA_SZ,
        .bInterval              = 0x01,
    },
#ifdef ENABLE_HID_COMBO
    .hid = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = 2,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 1,
        .bInterfaceClass        = USB_CLASS_HID,
        .bInterfaceSubClass     = USB_HID_SUBCLASS_NONBOOT,
        .bInterfaceProtocol     = USB_HID_PROTO_NONBOOT,
        .iInterface             = NO_DESCRIPTOR,
    },
    .hid_desc = {
        .bLength                = sizeof(struct usb_hid_descriptor),
        .bDescriptorType        = USB_DTYPE_HID,
        .bcdHID                 = VERSION_BCD(1,0,0),
        .bCountryCode           = USB_HID_COUNTRY_NONE,
        .bNumDescriptors        = 1,
        .bDescriptorType0       = USB_DTYPE_HID_REPORT,
        .wDescriptorLength0     = sizeof(hid_report_desc),
    },
    .hid_ep = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = HID_RIN_EP,
        .bmAttributes           = USB_EPTYPE_INTERRUPT,
        .wMaxPacketSize         = HID_RIN_SZ,
        .bInterval              = 50,
    },
#endif // ENABLE_HID_COMBO
};

static const struct usb_string_descriptor lang_desc     = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC("Open source USB stack for STM32");
static const struct usb_string_descriptor prod_desc_en  = USB_STRING_DESC("CDC Loopback demo");
static const struct usb_string_descriptor *const dtable[] = {
    &lang_desc,
    &manuf_desc_en,
    &prod_desc_en,
};

usbd_device udev;
uint32_t	ubuf[0x20];
uint8_t     fifo[0x200];
uint32_t    fpos = 0;

static struct usb_cdc_line_coding cdc_line = {
    .dwDTERate          = 38400,
    .bCharFormat        = USB_CDC_1_STOP_BITS,
    .bParityType        = USB_CDC_NO_PARITY,
    .bDataBits          = 8,
};

static struct {
    int8_t      x;
    int8_t      y;
    uint8_t     buttons;
} __attribute__((packed)) hid_report_data;

static usbd_respond cdc_getdesc (usbd_ctlreq *req, void **address, uint16_t *length) {
    const uint8_t dtype = req->wValue >> 8;
    const uint8_t dnumber = req->wValue & 0xFF;
    const void* desc;
    uint16_t len = 0;
    switch (dtype) {
    case USB_DTYPE_DEVICE:
        desc = &device_desc;
        break;
    case USB_DTYPE_CONFIGURATION:
        desc = &config_desc;
        len = sizeof(config_desc);
        break;
    case USB_DTYPE_STRING:
        if (dnumber < 3) {
            desc = dtable[dnumber];
        } else {
            return usbd_fail;
        }
        break;
    default:
        return usbd_fail;
    }
    if (len == 0) {
        len = ((struct usb_header_descriptor*)desc)->bLength;
    }
    *address = (void*)desc;
    *length = len;
    return usbd_ack;
}


static usbd_respond cdc_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback)
{
  if(((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) ==
		  ((USB_REQ_INTERFACE | USB_REQ_CLASS) && req->wIndex == 0))
  {
    switch(req->bRequest)
    {
      case USB_CDC_SET_CONTROL_LINE_STATE:
        return usbd_ack;
      case USB_CDC_SET_LINE_CODING:
        memcpy(&cdc_line, req->data, sizeof(cdc_line));
        return usbd_ack;
      case USB_CDC_GET_LINE_CODING:
        dev->status.data_ptr = &cdc_line;
        dev->status.data_count = sizeof(cdc_line);
        return usbd_ack;
      default:
        return usbd_fail;
    }
  }
#ifdef ENABLE_HID_COMBO
    if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) == (USB_REQ_INTERFACE | USB_REQ_CLASS)
        && req->wIndex == 2 ) {
        switch (req->bRequest) {
        case USB_HID_SETIDLE:
            return usbd_ack;
        case USB_HID_GETREPORT:
            dev->status.data_ptr = &hid_report_data;
            dev->status.data_count = sizeof(hid_report_data);
            return usbd_ack;
        default:
            return usbd_fail;
        }
    }
    if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) == (USB_REQ_INTERFACE | USB_REQ_STANDARD)
        && req->wIndex == 2
        && req->bRequest == USB_STD_GET_DESCRIPTOR) {
        switch (req->wValue >> 8) {
        case USB_DTYPE_HID:
            dev->status.data_ptr = (uint8_t*)&(config_desc.hid_desc);
            dev->status.data_count = sizeof(config_desc.hid_desc);
            return usbd_ack;
        case USB_DTYPE_HID_REPORT:
            dev->status.data_ptr = (uint8_t*)hid_report_desc;
            dev->status.data_count = sizeof(hid_report_desc);
            return usbd_ack;
        default:
            return usbd_fail;
        }
    }
#endif // ENABLE_HID_COMBO
    return usbd_fail;
}


static void cdc_rxonly (usbd_device *dev, uint8_t event, uint8_t ep) {
   usbd_ep_read(dev, ep, fifo, CDC_DATA_SZ);
}

static void cdc_txonly(usbd_device *dev, uint8_t event, uint8_t ep) {
    static uint8_t psize = 0x00U;
    static uint8_t remained = 0x00U;
    static uint8_t lastsym = 0x00U;

    uint8_t _t = (remained < CDC_DATA_SZ) ? remained : CDC_DATA_SZ;
    // fill buffer by sequental data
    for (size_t i = 0; i < _t; ++i) {
        fifo[i] = lastsym++;
    }
    usbd_ep_write(dev, ep, fifo, _t);

    if (remained < CDC_DATA_SZ) {
        // bulk xfer completed. increase bulk size
        remained = ++psize;
    } else {
        // continue to send remained data or ZLP
        remained -= _t;
    }
}

static void cdc_rxtx(usbd_device *dev, uint8_t event, uint8_t ep)
{
  if(event == usbd_evt_eptx){
    cdc_txonly(dev, event, ep);
  }else{
    cdc_rxonly(dev, event, ep);
  }
}

/* HID mouse IN endpoint callback */
static void hid_mouse_move(usbd_device *dev, uint8_t event, uint8_t ep)
{
    static uint8_t t = 0;
    if (t < 0x10) {
        hid_report_data.x = 1;
        hid_report_data.y = 0;
    } else if (t < 0x20) {
        hid_report_data.x = 1;
        hid_report_data.y = 1;
    } else if (t < 0x30) {
        hid_report_data.x = 0;
        hid_report_data.y = 1;
    } else if (t < 0x40) {
        hid_report_data.x = -1;
        hid_report_data.y = 1;
    } else if (t < 0x50) {
        hid_report_data.x = -1;
        hid_report_data.y = 0;
    } else if (t < 0x60) {
        hid_report_data.x = -1;
        hid_report_data.y = -1;
    } else if (t < 0x70) {
        hid_report_data.x = 0;
        hid_report_data.y = -1;
    } else  {
        hid_report_data.x = 1;
        hid_report_data.y = -1;
    }
    t = (t + 1) & 0x7F;
    usbd_ep_write(dev, ep, &hid_report_data, sizeof(hid_report_data));
}

/* CDC loop callback. Both for the Data IN and Data OUT endpoint */
static void cdc_loopback(usbd_device *dev, uint8_t event, uint8_t ep) {
    int _t;
    if (fpos <= (sizeof(fifo) - CDC_DATA_SZ)) {
        _t = usbd_ep_read(dev, CDC_RXD_EP, &fifo[fpos], CDC_DATA_SZ);
        if (_t > 0) {
            fpos += _t;
        }
    }
    if (fpos > 0) {
        _t = usbd_ep_write(dev, CDC_TXD_EP, &fifo[0], (fpos < CDC_DATA_SZ) ? fpos : CDC_DATA_SZ);
        if (_t > 0) {
            memmove(&fifo[0], &fifo[_t], fpos - _t);
            fpos -= _t;
        }
    }
}

static usbd_respond cdc_setconf (usbd_device *dev, uint8_t cfg) {
    switch (cfg) {
    case 0:
        /* deconfiguring device */
#ifdef ENABLE_HID_COMBO
        usbd_ep_deconfig(dev, HID_RIN_EP);
        usbd_reg_endpoint(dev, HID_RIN_EP, 0);
#endif // ENABLE_HID_COMBO
        usbd_ep_deconfig(dev, CDC_NTF_EP);
        usbd_ep_deconfig(dev, CDC_TXD_EP);
        usbd_ep_deconfig(dev, CDC_RXD_EP);
        usbd_reg_endpoint(dev, CDC_RXD_EP, 0);
        usbd_reg_endpoint(dev, CDC_TXD_EP, 0);
        return usbd_ack;
    case 1:
        /* configuring device */
        usbd_ep_config(dev, CDC_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_DATA_SZ);
        usbd_ep_config(dev, CDC_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_DATA_SZ);
        usbd_ep_config(dev, CDC_NTF_EP, USB_EPTYPE_INTERRUPT, CDC_NTF_SZ);
#if defined(CDC_LOOPBACK)
        usbd_reg_endpoint(dev, CDC_RXD_EP, cdc_loopback);
        usbd_reg_endpoint(dev, CDC_TXD_EP, cdc_loopback);
#elif ((CDC_TXD_EP & 0x7F) == (CDC_RXD_EP & 0x7F))
        usbd_reg_endpoint(dev, CDC_RXD_EP, cdc_rxtx);
        usbd_reg_endpoint(dev, CDC_TXD_EP, cdc_rxtx);
#else
        usbd_reg_endpoint(dev, CDC_RXD_EP, cdc_rxonly);
        usbd_reg_endpoint(dev, CDC_TXD_EP, cdc_txonly);
#endif
#ifdef ENABLE_HID_COMBO
        usbd_ep_config(dev, HID_RIN_EP, USB_EPTYPE_INTERRUPT, HID_RIN_SZ);
        usbd_reg_endpoint(dev, HID_RIN_EP, hid_mouse_move);
        usbd_ep_write(dev, HID_RIN_EP, 0, 0);
#endif // ENABLE_HID_COMBO
        usbd_ep_write(dev, CDC_TXD_EP, 0, 0);
        return usbd_ack;
    default:
        return usbd_fail;
    }
}

static void cdc_init_usbd(void)
{
  usbd_init(&udev, &usbd_hw, CDC_EP0_SIZE, ubuf, sizeof(ubuf));
  //usbd_init(&udev, &usbd_otgfs, CDC_EP0_SIZE, ubuf, sizeof(ubuf));
  usbd_reg_config(&udev, cdc_setconf);
  usbd_reg_control(&udev, cdc_control);
  usbd_reg_descr(&udev, cdc_getdesc);
}

#if defined(CDC_USE_IRQ)
#if defined(STM32L052xx) || defined(STM32F070xB) || \
	defined(STM32F042x6)
#define USB_HANDLER     USB_IRQHandler
    #define USB_NVIC_IRQ    USB_IRQn
#elif defined(STM32L100xC) || defined(STM32G4)
    #define USB_HANDLER     USB_LP_IRQHandler
    #define USB_NVIC_IRQ    USB_LP_IRQn
#elif defined(USBD_PRIMARY_OTGHS) && \
    (defined(STM32F446xx) || defined(STM32F429xx))
    #define USB_HANDLER     OTG_HS_IRQHandler
    #define USB_NVIC_IRQ    OTG_HS_IRQn
    /* WA. With __WFI/__WFE interrupt will not be fired
     * faced with F4 series and OTGHS only
     */
    #undef  __WFI
    #define __WFI __NOP
#elif defined(STM32L476xx) || defined(STM32F429xx) || \
      defined(STM32F105xC) || defined(STM32F107xC) || \
      defined(STM32F446xx) || defined(STM32F411xE) || \
      defined(STM32H743xx) || defined(STM32F767xx)
    #define USB_HANDLER     OTG_FS_IRQHandler
    #define USB_NVIC_IRQ    OTG_FS_IRQn
#elif defined(STM32F103x6)
    #define USB_HANDLER     USB_LP_CAN1_RX0_IRQHandler
    #define USB_NVIC_IRQ    USB_LP_CAN1_RX0_IRQn
#elif defined(STM32F103xE)
    #define USB_HANDLER     USB_LP_CAN1_RX0_IRQHandler
    #define USB_NVIC_IRQ    USB_LP_CAN1_RX0_IRQn
#else
    #error Not supported
#endif


void USB_HANDLER(void)
{
  usbd_poll(&udev);
}

void prvUSBappTask(void * pvParameters)
{
  cdc_init_usbd();
  usbd_enable(&udev, true);
  usbd_connect(&udev, true);
  for(;;)
  {
    //__WFI();
    vTaskDelay(1000);
  }
}


#else
void prvUSBappTask(void * pvParameters)
{
  cdc_init_usbd();
  usbd_enable(&udev, true);
  usbd_connect(&udev, true);
  for(;;)
  {
    usbd_poll(&udev);
    vTaskDelay(10);
  }
}
#endif

#elif defined(USE_SIMPLE_USB)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "board_api.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

// Increase stack size when debug log is enabled
#define USBD_STACK_SIZE    (3*configMINIMAL_STACK_SIZE/2) * (CFG_TUSB_DEBUG ? 2 : 1)


#define HID_STACK_SZIE      configMINIMAL_STACK_SIZE

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

// static timer & task
#if configSUPPORT_STATIC_ALLOCATION
StaticTimer_t blinky_tmdef;

StackType_t  usb_device_stack[USBD_STACK_SIZE];
StaticTask_t usb_device_taskdef;

StackType_t  hid_stack[HID_STACK_SZIE];
StaticTask_t hid_taskdef;
#endif

TimerHandle_t blinky_tm;

void led_blinky_cb(TimerHandle_t xTimer);
void usb_device_task(void* param);
void hid_task(void* params);

//--------------------------------------------------------------------+
// Main
//--------------------------------------------------------------------+

int main(void)
{
  board_init();

#if configSUPPORT_STATIC_ALLOCATION
  // soft timer for blinky
  blinky_tm = xTimerCreateStatic(NULL, pdMS_TO_TICKS(BLINK_NOT_MOUNTED), true, NULL, led_blinky_cb, &blinky_tmdef);

  // Create a task for tinyusb device stack
  xTaskCreateStatic(usb_device_task, "usbd", USBD_STACK_SIZE, NULL, configMAX_PRIORITIES-1, usb_device_stack, &usb_device_taskdef);

  // Create HID task
  xTaskCreateStatic(hid_task, "hid", HID_STACK_SZIE, NULL, configMAX_PRIORITIES-2, hid_stack, &hid_taskdef);
#else
  blinky_tm = xTimerCreate(NULL, pdMS_TO_TICKS(BLINK_NOT_MOUNTED), true, NULL, led_blinky_cb);
  xTaskCreate(usb_device_task, "usbd", USBD_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
  xTaskCreate(hid_task, "hid", HID_STACK_SZIE, NULL, configMAX_PRIORITIES-2, NULL);
#endif

  xTimerStart(blinky_tm, 0);

  // only start scheduler for non-espressif mcu
#ifndef ESP_PLATFORM
  vTaskStartScheduler();
#endif

  return 0;
}

#ifdef ESP_PLATFORM
void app_main(void) {
  main();
}
#endif

// USB Device Driver task
// This top level thread process all usb events and invoke callbacks
void usb_device_task(void* param)
{
  (void) param;

  // init device stack on configured roothub port
  // This should be called after scheduler/kernel is started.
  // Otherwise it could cause kernel issue since USB IRQ handler does use RTOS queue API.
  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };
  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  // RTOS forever loop
  while (1)
  {
    // put this thread to waiting state until there is new events
    tud_task();

    // following code only run if tud_task() process at least 1 event
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  xTimerChangePeriod(blinky_tm, pdMS_TO_TICKS(BLINK_MOUNTED), 0);
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  xTimerChangePeriod(blinky_tm, pdMS_TO_TICKS(BLINK_NOT_MOUNTED), 0);
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  xTimerChangePeriod(blinky_tm, pdMS_TO_TICKS(BLINK_SUSPENDED), 0);
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  if (tud_mounted())
  {
    xTimerChangePeriod(blinky_tm, pdMS_TO_TICKS(BLINK_MOUNTED), 0);
  }
  else
  {
    xTimerChangePeriod(blinky_tm, pdMS_TO_TICKS(BLINK_NOT_MOUNTED), 0);
  }
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  switch(report_id)
  {
    case REPORT_ID_KEYBOARD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_keyboard_key = false;

      if ( btn )
      {
        uint8_t keycode[6] = { 0 };
        keycode[0] = HID_KEY_A;

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        has_keyboard_key = true;
      }else
      {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
      }
    }
    break;

    case REPORT_ID_MOUSE:
    {
      int8_t const delta = 5;

      // no button, right + down, no scroll, no pan
      tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
      // use to avoid send multiple consecutive zero report
      static bool has_consumer_key = false;

      if ( btn )
      {
        // volume down
        uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
        has_consumer_key = true;
      }else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
        has_consumer_key = false;
      }
    }
    break;

    case REPORT_ID_GAMEPAD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_gamepad_key = false;

      hid_gamepad_report_t report =
      {
        .x   = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0,
        .hat = 0, .buttons = 0
      };

      if ( btn )
      {
        report.hat = GAMEPAD_HAT_UP;
        report.buttons = GAMEPAD_BUTTON_A;
        tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

        has_gamepad_key = true;
      }else
      {
        report.hat = GAMEPAD_HAT_CENTERED;
        report.buttons = 0;
        if (has_gamepad_key) tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        has_gamepad_key = false;
      }
    }
    break;

    default: break;
  }
}

void hid_task(void* param)
{
  (void) param;

  while(1)
  {
    // Poll every 10ms
    vTaskDelay(pdMS_TO_TICKS(10));

    uint32_t const btn = board_button_read();

    // Remote wakeup
    if ( tud_suspended() && btn )
    {
      // Wake up host if we are in suspend mode
      // and REMOTE_WAKEUP feature is enabled by host
      tud_remote_wakeup();
    }
    else
    {
      // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
      send_hid_report(REPORT_ID_KEYBOARD, btn);
    }
  }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;

  uint8_t next_report_id = report[0] + 1;

  if (next_report_id < REPORT_ID_COUNT)
  {
    send_hid_report(next_report_id, board_button_read());
  }
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        // Capslock On: disable blink, turn led on
        xTimerStop(blinky_tm, portMAX_DELAY);
        board_led_write(true);
      }else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        xTimerStart(blinky_tm, portMAX_DELAY);
      }
    }
  }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinky_cb(TimerHandle_t xTimer)
{
  (void) xTimer;
  static bool led_state = false;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}





#endif



