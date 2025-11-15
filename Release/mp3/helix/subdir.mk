################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mp3/helix/bitstream.c \
../mp3/helix/buffers.c \
../mp3/helix/dct32.c \
../mp3/helix/dequant.c \
../mp3/helix/dqchan.c \
../mp3/helix/huffman.c \
../mp3/helix/hufftabs.c \
../mp3/helix/imdct.c \
../mp3/helix/mp3dec.c \
../mp3/helix/mp3tabs.c \
../mp3/helix/polyphase.c \
../mp3/helix/scalfact.c \
../mp3/helix/stproc.c \
../mp3/helix/subband.c \
../mp3/helix/trigtabs.c 

OBJS += \
./mp3/helix/bitstream.o \
./mp3/helix/buffers.o \
./mp3/helix/dct32.o \
./mp3/helix/dequant.o \
./mp3/helix/dqchan.o \
./mp3/helix/huffman.o \
./mp3/helix/hufftabs.o \
./mp3/helix/imdct.o \
./mp3/helix/mp3dec.o \
./mp3/helix/mp3tabs.o \
./mp3/helix/polyphase.o \
./mp3/helix/scalfact.o \
./mp3/helix/stproc.o \
./mp3/helix/subband.o \
./mp3/helix/trigtabs.o 

C_DEPS += \
./mp3/helix/bitstream.d \
./mp3/helix/buffers.d \
./mp3/helix/dct32.d \
./mp3/helix/dequant.d \
./mp3/helix/dqchan.d \
./mp3/helix/huffman.d \
./mp3/helix/hufftabs.d \
./mp3/helix/imdct.d \
./mp3/helix/mp3dec.d \
./mp3/helix/mp3tabs.d \
./mp3/helix/polyphase.d \
./mp3/helix/scalfact.d \
./mp3/helix/stproc.d \
./mp3/helix/subband.d \
./mp3/helix/trigtabs.d 


# Each subdirectory must supply rules for building sources it contributes
mp3/helix/%.o mp3/helix/%.su mp3/helix/%.cyclo: ../mp3/helix/%.c mp3/helix/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-mp3-2f-helix

clean-mp3-2f-helix:
	-$(RM) ./mp3/helix/bitstream.cyclo ./mp3/helix/bitstream.d ./mp3/helix/bitstream.o ./mp3/helix/bitstream.su ./mp3/helix/buffers.cyclo ./mp3/helix/buffers.d ./mp3/helix/buffers.o ./mp3/helix/buffers.su ./mp3/helix/dct32.cyclo ./mp3/helix/dct32.d ./mp3/helix/dct32.o ./mp3/helix/dct32.su ./mp3/helix/dequant.cyclo ./mp3/helix/dequant.d ./mp3/helix/dequant.o ./mp3/helix/dequant.su ./mp3/helix/dqchan.cyclo ./mp3/helix/dqchan.d ./mp3/helix/dqchan.o ./mp3/helix/dqchan.su ./mp3/helix/huffman.cyclo ./mp3/helix/huffman.d ./mp3/helix/huffman.o ./mp3/helix/huffman.su ./mp3/helix/hufftabs.cyclo ./mp3/helix/hufftabs.d ./mp3/helix/hufftabs.o ./mp3/helix/hufftabs.su ./mp3/helix/imdct.cyclo ./mp3/helix/imdct.d ./mp3/helix/imdct.o ./mp3/helix/imdct.su ./mp3/helix/mp3dec.cyclo ./mp3/helix/mp3dec.d ./mp3/helix/mp3dec.o ./mp3/helix/mp3dec.su ./mp3/helix/mp3tabs.cyclo ./mp3/helix/mp3tabs.d ./mp3/helix/mp3tabs.o ./mp3/helix/mp3tabs.su ./mp3/helix/polyphase.cyclo ./mp3/helix/polyphase.d ./mp3/helix/polyphase.o ./mp3/helix/polyphase.su ./mp3/helix/scalfact.cyclo ./mp3/helix/scalfact.d ./mp3/helix/scalfact.o ./mp3/helix/scalfact.su ./mp3/helix/stproc.cyclo ./mp3/helix/stproc.d ./mp3/helix/stproc.o ./mp3/helix/stproc.su ./mp3/helix/subband.cyclo ./mp3/helix/subband.d ./mp3/helix/subband.o ./mp3/helix/subband.su ./mp3/helix/trigtabs.cyclo ./mp3/helix/trigtabs.d ./mp3/helix/trigtabs.o ./mp3/helix/trigtabs.su

.PHONY: clean-mp3-2f-helix

