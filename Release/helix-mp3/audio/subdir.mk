################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helix-mp3/audio/audio_common.c \
../helix-mp3/audio/audio_flac.c \
../helix-mp3/audio/audio_mp3.c \
../helix-mp3/audio/audio_mp3_rom.c \
../helix-mp3/audio/audio_wav.c 

OBJS += \
./helix-mp3/audio/audio_common.o \
./helix-mp3/audio/audio_flac.o \
./helix-mp3/audio/audio_mp3.o \
./helix-mp3/audio/audio_mp3_rom.o \
./helix-mp3/audio/audio_wav.o 

C_DEPS += \
./helix-mp3/audio/audio_common.d \
./helix-mp3/audio/audio_flac.d \
./helix-mp3/audio/audio_mp3.d \
./helix-mp3/audio/audio_mp3_rom.d \
./helix-mp3/audio/audio_wav.d 


# Each subdirectory must supply rules for building sources it contributes
helix-mp3/audio/%.o helix-mp3/audio/%.su helix-mp3/audio/%.cyclo: ../helix-mp3/audio/%.c helix-mp3/audio/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/music_app" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libhelix" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libflac" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/audio" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-helix-2d-mp3-2f-audio

clean-helix-2d-mp3-2f-audio:
	-$(RM) ./helix-mp3/audio/audio_common.cyclo ./helix-mp3/audio/audio_common.d ./helix-mp3/audio/audio_common.o ./helix-mp3/audio/audio_common.su ./helix-mp3/audio/audio_flac.cyclo ./helix-mp3/audio/audio_flac.d ./helix-mp3/audio/audio_flac.o ./helix-mp3/audio/audio_flac.su ./helix-mp3/audio/audio_mp3.cyclo ./helix-mp3/audio/audio_mp3.d ./helix-mp3/audio/audio_mp3.o ./helix-mp3/audio/audio_mp3.su ./helix-mp3/audio/audio_mp3_rom.cyclo ./helix-mp3/audio/audio_mp3_rom.d ./helix-mp3/audio/audio_mp3_rom.o ./helix-mp3/audio/audio_mp3_rom.su ./helix-mp3/audio/audio_wav.cyclo ./helix-mp3/audio/audio_wav.d ./helix-mp3/audio/audio_wav.o ./helix-mp3/audio/audio_wav.su

.PHONY: clean-helix-2d-mp3-2f-audio

