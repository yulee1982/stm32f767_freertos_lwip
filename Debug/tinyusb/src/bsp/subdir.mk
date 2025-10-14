################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/src/bsp/board.c \
../tinyusb/src/bsp/family.c 

OBJS += \
./tinyusb/src/bsp/board.o \
./tinyusb/src/bsp/family.o 

C_DEPS += \
./tinyusb/src/bsp/board.d \
./tinyusb/src/bsp/family.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/src/bsp/%.o tinyusb/src/bsp/%.su tinyusb/src/bsp/%.cyclo: ../tinyusb/src/bsp/%.c tinyusb/src/bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-src-2f-bsp

clean-tinyusb-2f-src-2f-bsp:
	-$(RM) ./tinyusb/src/bsp/board.cyclo ./tinyusb/src/bsp/board.d ./tinyusb/src/bsp/board.o ./tinyusb/src/bsp/board.su ./tinyusb/src/bsp/family.cyclo ./tinyusb/src/bsp/family.d ./tinyusb/src/bsp/family.o ./tinyusb/src/bsp/family.su

.PHONY: clean-tinyusb-2f-src-2f-bsp

