################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helix-mp3/libflac/bitstreamf.c \
../helix-mp3/libflac/crc8.c \
../helix-mp3/libflac/flacdecoder.c \
../helix-mp3/libflac/golomb.c \
../helix-mp3/libflac/subframe_decoders.c 

OBJS += \
./helix-mp3/libflac/bitstreamf.o \
./helix-mp3/libflac/crc8.o \
./helix-mp3/libflac/flacdecoder.o \
./helix-mp3/libflac/golomb.o \
./helix-mp3/libflac/subframe_decoders.o 

C_DEPS += \
./helix-mp3/libflac/bitstreamf.d \
./helix-mp3/libflac/crc8.d \
./helix-mp3/libflac/flacdecoder.d \
./helix-mp3/libflac/golomb.d \
./helix-mp3/libflac/subframe_decoders.d 


# Each subdirectory must supply rules for building sources it contributes
helix-mp3/libflac/%.o helix-mp3/libflac/%.su helix-mp3/libflac/%.cyclo: ../helix-mp3/libflac/%.c helix-mp3/libflac/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/music_app" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libhelix" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libflac" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/audio" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-helix-2d-mp3-2f-libflac

clean-helix-2d-mp3-2f-libflac:
	-$(RM) ./helix-mp3/libflac/bitstreamf.cyclo ./helix-mp3/libflac/bitstreamf.d ./helix-mp3/libflac/bitstreamf.o ./helix-mp3/libflac/bitstreamf.su ./helix-mp3/libflac/crc8.cyclo ./helix-mp3/libflac/crc8.d ./helix-mp3/libflac/crc8.o ./helix-mp3/libflac/crc8.su ./helix-mp3/libflac/flacdecoder.cyclo ./helix-mp3/libflac/flacdecoder.d ./helix-mp3/libflac/flacdecoder.o ./helix-mp3/libflac/flacdecoder.su ./helix-mp3/libflac/golomb.cyclo ./helix-mp3/libflac/golomb.d ./helix-mp3/libflac/golomb.o ./helix-mp3/libflac/golomb.su ./helix-mp3/libflac/subframe_decoders.cyclo ./helix-mp3/libflac/subframe_decoders.d ./helix-mp3/libflac/subframe_decoders.o ./helix-mp3/libflac/subframe_decoders.su

.PHONY: clean-helix-2d-mp3-2f-libflac

