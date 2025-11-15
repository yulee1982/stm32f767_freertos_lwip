################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helix-mp3/musicapp/mp3_callback.c \
../helix-mp3/musicapp/mus_data.c \
../helix-mp3/musicapp/music_app.c \
../helix-mp3/musicapp/wav_callback.c 

OBJS += \
./helix-mp3/musicapp/mp3_callback.o \
./helix-mp3/musicapp/mus_data.o \
./helix-mp3/musicapp/music_app.o \
./helix-mp3/musicapp/wav_callback.o 

C_DEPS += \
./helix-mp3/musicapp/mp3_callback.d \
./helix-mp3/musicapp/mus_data.d \
./helix-mp3/musicapp/music_app.d \
./helix-mp3/musicapp/wav_callback.d 


# Each subdirectory must supply rules for building sources it contributes
helix-mp3/musicapp/%.o helix-mp3/musicapp/%.su helix-mp3/musicapp/%.cyclo: ../helix-mp3/musicapp/%.c helix-mp3/musicapp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libhelix" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/musicapp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-helix-2d-mp3-2f-musicapp

clean-helix-2d-mp3-2f-musicapp:
	-$(RM) ./helix-mp3/musicapp/mp3_callback.cyclo ./helix-mp3/musicapp/mp3_callback.d ./helix-mp3/musicapp/mp3_callback.o ./helix-mp3/musicapp/mp3_callback.su ./helix-mp3/musicapp/mus_data.cyclo ./helix-mp3/musicapp/mus_data.d ./helix-mp3/musicapp/mus_data.o ./helix-mp3/musicapp/mus_data.su ./helix-mp3/musicapp/music_app.cyclo ./helix-mp3/musicapp/music_app.d ./helix-mp3/musicapp/music_app.o ./helix-mp3/musicapp/music_app.su ./helix-mp3/musicapp/wav_callback.cyclo ./helix-mp3/musicapp/wav_callback.d ./helix-mp3/musicapp/wav_callback.o ./helix-mp3/musicapp/wav_callback.su

.PHONY: clean-helix-2d-mp3-2f-musicapp

