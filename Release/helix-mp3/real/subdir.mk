################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helix-mp3/real/bitstream.c \
../helix-mp3/real/buffers.c \
../helix-mp3/real/dct32.c \
../helix-mp3/real/dequant.c \
../helix-mp3/real/dqchan.c \
../helix-mp3/real/huffman.c \
../helix-mp3/real/hufftabs.c \
../helix-mp3/real/imdct.c \
../helix-mp3/real/mp3dec.c \
../helix-mp3/real/mp3tabs.c \
../helix-mp3/real/polyphase.c \
../helix-mp3/real/scalfact.c \
../helix-mp3/real/stproc.c \
../helix-mp3/real/subband.c \
../helix-mp3/real/trigtabs.c 

OBJS += \
./helix-mp3/real/bitstream.o \
./helix-mp3/real/buffers.o \
./helix-mp3/real/dct32.o \
./helix-mp3/real/dequant.o \
./helix-mp3/real/dqchan.o \
./helix-mp3/real/huffman.o \
./helix-mp3/real/hufftabs.o \
./helix-mp3/real/imdct.o \
./helix-mp3/real/mp3dec.o \
./helix-mp3/real/mp3tabs.o \
./helix-mp3/real/polyphase.o \
./helix-mp3/real/scalfact.o \
./helix-mp3/real/stproc.o \
./helix-mp3/real/subband.o \
./helix-mp3/real/trigtabs.o 

C_DEPS += \
./helix-mp3/real/bitstream.d \
./helix-mp3/real/buffers.d \
./helix-mp3/real/dct32.d \
./helix-mp3/real/dequant.d \
./helix-mp3/real/dqchan.d \
./helix-mp3/real/huffman.d \
./helix-mp3/real/hufftabs.d \
./helix-mp3/real/imdct.d \
./helix-mp3/real/mp3dec.d \
./helix-mp3/real/mp3tabs.d \
./helix-mp3/real/polyphase.d \
./helix-mp3/real/scalfact.d \
./helix-mp3/real/stproc.d \
./helix-mp3/real/subband.d \
./helix-mp3/real/trigtabs.d 


# Each subdirectory must supply rules for building sources it contributes
helix-mp3/real/%.o helix-mp3/real/%.su helix-mp3/real/%.cyclo: ../helix-mp3/real/%.c helix-mp3/real/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/real" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/music_app" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-helix-2d-mp3-2f-real

clean-helix-2d-mp3-2f-real:
	-$(RM) ./helix-mp3/real/bitstream.cyclo ./helix-mp3/real/bitstream.d ./helix-mp3/real/bitstream.o ./helix-mp3/real/bitstream.su ./helix-mp3/real/buffers.cyclo ./helix-mp3/real/buffers.d ./helix-mp3/real/buffers.o ./helix-mp3/real/buffers.su ./helix-mp3/real/dct32.cyclo ./helix-mp3/real/dct32.d ./helix-mp3/real/dct32.o ./helix-mp3/real/dct32.su ./helix-mp3/real/dequant.cyclo ./helix-mp3/real/dequant.d ./helix-mp3/real/dequant.o ./helix-mp3/real/dequant.su ./helix-mp3/real/dqchan.cyclo ./helix-mp3/real/dqchan.d ./helix-mp3/real/dqchan.o ./helix-mp3/real/dqchan.su ./helix-mp3/real/huffman.cyclo ./helix-mp3/real/huffman.d ./helix-mp3/real/huffman.o ./helix-mp3/real/huffman.su ./helix-mp3/real/hufftabs.cyclo ./helix-mp3/real/hufftabs.d ./helix-mp3/real/hufftabs.o ./helix-mp3/real/hufftabs.su ./helix-mp3/real/imdct.cyclo ./helix-mp3/real/imdct.d ./helix-mp3/real/imdct.o ./helix-mp3/real/imdct.su ./helix-mp3/real/mp3dec.cyclo ./helix-mp3/real/mp3dec.d ./helix-mp3/real/mp3dec.o ./helix-mp3/real/mp3dec.su ./helix-mp3/real/mp3tabs.cyclo ./helix-mp3/real/mp3tabs.d ./helix-mp3/real/mp3tabs.o ./helix-mp3/real/mp3tabs.su ./helix-mp3/real/polyphase.cyclo ./helix-mp3/real/polyphase.d ./helix-mp3/real/polyphase.o ./helix-mp3/real/polyphase.su ./helix-mp3/real/scalfact.cyclo ./helix-mp3/real/scalfact.d ./helix-mp3/real/scalfact.o ./helix-mp3/real/scalfact.su ./helix-mp3/real/stproc.cyclo ./helix-mp3/real/stproc.d ./helix-mp3/real/stproc.o ./helix-mp3/real/stproc.su ./helix-mp3/real/subband.cyclo ./helix-mp3/real/subband.d ./helix-mp3/real/subband.o ./helix-mp3/real/subband.su ./helix-mp3/real/trigtabs.cyclo ./helix-mp3/real/trigtabs.d ./helix-mp3/real/trigtabs.o ./helix-mp3/real/trigtabs.su

.PHONY: clean-helix-2d-mp3-2f-real

