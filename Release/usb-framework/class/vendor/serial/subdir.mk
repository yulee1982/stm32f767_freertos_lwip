################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/class/vendor/serial/usbh_ch34x.c \
../usb-framework/class/vendor/serial/usbh_cp210x.c \
../usb-framework/class/vendor/serial/usbh_ftdi.c \
../usb-framework/class/vendor/serial/usbh_pl2303.c 

OBJS += \
./usb-framework/class/vendor/serial/usbh_ch34x.o \
./usb-framework/class/vendor/serial/usbh_cp210x.o \
./usb-framework/class/vendor/serial/usbh_ftdi.o \
./usb-framework/class/vendor/serial/usbh_pl2303.o 

C_DEPS += \
./usb-framework/class/vendor/serial/usbh_ch34x.d \
./usb-framework/class/vendor/serial/usbh_cp210x.d \
./usb-framework/class/vendor/serial/usbh_ftdi.d \
./usb-framework/class/vendor/serial/usbh_pl2303.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/class/vendor/serial/%.o usb-framework/class/vendor/serial/%.su usb-framework/class/vendor/serial/%.cyclo: ../usb-framework/class/vendor/serial/%.c usb-framework/class/vendor/serial/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-class-2f-vendor-2f-serial

clean-usb-2d-framework-2f-class-2f-vendor-2f-serial:
	-$(RM) ./usb-framework/class/vendor/serial/usbh_ch34x.cyclo ./usb-framework/class/vendor/serial/usbh_ch34x.d ./usb-framework/class/vendor/serial/usbh_ch34x.o ./usb-framework/class/vendor/serial/usbh_ch34x.su ./usb-framework/class/vendor/serial/usbh_cp210x.cyclo ./usb-framework/class/vendor/serial/usbh_cp210x.d ./usb-framework/class/vendor/serial/usbh_cp210x.o ./usb-framework/class/vendor/serial/usbh_cp210x.su ./usb-framework/class/vendor/serial/usbh_ftdi.cyclo ./usb-framework/class/vendor/serial/usbh_ftdi.d ./usb-framework/class/vendor/serial/usbh_ftdi.o ./usb-framework/class/vendor/serial/usbh_ftdi.su ./usb-framework/class/vendor/serial/usbh_pl2303.cyclo ./usb-framework/class/vendor/serial/usbh_pl2303.d ./usb-framework/class/vendor/serial/usbh_pl2303.o ./usb-framework/class/vendor/serial/usbh_pl2303.su

.PHONY: clean-usb-2d-framework-2f-class-2f-vendor-2f-serial

