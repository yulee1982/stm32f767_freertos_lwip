################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fatfs_r0.15b/source/diskio.c \
../fatfs_r0.15b/source/ff.c \
../fatfs_r0.15b/source/ffsystem.c \
../fatfs_r0.15b/source/ffunicode.c 

OBJS += \
./fatfs_r0.15b/source/diskio.o \
./fatfs_r0.15b/source/ff.o \
./fatfs_r0.15b/source/ffsystem.o \
./fatfs_r0.15b/source/ffunicode.o 

C_DEPS += \
./fatfs_r0.15b/source/diskio.d \
./fatfs_r0.15b/source/ff.d \
./fatfs_r0.15b/source/ffsystem.d \
./fatfs_r0.15b/source/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
fatfs_r0.15b/source/%.o fatfs_r0.15b/source/%.su fatfs_r0.15b/source/%.cyclo: ../fatfs_r0.15b/source/%.c fatfs_r0.15b/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/usb_app" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-fatfs_r0-2e-15b-2f-source

clean-fatfs_r0-2e-15b-2f-source:
	-$(RM) ./fatfs_r0.15b/source/diskio.cyclo ./fatfs_r0.15b/source/diskio.d ./fatfs_r0.15b/source/diskio.o ./fatfs_r0.15b/source/diskio.su ./fatfs_r0.15b/source/ff.cyclo ./fatfs_r0.15b/source/ff.d ./fatfs_r0.15b/source/ff.o ./fatfs_r0.15b/source/ff.su ./fatfs_r0.15b/source/ffsystem.cyclo ./fatfs_r0.15b/source/ffsystem.d ./fatfs_r0.15b/source/ffsystem.o ./fatfs_r0.15b/source/ffsystem.su ./fatfs_r0.15b/source/ffunicode.cyclo ./fatfs_r0.15b/source/ffunicode.d ./fatfs_r0.15b/source/ffunicode.o ./fatfs_r0.15b/source/ffunicode.su

.PHONY: clean-fatfs_r0-2e-15b-2f-source

