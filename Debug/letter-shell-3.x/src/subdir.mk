################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../letter-shell-3.x/src/shell.c \
../letter-shell-3.x/src/shell_cmd_list.c \
../letter-shell-3.x/src/shell_companion.c \
../letter-shell-3.x/src/shell_ext.c 

OBJS += \
./letter-shell-3.x/src/shell.o \
./letter-shell-3.x/src/shell_cmd_list.o \
./letter-shell-3.x/src/shell_companion.o \
./letter-shell-3.x/src/shell_ext.o 

C_DEPS += \
./letter-shell-3.x/src/shell.d \
./letter-shell-3.x/src/shell_cmd_list.d \
./letter-shell-3.x/src/shell_companion.d \
./letter-shell-3.x/src/shell_ext.d 


# Each subdirectory must supply rules for building sources it contributes
letter-shell-3.x/src/%.o letter-shell-3.x/src/%.su letter-shell-3.x/src/%.cyclo: ../letter-shell-3.x/src/%.c letter-shell-3.x/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/usb_app" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-letter-2d-shell-2d-3-2e-x-2f-src

clean-letter-2d-shell-2d-3-2e-x-2f-src:
	-$(RM) ./letter-shell-3.x/src/shell.cyclo ./letter-shell-3.x/src/shell.d ./letter-shell-3.x/src/shell.o ./letter-shell-3.x/src/shell.su ./letter-shell-3.x/src/shell_cmd_list.cyclo ./letter-shell-3.x/src/shell_cmd_list.d ./letter-shell-3.x/src/shell_cmd_list.o ./letter-shell-3.x/src/shell_cmd_list.su ./letter-shell-3.x/src/shell_companion.cyclo ./letter-shell-3.x/src/shell_companion.d ./letter-shell-3.x/src/shell_companion.o ./letter-shell-3.x/src/shell_companion.su ./letter-shell-3.x/src/shell_ext.cyclo ./letter-shell-3.x/src/shell_ext.d ./letter-shell-3.x/src/shell_ext.o ./letter-shell-3.x/src/shell_ext.su

.PHONY: clean-letter-2d-shell-2d-3-2e-x-2f-src

