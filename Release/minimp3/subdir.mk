################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../minimp3/audio_common.c \
../minimp3/music_app.c \
../minimp3/proc_files.c \
../minimp3/proc_player.c 

OBJS += \
./minimp3/audio_common.o \
./minimp3/music_app.o \
./minimp3/proc_files.o \
./minimp3/proc_player.o 

C_DEPS += \
./minimp3/audio_common.d \
./minimp3/music_app.d \
./minimp3/proc_files.d \
./minimp3/proc_player.d 


# Each subdirectory must supply rules for building sources it contributes
minimp3/%.o minimp3/%.su minimp3/%.cyclo: ../minimp3/%.c minimp3/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/minimp3" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-minimp3

clean-minimp3:
	-$(RM) ./minimp3/audio_common.cyclo ./minimp3/audio_common.d ./minimp3/audio_common.o ./minimp3/audio_common.su ./minimp3/music_app.cyclo ./minimp3/music_app.d ./minimp3/music_app.o ./minimp3/music_app.su ./minimp3/proc_files.cyclo ./minimp3/proc_files.d ./minimp3/proc_files.o ./minimp3/proc_files.su ./minimp3/proc_player.cyclo ./minimp3/proc_player.d ./minimp3/proc_player.o ./minimp3/proc_player.su

.PHONY: clean-minimp3

