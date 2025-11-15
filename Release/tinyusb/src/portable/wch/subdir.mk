################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/src/portable/wch/dcd_ch32_usbfs.c \
../tinyusb/src/portable/wch/dcd_ch32_usbhs.c \
../tinyusb/src/portable/wch/hcd_ch32_usbfs.c 

OBJS += \
./tinyusb/src/portable/wch/dcd_ch32_usbfs.o \
./tinyusb/src/portable/wch/dcd_ch32_usbhs.o \
./tinyusb/src/portable/wch/hcd_ch32_usbfs.o 

C_DEPS += \
./tinyusb/src/portable/wch/dcd_ch32_usbfs.d \
./tinyusb/src/portable/wch/dcd_ch32_usbhs.d \
./tinyusb/src/portable/wch/hcd_ch32_usbfs.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/src/portable/wch/%.o tinyusb/src/portable/wch/%.su tinyusb/src/portable/wch/%.cyclo: ../tinyusb/src/portable/wch/%.c tinyusb/src/portable/wch/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/music_app" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libhelix" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libflac" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/audio" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-src-2f-portable-2f-wch

clean-tinyusb-2f-src-2f-portable-2f-wch:
	-$(RM) ./tinyusb/src/portable/wch/dcd_ch32_usbfs.cyclo ./tinyusb/src/portable/wch/dcd_ch32_usbfs.d ./tinyusb/src/portable/wch/dcd_ch32_usbfs.o ./tinyusb/src/portable/wch/dcd_ch32_usbfs.su ./tinyusb/src/portable/wch/dcd_ch32_usbhs.cyclo ./tinyusb/src/portable/wch/dcd_ch32_usbhs.d ./tinyusb/src/portable/wch/dcd_ch32_usbhs.o ./tinyusb/src/portable/wch/dcd_ch32_usbhs.su ./tinyusb/src/portable/wch/hcd_ch32_usbfs.cyclo ./tinyusb/src/portable/wch/hcd_ch32_usbfs.d ./tinyusb/src/portable/wch/hcd_ch32_usbfs.o ./tinyusb/src/portable/wch/hcd_ch32_usbfs.su

.PHONY: clean-tinyusb-2f-src-2f-portable-2f-wch

