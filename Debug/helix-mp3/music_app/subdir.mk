################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helix-mp3/music_app/music_app.c \
../helix-mp3/music_app/proc_files.c \
../helix-mp3/music_app/proc_player.c \
../helix-mp3/music_app/system_mem.c 

OBJS += \
./helix-mp3/music_app/music_app.o \
./helix-mp3/music_app/proc_files.o \
./helix-mp3/music_app/proc_player.o \
./helix-mp3/music_app/system_mem.o 

C_DEPS += \
./helix-mp3/music_app/music_app.d \
./helix-mp3/music_app/proc_files.d \
./helix-mp3/music_app/proc_player.d \
./helix-mp3/music_app/system_mem.d 


# Each subdirectory must supply rules for building sources it contributes
helix-mp3/music_app/%.o helix-mp3/music_app/%.su helix-mp3/music_app/%.cyclo: ../helix-mp3/music_app/%.c helix-mp3/music_app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/music_app" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libhelix" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libflac" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/audio" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-helix-2d-mp3-2f-music_app

clean-helix-2d-mp3-2f-music_app:
	-$(RM) ./helix-mp3/music_app/music_app.cyclo ./helix-mp3/music_app/music_app.d ./helix-mp3/music_app/music_app.o ./helix-mp3/music_app/music_app.su ./helix-mp3/music_app/proc_files.cyclo ./helix-mp3/music_app/proc_files.d ./helix-mp3/music_app/proc_files.o ./helix-mp3/music_app/proc_files.su ./helix-mp3/music_app/proc_player.cyclo ./helix-mp3/music_app/proc_player.d ./helix-mp3/music_app/proc_player.o ./helix-mp3/music_app/proc_player.su ./helix-mp3/music_app/system_mem.cyclo ./helix-mp3/music_app/system_mem.d ./helix-mp3/music_app/system_mem.o ./helix-mp3/music_app/system_mem.su

.PHONY: clean-helix-2d-mp3-2f-music_app

