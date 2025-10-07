################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/simple_usb/class/msc/usbd_msc.c \
../usb-framework/simple_usb/class/msc/usbh_msc.c 

OBJS += \
./usb-framework/simple_usb/class/msc/usbd_msc.o \
./usb-framework/simple_usb/class/msc/usbh_msc.o 

C_DEPS += \
./usb-framework/simple_usb/class/msc/usbd_msc.d \
./usb-framework/simple_usb/class/msc/usbh_msc.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/simple_usb/class/msc/%.o usb-framework/simple_usb/class/msc/%.su usb-framework/simple_usb/class/msc/%.cyclo: ../usb-framework/simple_usb/class/msc/%.c usb-framework/simple_usb/class/msc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/core" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/common" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/adb" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/aoa" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/audio" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/cdc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/dfu" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/hid" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/hub" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/midi" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/mtp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb/class/msc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-simple_usb-2f-class-2f-msc

clean-usb-2d-framework-2f-simple_usb-2f-class-2f-msc:
	-$(RM) ./usb-framework/simple_usb/class/msc/usbd_msc.cyclo ./usb-framework/simple_usb/class/msc/usbd_msc.d ./usb-framework/simple_usb/class/msc/usbd_msc.o ./usb-framework/simple_usb/class/msc/usbd_msc.su ./usb-framework/simple_usb/class/msc/usbh_msc.cyclo ./usb-framework/simple_usb/class/msc/usbh_msc.d ./usb-framework/simple_usb/class/msc/usbh_msc.o ./usb-framework/simple_usb/class/msc/usbh_msc.su

.PHONY: clean-usb-2d-framework-2f-simple_usb-2f-class-2f-msc

