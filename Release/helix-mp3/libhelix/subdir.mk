################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helix-mp3/libhelix/bitstream.c \
../helix-mp3/libhelix/buffers.c \
../helix-mp3/libhelix/dct32.c \
../helix-mp3/libhelix/dequant.c \
../helix-mp3/libhelix/dqchan.c \
../helix-mp3/libhelix/huffman.c \
../helix-mp3/libhelix/hufftabs.c \
../helix-mp3/libhelix/imdct.c \
../helix-mp3/libhelix/mp3dec.c \
../helix-mp3/libhelix/mp3tabs.c \
../helix-mp3/libhelix/scalfact.c \
../helix-mp3/libhelix/stproc.c \
../helix-mp3/libhelix/subband.c \
../helix-mp3/libhelix/trigtabs.c 

OBJS += \
./helix-mp3/libhelix/bitstream.o \
./helix-mp3/libhelix/buffers.o \
./helix-mp3/libhelix/dct32.o \
./helix-mp3/libhelix/dequant.o \
./helix-mp3/libhelix/dqchan.o \
./helix-mp3/libhelix/huffman.o \
./helix-mp3/libhelix/hufftabs.o \
./helix-mp3/libhelix/imdct.o \
./helix-mp3/libhelix/mp3dec.o \
./helix-mp3/libhelix/mp3tabs.o \
./helix-mp3/libhelix/scalfact.o \
./helix-mp3/libhelix/stproc.o \
./helix-mp3/libhelix/subband.o \
./helix-mp3/libhelix/trigtabs.o 

C_DEPS += \
./helix-mp3/libhelix/bitstream.d \
./helix-mp3/libhelix/buffers.d \
./helix-mp3/libhelix/dct32.d \
./helix-mp3/libhelix/dequant.d \
./helix-mp3/libhelix/dqchan.d \
./helix-mp3/libhelix/huffman.d \
./helix-mp3/libhelix/hufftabs.d \
./helix-mp3/libhelix/imdct.d \
./helix-mp3/libhelix/mp3dec.d \
./helix-mp3/libhelix/mp3tabs.d \
./helix-mp3/libhelix/scalfact.d \
./helix-mp3/libhelix/stproc.d \
./helix-mp3/libhelix/subband.d \
./helix-mp3/libhelix/trigtabs.d 


# Each subdirectory must supply rules for building sources it contributes
helix-mp3/libhelix/%.o helix-mp3/libhelix/%.su helix-mp3/libhelix/%.cyclo: ../helix-mp3/libhelix/%.c helix-mp3/libhelix/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/music_app" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libhelix" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libflac" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/audio" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-helix-2d-mp3-2f-libhelix

clean-helix-2d-mp3-2f-libhelix:
	-$(RM) ./helix-mp3/libhelix/bitstream.cyclo ./helix-mp3/libhelix/bitstream.d ./helix-mp3/libhelix/bitstream.o ./helix-mp3/libhelix/bitstream.su ./helix-mp3/libhelix/buffers.cyclo ./helix-mp3/libhelix/buffers.d ./helix-mp3/libhelix/buffers.o ./helix-mp3/libhelix/buffers.su ./helix-mp3/libhelix/dct32.cyclo ./helix-mp3/libhelix/dct32.d ./helix-mp3/libhelix/dct32.o ./helix-mp3/libhelix/dct32.su ./helix-mp3/libhelix/dequant.cyclo ./helix-mp3/libhelix/dequant.d ./helix-mp3/libhelix/dequant.o ./helix-mp3/libhelix/dequant.su ./helix-mp3/libhelix/dqchan.cyclo ./helix-mp3/libhelix/dqchan.d ./helix-mp3/libhelix/dqchan.o ./helix-mp3/libhelix/dqchan.su ./helix-mp3/libhelix/huffman.cyclo ./helix-mp3/libhelix/huffman.d ./helix-mp3/libhelix/huffman.o ./helix-mp3/libhelix/huffman.su ./helix-mp3/libhelix/hufftabs.cyclo ./helix-mp3/libhelix/hufftabs.d ./helix-mp3/libhelix/hufftabs.o ./helix-mp3/libhelix/hufftabs.su ./helix-mp3/libhelix/imdct.cyclo ./helix-mp3/libhelix/imdct.d ./helix-mp3/libhelix/imdct.o ./helix-mp3/libhelix/imdct.su ./helix-mp3/libhelix/mp3dec.cyclo ./helix-mp3/libhelix/mp3dec.d ./helix-mp3/libhelix/mp3dec.o ./helix-mp3/libhelix/mp3dec.su ./helix-mp3/libhelix/mp3tabs.cyclo ./helix-mp3/libhelix/mp3tabs.d ./helix-mp3/libhelix/mp3tabs.o ./helix-mp3/libhelix/mp3tabs.su ./helix-mp3/libhelix/scalfact.cyclo ./helix-mp3/libhelix/scalfact.d ./helix-mp3/libhelix/scalfact.o ./helix-mp3/libhelix/scalfact.su ./helix-mp3/libhelix/stproc.cyclo ./helix-mp3/libhelix/stproc.d ./helix-mp3/libhelix/stproc.o ./helix-mp3/libhelix/stproc.su ./helix-mp3/libhelix/subband.cyclo ./helix-mp3/libhelix/subband.d ./helix-mp3/libhelix/subband.o ./helix-mp3/libhelix/subband.su ./helix-mp3/libhelix/trigtabs.cyclo ./helix-mp3/libhelix/trigtabs.d ./helix-mp3/libhelix/trigtabs.o ./helix-mp3/libhelix/trigtabs.su

.PHONY: clean-helix-2d-mp3-2f-libhelix

