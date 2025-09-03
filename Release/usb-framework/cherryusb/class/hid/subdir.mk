################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/cherryusb/class/hid/usbd_hid.c \
../usb-framework/cherryusb/class/hid/usbh_hid.c 

OBJS += \
./usb-framework/cherryusb/class/hid/usbd_hid.o \
./usb-framework/cherryusb/class/hid/usbh_hid.o 

C_DEPS += \
./usb-framework/cherryusb/class/hid/usbd_hid.d \
./usb-framework/cherryusb/class/hid/usbh_hid.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/cherryusb/class/hid/%.o usb-framework/cherryusb/class/hid/%.su usb-framework/cherryusb/class/hid/%.cyclo: ../usb-framework/cherryusb/class/hid/%.c usb-framework/cherryusb/class/hid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-cherryusb-2f-class-2f-hid

clean-usb-2d-framework-2f-cherryusb-2f-class-2f-hid:
	-$(RM) ./usb-framework/cherryusb/class/hid/usbd_hid.cyclo ./usb-framework/cherryusb/class/hid/usbd_hid.d ./usb-framework/cherryusb/class/hid/usbd_hid.o ./usb-framework/cherryusb/class/hid/usbd_hid.su ./usb-framework/cherryusb/class/hid/usbh_hid.cyclo ./usb-framework/cherryusb/class/hid/usbh_hid.d ./usb-framework/cherryusb/class/hid/usbh_hid.o ./usb-framework/cherryusb/class/hid/usbh_hid.su

.PHONY: clean-usb-2d-framework-2f-cherryusb-2f-class-2f-hid

