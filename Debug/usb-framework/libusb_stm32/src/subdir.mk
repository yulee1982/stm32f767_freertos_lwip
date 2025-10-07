################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/libusb_stm32/src/usbd_core.c \
../usb-framework/libusb_stm32/src/usbd_stm32f446_otgfs.c \
../usb-framework/libusb_stm32/src/usbd_stm32f446_otghs.c \
../usb-framework/libusb_stm32/src/usbd_stm32f767_otgfs.c 

OBJS += \
./usb-framework/libusb_stm32/src/usbd_core.o \
./usb-framework/libusb_stm32/src/usbd_stm32f446_otgfs.o \
./usb-framework/libusb_stm32/src/usbd_stm32f446_otghs.o \
./usb-framework/libusb_stm32/src/usbd_stm32f767_otgfs.o 

C_DEPS += \
./usb-framework/libusb_stm32/src/usbd_core.d \
./usb-framework/libusb_stm32/src/usbd_stm32f446_otgfs.d \
./usb-framework/libusb_stm32/src/usbd_stm32f446_otghs.d \
./usb-framework/libusb_stm32/src/usbd_stm32f767_otgfs.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/libusb_stm32/src/%.o usb-framework/libusb_stm32/src/%.su usb-framework/libusb_stm32/src/%.cyclo: ../usb-framework/libusb_stm32/src/%.c usb-framework/libusb_stm32/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-libusb_stm32-2f-src

clean-usb-2d-framework-2f-libusb_stm32-2f-src:
	-$(RM) ./usb-framework/libusb_stm32/src/usbd_core.cyclo ./usb-framework/libusb_stm32/src/usbd_core.d ./usb-framework/libusb_stm32/src/usbd_core.o ./usb-framework/libusb_stm32/src/usbd_core.su ./usb-framework/libusb_stm32/src/usbd_stm32f446_otgfs.cyclo ./usb-framework/libusb_stm32/src/usbd_stm32f446_otgfs.d ./usb-framework/libusb_stm32/src/usbd_stm32f446_otgfs.o ./usb-framework/libusb_stm32/src/usbd_stm32f446_otgfs.su ./usb-framework/libusb_stm32/src/usbd_stm32f446_otghs.cyclo ./usb-framework/libusb_stm32/src/usbd_stm32f446_otghs.d ./usb-framework/libusb_stm32/src/usbd_stm32f446_otghs.o ./usb-framework/libusb_stm32/src/usbd_stm32f446_otghs.su ./usb-framework/libusb_stm32/src/usbd_stm32f767_otgfs.cyclo ./usb-framework/libusb_stm32/src/usbd_stm32f767_otgfs.d ./usb-framework/libusb_stm32/src/usbd_stm32f767_otgfs.o ./usb-framework/libusb_stm32/src/usbd_stm32f767_otgfs.su

.PHONY: clean-usb-2d-framework-2f-libusb_stm32-2f-src

