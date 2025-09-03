################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/cherryusb/platform/lvgl/image_cursor.c \
../usb-framework/cherryusb/platform/lvgl/usbh_hid_lvgl.c 

OBJS += \
./usb-framework/cherryusb/platform/lvgl/image_cursor.o \
./usb-framework/cherryusb/platform/lvgl/usbh_hid_lvgl.o 

C_DEPS += \
./usb-framework/cherryusb/platform/lvgl/image_cursor.d \
./usb-framework/cherryusb/platform/lvgl/usbh_hid_lvgl.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/cherryusb/platform/lvgl/%.o usb-framework/cherryusb/platform/lvgl/%.su usb-framework/cherryusb/platform/lvgl/%.cyclo: ../usb-framework/cherryusb/platform/lvgl/%.c usb-framework/cherryusb/platform/lvgl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-cherryusb-2f-platform-2f-lvgl

clean-usb-2d-framework-2f-cherryusb-2f-platform-2f-lvgl:
	-$(RM) ./usb-framework/cherryusb/platform/lvgl/image_cursor.cyclo ./usb-framework/cherryusb/platform/lvgl/image_cursor.d ./usb-framework/cherryusb/platform/lvgl/image_cursor.o ./usb-framework/cherryusb/platform/lvgl/image_cursor.su ./usb-framework/cherryusb/platform/lvgl/usbh_hid_lvgl.cyclo ./usb-framework/cherryusb/platform/lvgl/usbh_hid_lvgl.d ./usb-framework/cherryusb/platform/lvgl/usbh_hid_lvgl.o ./usb-framework/cherryusb/platform/lvgl/usbh_hid_lvgl.su

.PHONY: clean-usb-2d-framework-2f-cherryusb-2f-platform-2f-lvgl

